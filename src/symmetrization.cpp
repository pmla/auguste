/*MIT License

Copyright (c) 2019 P. M. Larsen

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.*/


#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <cassert>
#include <set>
#include <vector>
#include "mahalonobis_transform.h"
#include "matrix_vector.h"
#include "minkowski_reduction.h"
#include "quaternion.h"
#include "sqp_newton_lagrange.h"
#include "stepwise_iteration.h"
#include "unimodular_neighbourhood.h"
#include "unimodular_functions.h"
#include "templates.h"
#include "constants.h"
#include "parse_string.h"


static double optimal_scaling_factor(double* P)
{
	double trace = P[0] + P[4] + P[8];

	double normsq = 0;
	for (int i=0;i<9;i++)
		normsq += P[i] * P[i];

	return trace / normsq;
}

static void get_key_matrix(double* S, double* key)
{
	double sxx = S[0], sxy = S[1], sxz = S[2];
	double syx = S[3], syy = S[4], syz = S[5];
	double szx = S[6], szy = S[7], szz = S[8];

	 key[0] = sxx + syy + szz;   key[1] =       szy - syz;   key[2] =        sxz - szx;   key[3] =       syx - sxy;
	 key[4] =       szy - syz;   key[5] = sxx - syy - szz;   key[6] =        sxy + syx;   key[7] =       szx + sxz;
	 key[8] =       sxz - szx;   key[9] =       sxy + syx;  key[10] = -sxx + syy - szz;  key[11] =       syz + szy;
	key[12] =       syx - sxy;  key[13] =       szx + sxz;  key[14] =        syz + szy;  key[15] = -sxx -syy + szz;
}

static double optimize_lattice_basis(int n, double* x, double* T, double* B, double* Q, double* opt)
{
	// compute Mahalonobis transform
	double Ktrans[4 * 9];
	mahalonobis_transform(n, T, B, Ktrans);

	// perform stepwise iteration to get a good initial guess
	// for cubic templates (those with a single template parameter) the initial guess is optimal
	double P[9];
	double tolerance = 1E-5;
	int max_it = n == 1 ? 1 : 100;	//cubic lattice types need a single iteration only
	optimize_stepwise(n, x, Ktrans, Q, P, max_it, tolerance);

	// use Newton's method to get fast convergence from initial guess to optimal solution
	// (non-cubic templates only)
	if (n >= 2)
	{
		// need rotation in quaternion form for SQP
		double q[4];
		rotation_matrix_to_quaternion(Q, q);

		double key[4][16];
		for (int i=0;i<n;i++)
			get_key_matrix(&Ktrans[i * 9], key[i]);

		// calculate initial Lagrange multipliers
		double keysum[16] = {0};
		for (int i=0;i<n;i++)
			for (int j=0;j<16;j++)
				keysum[j] += x[i] * key[i][j];

		double temp[4];
		matvec(4, keysum, q, temp);
		double v = quat_dot(q, temp);

		// initialize solution vector
		double kappa = v / 2;
		double lambda = v;
		double args[10] = {kappa, lambda, q[0], q[1], q[2], q[3], 0, 0, 0, 0};
		for (int i=0;i<n;i++)
			args[6 + i] = x[i];

		// perform sequential quadratic programming
		for (int it=0;it<100;it++)
		{
			double step[10];
			double gradient_norm = newton_lagrange_step(n, args, (double (*)[4][4])key, step);
			for (int i=0;i<6+n;i++)
				args[i] -= step[i];

			if (gradient_norm < 1E-14)
				break;
		}

		for (int i=0;i<n;i++)
			x[i] = args[6 + i];
		normalize_vector(n, x);
	}

	// ensure that Q, P, and x are consistent, post-optimization
	calculate_trace(n, x, Ktrans, Q, P);

	// calculate optimal scaling factor
	double s = optimal_scaling_factor(P);
	for (int i=0;i<9;i++)
		P[i] *= s;

	// post-multiply strain tensor by B to get symmetrized cell (in original frame)
	matmul(3, P, B, opt);

	// calculate objective function |P - I|_F
	double obj = 0;
	for (int i=0;i<9;i++)
	{
		double t = P[i];
		if (i == 0 || i == 4 || i == 8)
			t -= 1;
		obj += t * t;
	}

	return sqrt(obj);
}

