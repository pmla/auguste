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


#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <cstring>
#include "lup_decomposition.h"
#include "matrix_vector.h"
#include "quaternion.h"


#define HESSIAN(a,b) hessian[(a) * (6 + n) + b]

double newton_lagrange_step(int n, double* args, double (*key)[4][4], double* step)
{
	double k = args[0];	//kappa
	double l = args[1];	//lambda

	double qw = args[2];
	double qx = args[3];
	double qy = args[4];
	double qz = args[5];

	double* q = &args[2];
	double* x = &args[6];

	double gradient[10] = {1, 1, -2*l*qw, -2*l*qx, -2*l*qy, -2*l*qz, 0, 0, 0, 0};

	double hessian[10 * 10];
	memset(hessian, 0, 10 * 10 * sizeof(double));

	gradient[0] += -vector_dot(n, x, x);
	gradient[1] += -vector_dot(4, q, q);

	HESSIAN(2,2) += -2 * l;
	HESSIAN(3,3) += -2 * l;
	HESSIAN(4,4) += -2 * l;
	HESSIAN(5,5) += -2 * l;

	for (int i=0;i<n;i++)
		HESSIAN(6+i,6+i) += -2 * k;

	for (int i=0;i<4;i++)
		HESSIAN(1,2 + i) = -2 * q[i];

	for (int i=0;i<n;i++)
	{
		double r[4];
		matvec(4, (double*)key[i], q, r);

		gradient[2] += 2 * x[i] * r[0];
		gradient[3] += 2 * x[i] * r[1];
		gradient[4] += 2 * x[i] * r[2];
		gradient[5] += 2 * x[i] * r[3];
		gradient[6 + i] += -2 * x[i] * k + quat_dot(q, r);

		HESSIAN(0,6 + i) = -2 * x[i];

		HESSIAN(2,2) += 2 * x[i] * key[i][0][0];
		HESSIAN(2,3) += 2 * x[i] * key[i][0][1];
		HESSIAN(2,4) += 2 * x[i] * key[i][0][2];
		HESSIAN(2,5) += 2 * x[i] * key[i][0][3];
		HESSIAN(2,6 + i) = 2 * r[0];

		HESSIAN(3,3) += 2 * x[i] * key[i][1][1];
		HESSIAN(3,4) += 2 * x[i] * key[i][1][2];
		HESSIAN(3,5) += 2 * x[i] * key[i][1][3];
		HESSIAN(3,6 + i) = 2 * r[1];

		HESSIAN(4,4) += 2 * x[i] * key[i][2][2];
		HESSIAN(4,5) += 2 * x[i] * key[i][2][3];
		HESSIAN(4,6 + i) = 2 * r[2];

		HESSIAN(5,5) += 2 * x[i] * key[i][3][3];
		HESSIAN(5,6 + i) = 2 * r[3];
	}

	// make hessian symmetric
	for (int j=0;j<6+n;j++)
		for (int i=j+1;i<6+n;i++)
			HESSIAN(i,j) = HESSIAN(j,i);

	int pivot[10];
	lup_decompose(6 + n, (double*)hessian, pivot);	//todo: check return code
	lup_solve(6 + n, (double*)hessian, pivot, gradient, step);
	return vector_norm(6 + n, gradient);
}

