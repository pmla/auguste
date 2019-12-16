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
#include <algorithm>


//Consider the lowly 2 x 2 matrix
//J. Blinn, Microsoft Res., USA
//IEEE Computer Graphics and Applications (Volume: 16, Issue: 2, March 1996)
//doi: 10.1109/38.486688
static void eigendecomposition_2x2(double* H, double* l, double* V)
{
	double a = H[0];
	double b = H[1];
	double c = H[2];
	double d = H[3];

	if (b * c <= 1E-18)
	{
		l[0] = a;
		l[1] = d;
		V[0] = 1;
		V[1] = 0;
		V[2] = 0;
		V[3] = 1;
	}
	else
	{
		double sum = a + d;
		double sqrt_disc = sqrt(std::max(0., sum * sum / 4 - (a * d - b * c)));
		l[0] = sum / 2 + sqrt_disc;
		l[1] = sum / 2 - sqrt_disc;

		double dif = a - d;
		double k = sqrt(std::max(0., dif * dif / 4 + b * c));
		if (a - d >= 0)
		{
			//Blinn's article has these vectors the wrong way around
			V[0] = +(a - d) / 2 + k;
			V[2] = b;

			V[1] = c;
			V[3] = -(a - d) / 2 - k;
		}
		else
		{
			V[0] = c;
			V[2] = -(a - d) / 2 + k;

			V[1] = +(a - d) / 2 - k;
			V[3] = b;
		}

		double norm1 = sqrt(V[0] * V[0] + V[2] * V[2]);
		V[0] /= norm1;
		V[2] /= norm1;

		double norm2 = sqrt(V[1] * V[1] + V[3] * V[3]);
		V[1] /= norm2;
		V[3] /= norm2;
	}
}

static void eigendecomposition_4x4(double* H, double* l, double* V)
{
	double corners[4] = {H[0], H[3], H[12], H[15]};

	double cl[2], cV[4];
	eigendecomposition_2x2(corners, cl, cV);

	l[0] = cl[0];
	l[1] = H[5];
	l[2] = H[10];
	l[3] = cl[1];

	memset(V, 0, 16 * sizeof(double));
	V[0] = cV[0];
	V[3] = cV[1];
	V[5] = 1;
	V[10] = 1;
	V[12] = cV[2];
	V[15] = cV[3];
}

void eigendecomposition(int n, double* H, double* l, double* V)
{
	if (n == 1)
	{
		l[0] = H[0];
		V[0] = 1;
	}
	else if (n == 2)
	{
		eigendecomposition_2x2(H, l, V);
	}
	else if (n == 4)
	{
		eigendecomposition_4x4(H, l, V);
	}
	else if (n == 3)
	{
		l[0] = H[0];
		l[1] = H[4];
		l[2] = H[8];

		memset(V, 0, 9 * sizeof(double));
		V[0] = 1;
		V[4] = 1;
		V[8] = 1;
	}
}