static int initialize_lattice_basis(	double* B, bool search_correspondences,
					double* R, int* path)
{
	if (search_correspondences)
	{
		int ret = minkowski_basis((double (*)[3])B, (double (*)[3])R, (int (*)[3])path);
		if (ret != 0)
			return -1;
	}
	else
	{
		// set path to identity
		memset(path, 0, 9 * sizeof(int));
		path[0] = 1;
		path[4] = 1;
		path[8] = 1;
		memcpy(R, B, 9 * sizeof(double));
	}

	if (determinant_3x3(B) < 0) {
		flip_matrix(3, R);
		flip_matrix_i(3, path);
	}

	return 0;
}

static int _optimize(	char* name,
			double* B,	//lattice basis in column-vector format
			bool search_correspondences,
			int* correspondence,
			double* rotation,
			double* symmetrized,
			double* p_strain)
{
	int type = parse_string(name);
	assert(type <= 13);
	if (type < 0)
		return INVALID_BRAVAIS_TYPE;

	// triclinic lattice has trivial solution
	if (type == TRICLINIC)
	{
		// set correspondence to identity
		memset(correspondence, 0, 9 * sizeof(int));
		correspondence[0] = 1;
		correspondence[4] = 1;
		correspondence[8] = 1;

		// set rotation to identity
		memset(rotation, 0, 9 * sizeof(double));
		rotation[0] = 1;
		rotation[4] = 1;
		rotation[8] = 1;

		memcpy(symmetrized, B, 9 * sizeof(double));
		*p_strain = 0;
		return 0;
	}

	double R[9] = {0};
	int path[9] = {0};
	int ret = initialize_lattice_basis(B, search_correspondences, R, path);
	if (ret != 0)
		return ret;

	int Lbest[9] = {1, 0, 0, 0, 1, 0, 0, 0, 1};
	double best_strain = INFINITY;
	double best_cell[9] = {0};
	std::set<uint64_t> visited;

	int num_neighbours = search_correspondences ? NUM_UNIMODULAR_NEIGHBOURS : 1;
	int max_it = search_correspondences ? 40 : 1;

	const int n = template_sizes[type];
	double* T = (double*)templates[type];

	for (int it=0;it<max_it;it++)
	{
		int L0[9];
		memcpy(L0, Lbest, 9 * sizeof(int));
		bool found = false;

		for (int i=0;i<num_neighbours;i++)
		{
			int Lcur[9];
			matmul_int8(3, L0, unimodular_neighborhood[i], Lcur);
			if (unimodular_too_large(Lcur))
				continue;

			uint64_t key = unimodular_hash(Lcur);
			if (visited.find(key) != visited.end())
				continue;
			visited.insert(key);

			double A[4 * 9];
			for (int j=0;j<n;j++)
				matmul_di(3, &T[j * 9], Lcur, &A[j * 9]);

			double x[4] = {1, 1, 1, 1};
			if (type == RHOMBOHEDRAL)
				x[1] = 0;
			normalize_vector(n, x);

			double Q[9], opt[9];
			double strain = optimize_lattice_basis(n, x, A, R, Q, opt);
			if (strain < best_strain - 1E-10)
			{
				best_strain = strain;
				memcpy(Lbest, Lcur, 9 * sizeof(int));
				memcpy(rotation, Q, 9 * sizeof(double));
				memcpy(best_cell, opt, 9 * sizeof(double));
				found = true;
			}
		}

		if (!found)
			break;
	}

	int Linverse[9] = {0};
	unimodular_inverse_3x3i(path, Linverse);
	matmul_di(3, best_cell, Linverse, symmetrized);

	int inverseLbest[9] = {0};
	unimodular_inverse_3x3i(Lbest, inverseLbest);
	matmuli(3, path, inverseLbest, correspondence);

	*p_strain = best_strain;
	return 0;
}

#ifdef __cplusplus
extern "C" {
#endif

int optimize(	char* name,
		double* B,	//lattice basis in column-vector format
		bool search_correspondences,
		int* correspondence,
		double* rotation,
		double* symmetrized,
		double* p_strain)
{
	return _optimize(name, B, search_correspondences, correspondence, rotation, symmetrized, p_strain);
}

#ifdef __cplusplus
}
#endif




/*
overall procedure:

	minkowski reduction

	search over correspondences:

		single correspondence optimization:
			-compute Mahalonobis transform
			-perform stepwise iteration
			-(optionally) perform Newton optimization of Langrangian form
			-post-multiply strain by B to get symmetrized cell in original frame
			-find optimal scaling

	-get back into original frame
		undo correspondence
		undo minkowski reduction
*/

