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


// Implements the Minkowski lattice basis reduction algorithm.  Described in:
// Nguyen, Phong Q. and Stehlé, Damien
// "Low-dimensional Lattice Basis Reduction Revisited"
// ACM Transactions on Algorithms
// Volume 5 Issue 4, October 2009

#include <cmath>
#include <cstring>
#include <algorithm>
#include "matrix_vector.h"


#define SIGN(x) ((x > 0 ? 1 : -1))

static int gauss(double (*BT)[3], int* hu, int *hv)
{
	double u[3], v[3];
	matveci(3, (double*)BT, hu, u);
	matveci(3, (double*)BT, hv, v);

	int temp[3];
	const int max_it = 10000;	//in practice this is not exceeded
	for (int it=0;it<max_it;it++)
	{
		int x = (int)round(vector_dot(3, u, v) / vector_dot(3, u, u));

		memcpy(temp, hu, 3 * sizeof(int));
		for (int i=0;i<3;i++)
			hu[i] = hv[i] - x * hu[i];
		memcpy(hv, temp, 3 * sizeof(int));

		matveci(3, (double*)BT, hu, u);
		matveci(3, (double*)BT, hv, v);

		if (vector_dot(3, u, u) >= vector_dot(3, v, v))
		{
			memcpy(temp, hv, 3 * sizeof(int));
			memcpy(hv, hu, 3 * sizeof(int));
			memcpy(hu, temp, 3 * sizeof(int));
			return 0;
		}
	}

	return -1;
}

static int closest_vector(double* t0, double* u, double* v, int* _a)
{
	double t[2] = {t0[0], t0[1]};
	int cs[9][2] = { {0, 0}, {-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1} };

	//double sizes[9];
	double vs[9][2];
	for (int i=0;i<9;i++)
	{
		double x = cs[i][0] * u[0] + cs[i][1] * v[0];
		double y = cs[i][0] * u[1] + cs[i][1] * v[1];

		//double size = x * x + y * y;
		vs[i][0] = x;
		vs[i][1] = y;
	}

	int a[2] = {0, 0};
	double dprev = INFINITY;
	const int max_it = 10000;	//in practice this is not exceeded
	for (int it=0;it<max_it;it++)
	{
		int index = 0;
		double best = vector_dot(2, t, t);

		//todo: only test 7 best
		for (int i=0;i<9;i++)
		{
			double dx = vs[i][0] + t[0];
			double dy = vs[i][1] + t[1];
			double d = dx * dx + dy * dy;
			if (d < best)
			{
				best = d;
				index = i;
			}
		}

		if (index == 0 || best >= dprev)
		{
			_a[0] = a[0];
			_a[1] = a[1];
			return 0;
		}

		dprev = best;
		int kopt = (int)round(-vector_dot(2, t, vs[index]) / vector_dot(2, vs[index], vs[index]));
		a[0] += kopt * cs[index][0];
		a[1] += kopt * cs[index][1];

		t[0] = t0[0] + a[0] * u[0] + a[1] * v[0];
		t[1] = t0[1] + a[0] * u[1] + a[1] * v[1];
	}

	return -1;
}

static void argsort3(double* x, int* order)
{
	order[0] = 0;
	order[1] = 1;
	order[2] = 2;

	double c[3] = {x[0], x[1], x[2]};
	if (c[0] > c[1])
	{
		std::swap(c[0], c[1]);
		std::swap(order[0], order[1]);
	}

	if (c[0] > c[2])
	{
		std::swap(c[0], c[2]);
		std::swap(order[0], order[2]);
	}

	if (c[1] > c[2])
	{
		std::swap(c[1], c[2]);
		std::swap(order[1], order[2]);
	}
}

static void order_path_by_norms(double* norms, int (*path)[3])
{
	int order[3];
	argsort3(norms, order);

	int mtemp[3][3];
	memcpy(mtemp[0], path[order[0]], 3 * sizeof(int));
	memcpy(mtemp[1], path[order[1]], 3 * sizeof(int));
	memcpy(mtemp[2], path[order[2]], 3 * sizeof(int));
	memcpy(path, mtemp, 9 * sizeof(int));
}

static void column_norms(double (*BT)[3], double* norms)
{
	for (int i=0;i<3;i++)
	{
		double norm = 0;

		for (int j=0;j<3;j++)
			norm += BT[j][i] * BT[j][i];
		norms[i] = sqrt(norm);
	}
}

static void gram_schmidt(double (*Bprime)[3], double* X, double* Y)
{
	memcpy(X, Bprime[0], 3 * sizeof(double));
	normalize_vector(3, X);
	double dot = vector_dot(3, Bprime[1], X);
	for (int i=0;i<3;i++)
		Y[i] = Bprime[1][i] - dot * X[i];
	normalize_vector(3, Y);
}

static int _minkowski_basis(double (*BT)[3], double (*reduced_basis)[3], int (*output_path)[3])
{
	int ret = 0;
	int path[3][3] = {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}};
	double norms[3];
	column_norms(BT, norms);
	int sign0 = SIGN(determinant_3x3(BT[0]));


	const int max_it = 10000;	//in practice this is not exceeded
	for (int it=0;it<max_it;it++)
	{
		order_path_by_norms(norms, path);

		ret = gauss(BT, path[0], path[1]);
		if (ret != 0)
			return ret;

		double Bprime[3][3];
		matveci(3, (double*)BT, path[0], Bprime[0]);
		matveci(3, (double*)BT, path[1], Bprime[1]);
		matveci(3, (double*)BT, path[2], Bprime[2]);

		double X[3], Y[3];
		gram_schmidt(Bprime, X, Y);

		double pu[2], pv[2], pw[2], temp[3];
		matvec(3, (double*)Bprime, X, temp);
		pu[0] = temp[0];
		pv[0] = temp[1];
		pw[0] = temp[2];

		matvec(3, (double*)Bprime, Y, temp);
		pu[1] = temp[0];
		pv[1] = temp[1];
		pw[1] = temp[2];

		int nb[3] = {0, 0, 0};
		ret = closest_vector(pw, pu, pv, nb);
		if (ret != 0)
			return ret;

		for (int i=0;i<3;i++)
		{
			path[2][i] += nb[0] * path[0][i];
			path[2][i] += nb[1] * path[1][i];
		}

		matveci(3, (double*)BT, path[0], Bprime[0]);
		matveci(3, (double*)BT, path[1], Bprime[1]);
		matveci(3, (double*)BT, path[2], Bprime[2]);

		norms[0] = vector_norm(3, Bprime[0]);
		norms[1] = vector_norm(3, Bprime[1]);
		norms[2] = vector_norm(3, Bprime[2]);

		if (norms[2] >= norms[1] or (nb[0] == 0 && nb[1] == 0 && nb[2] == 0))
		{
			if (SIGN(determinant_3x3(Bprime[0])) != sign0)
			{
				for (int i=0;i<3;i++)
					for (int j=0;j<3;j++)
					{
						Bprime[i][j] = -Bprime[i][j];
						path[i][j] = -path[i][j];
					}
			}

			transpose(3, (double*)Bprime);
			transposei(3, (int*)path);
			memcpy(reduced_basis, Bprime, 9 * sizeof(double));
			memcpy(output_path, path, 9 * sizeof(int));
			return 0;
		}
	}

	return -1;
}

#ifdef __cplusplus
extern "C" {
#endif

int minkowski_basis(double (*BT)[3], double (*reduced_basis)[3], int (*output_path)[3])
{
	return _minkowski_basis(BT, reduced_basis, output_path);
}

#ifdef __cplusplus
}
#endif

