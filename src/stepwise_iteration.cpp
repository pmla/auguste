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
#include <cstring>
#include "matrix_vector.h"
#include "polar_decomposition.h"


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

