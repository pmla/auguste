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


#include <cmath>
#include <stdint.h>
#include <utility>


double vector_dot(int n, double* a, double* b)
{
	double dot = 0;
	for (int i=0;i<n;i++)
		dot += a[i] * b[i];
	return dot;
}

double vector_norm(int n, double* x)
{
	return sqrt(vector_dot(n, x, x));
}

void normalize_vector(int n, double* x)
{
	double norm = vector_norm(n, x);

	for (int i=0;i<n;i++)
		x[i] /= norm;
}

void matvec(int n, double* A, double* x, double* b)
{
	for (int i=0;i<n;i++)
	{
		double acc = 0;
		for (int j=0;j<n;j++)
			acc += A[i * n + j] * x[j];

		b[i] = acc;
	}
}

void matveci(int n, double* A, int* x, double* b)
{
	for (int i=0;i<n;i++)
	{
		double acc = 0;
		for (int j=0;j<n;j++)
			acc += A[i * n + j] * x[j];

		b[i] = acc;
	}
}

void transpose(int n, double* A)
{
	for (int i=0;i<n;i++)
		for (int j=i+1;j<n;j++)
			std::swap(A[i * n + j], A[j * n + i]);
}

void transposei(int n, int* A)
{
	for (int i=0;i<n;i++)
		for (int j=i+1;j<n;j++)
			std::swap(A[i * n + j], A[j * n + i]);
}

double frobenius_inner_product(double* A, double* B)
{
	double acc = 0;
	for (int i=0;i<9;i++)
		acc += A[i] * B[i];
	return acc;
}

void matmul(int n, double* A, double* x, double* b)
{
	for (int i=0;i<n;i++)
	{
		for (int j=0;j<n;j++)
		{
			double acc = 0;
			for (int k=0;k<n;k++)
				acc += A[i * n + k] * x[k * n + j];

			b[i * n + j] = acc;
		}
	}
}

void matmuli(int n, int* A, int* x, int* b)
{
	for (int i=0;i<n;i++)
	{
		for (int j=0;j<n;j++)
		{
			int acc = 0;
			for (int k=0;k<n;k++)
				acc += A[i * n + k] * x[k * n + j];

			b[i * n + j] = acc;
		}
	}
}

void matmul_int8(int n, int* A, int8_t* x, int* b)
{
	for (int i=0;i<n;i++)
	{
		for (int j=0;j<n;j++)
		{
			int acc = 0;
			for (int k=0;k<n;k++)
				acc += A[i * n + k] * x[k * n + j];

			b[i * n + j] = acc;
		}
	}
}

void matmul_di(int n, double* A, int* x, double* b)
{
	for (int i=0;i<n;i++)
	{
		for (int j=0;j<n;j++)
		{
			double acc = 0;
			for (int k=0;k<n;k++)
				acc += A[i * n + k] * x[k * n + j];

			b[i * n + j] = acc;
		}
	}
}

void matmul_id(int n, int* A, double* x, double* b)
{
	for (int i=0;i<n;i++)
	{
		for (int j=0;j<n;j++)
		{
			double acc = 0;
			for (int k=0;k<n;k++)
				acc += A[i * n + k] * x[k * n + j];

			b[i * n + j] = acc;
		}
	}
}

double determinant_3x3(double* m)
{
	/*	0 1 2
		3 4 5
		6 7 8	*/

	return    m[0] * (m[4] * m[8] - m[5] * m[7])
		- m[1] * (m[3] * m[8] - m[5] * m[6])
		+ m[2] * (m[3] * m[7] - m[4] * m[6]);
}

int determinant_3x3_i(int* m)
{
	/*	0 1 2
		3 4 5
		6 7 8	*/

	return    m[0] * (m[4] * m[8] - m[5] * m[7])
		- m[1] * (m[3] * m[8] - m[5] * m[6])
		+ m[2] * (m[3] * m[7] - m[4] * m[6]);
}

void unimodular_inverse_3x3i(int* A, int* B)
{
	int sign = determinant_3x3_i(A);

	int a11 = A[0];
	int a12 = A[1];
	int a13 = A[2];

	int a21 = A[3];
	int a22 = A[4];
	int a23 = A[5];

	int a31 = A[6];
	int a32 = A[7];
	int a33 = A[8];

	B[0] = a22 * a33 - a23 * a32;
	B[1] = a13 * a32 - a12 * a33;
	B[2] = a12 * a23 - a13 * a22;

	B[3] = a23 * a31 - a21 * a33;
	B[4] = a11 * a33 - a13 * a31;
	B[5] = a13 * a21 - a11 * a23;

	B[6] = a21 * a32 - a22 * a31;
	B[7] = a12 * a31 - a11 * a32;
	B[8] = a11 * a22 - a12 * a21;

	for (int i=0;i<9;i++) {
		B[i] *= sign;
	}
}

void flip_matrix(int n, double* m)
{
	for (int i=0;i<n*n;i++)
		m[i] = -m[i];
}

void flip_matrix_i(int n, int* m)
{
	for (int i=0;i<n*n;i++)
		m[i] = -m[i];
}
