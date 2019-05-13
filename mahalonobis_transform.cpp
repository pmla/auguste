#include <cstring>
#include <cmath>
#include "matrix_vector.h"
#include "eigendecomposition.h"
#include "lup.h"


static void compute_matrix_inverse_product(int n, double* LU, int* pivot, double* A, double* C)
{
	// We want to calculate C = A B^{1} without inverting B.
	// We can do this by solving the linear equation C B = A
	// LU is the LU decomposition of B

	for (int j=0;j<n;j++)
		lup_solve(n, LU, pivot, &A[j * n], &C[j * n]);
}

void mahalonobis_transform(int n, double* T, double* B, double* Ktrans)
{
	// compute LU decomposition of B
	double LU[9], Hinvsqrt[16];
	int pivot[3];


	memcpy(LU, B, 9 * sizeof(double));
	transpose(3, LU);
	lup_decompose(3, LU, pivot);	//todo: check return code

	double K[4 * 9];
	for (int i=0;i<n;i++)
		compute_matrix_inverse_product(3, LU, pivot, &T[i * 9], &K[i * 9]);

	double H[4 * 4] = {0};
	for (int i=0;i<n;i++)
		for (int j=0;j<n;j++)
			H[i * n + j] += frobenius_inner_product(&K[i * 9], &K[j * 9]);

	double l[4], V[16];
	eigendecomposition(n, H, l, V);

	double d[16] = {0};
	for (int i=0;i<n;i++)
		d[i*n + i] = 1. / sqrt(l[i]);

#if 1
	double VT[16];
	double Vd[16];
	memcpy(VT, V, n * n * sizeof(double));
	transpose(n, VT);
	matmul(n, V, d, Vd);
	matmul(n, Vd, VT, Hinvsqrt);
#else
	matmul(n, V, d, Hinvsqrt);
#endif

	memset(Ktrans, 0, 9 * n * sizeof(double));
	for (int j=0;j<n;j++)
	{
		for (int i=0;i<n;i++)
		{
			double s = Hinvsqrt[j * n + i];
			for (int k=0;k<9;k++)
			{
				Ktrans[j * 9 + k] += s * K[i * 9 + k];
			}
		}
	}
}

