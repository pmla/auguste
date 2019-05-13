#include <cmath>
#include <cstring>
#include "matrix_vector.h"
#include "polar.h"


double calculate_trace(int n, double* x, double* Ktrans, double* Q, double* P)
{
	double t[9] = {0};
	for (int i=0;i<n;i++)
		for (int j=0;j<9;j++)
			t[j] += x[i] * Ktrans[i * 9 + j];

	polar_decomposition_3x3(t, true, Q, P);
	return P[0] + P[4] + P[8];
}

double stepwise_iteration(int n, double* x, double* Ktrans, double* Q, double* P)
{
	double trace = calculate_trace(n, x, Ktrans, Q, P);

	if (n == 1)
	{
		x[0] = 1;
	}
	else
	{
		//x[i] = trace(Q^T &Ktrans[i * 9] )
		for (int i=0;i<n;i++)
		{
			x[i] = 0;
			for (int j=0;j<9;j++)
				x[i] += Q[j] * Ktrans[i * 9 + j];
		}

		normalize_vector(n, x);
	}

	return trace;
}

double optimize_stepwise(int n, double* x, double* Ktrans, double* Q, double* P, int max_it, double tolerance)
{
	int it = 0;
	double previous = 0, dif = 0;
	for (it=0;it<max_it;it++)
	{
		double trace = stepwise_iteration(n, x, Ktrans, Q, P);
		dif = fabs(trace - previous);
		if (dif < tolerance)
			break;

		previous = trace;
	}

	if (determinant_3x3(Q) < 0)
	{
		for (int i=0;i<9;i++)
			Q[i] = -Q[i];

		for (int i=0;i<n;i++)
			x[i] = -x[i];
	}

	return previous;
}

