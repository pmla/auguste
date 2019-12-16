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


#include <cstring>
#include <cmath>
#include <cfloat>


#define SIGN(x) (x >= 0 ? 1 : -1)
#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))
#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))


double quat_dot(double* a, double* b)
{
	return	  a[0] * b[0]
		+ a[1] * b[1]
		+ a[2] * b[2]
		+ a[3] * b[3];
}

double quat_size(double* q)
{
	return sqrt(quat_dot(q, q));
}

void normalize_quaternion(double* q)
{
	double size = quat_size(q);

	q[0] /= size;
	q[1] /= size;
	q[2] /= size;
	q[3] /= size;
}

void rotation_matrix_to_quaternion(double* u, double* q)
{
	double r11 = u[0];
	double r12 = u[1];
	double r13 = u[2];
	double r21 = u[3];
	double r22 = u[4];
	double r23 = u[5];
	double r31 = u[6];
	double r32 = u[7];
	double r33 = u[8];

	q[0] = (1.0 + r11 + r22 + r33) / 4.0;
	q[1] = (1.0 + r11 - r22 - r33) / 4.0;
	q[2] = (1.0 - r11 + r22 - r33) / 4.0;
	q[3] = (1.0 - r11 - r22 + r33) / 4.0;

	q[0] = sqrt(MAX(0, q[0]));
	q[1] = sqrt(MAX(0, q[1]));
	q[2] = sqrt(MAX(0, q[2]));
	q[3] = sqrt(MAX(0, q[3]));

	double m0 = MAX(q[0], q[1]);
	double m1 = MAX(q[2], q[3]);
	double max = MAX(m0, m1);

	int i = 0;
	for (i=0;i<4;i++)
		if (q[i] == max)
			break;

	if (i == 0)
	{
		q[1] *= SIGN(r32 - r23);
		q[2] *= SIGN(r13 - r31);
		q[3] *= SIGN(r21 - r12);
	}
	else if (i == 1)
	{
		q[0] *= SIGN(r32 - r23);
		q[2] *= SIGN(r21 + r12);
		q[3] *= SIGN(r13 + r31);
	}
	else if (i == 2)
	{
		q[0] *= SIGN(r13 - r31);
		q[1] *= SIGN(r21 + r12);
		q[3] *= SIGN(r32 + r23);
	}
	else if (i == 3)
	{
		q[0] *= SIGN(r21 - r12);
		q[1] *= SIGN(r31 + r13);
		q[2] *= SIGN(r32 + r23);
	}

	normalize_quaternion(q);
}

void quaternion_to_rotation_matrix(double* q, double* u)
{
	double a = q[0];
	double b = q[1];
	double c = q[2];
	double d = q[3];

	u[0] = a*a + b*b - c*c - d*d;
	u[1] = 2*b*c - 2*a*d;
	u[2] = 2*b*d + 2*a*c;

	u[3] = 2*b*c + 2*a*d;
	u[4] = a*a - b*b + c*c - d*d;
	u[5] = 2*c*d - 2*a*b;

	u[6] = 2*b*d - 2*a*c;
	u[7] = 2*c*d + 2*a*b;
	u[8] = a*a - b*b - c*c + d*d;
}

