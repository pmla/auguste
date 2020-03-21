/*
 * The copyright in this software is being made available under the 2-clauses
 * BSD License, included below. This software may be subject to other third
 * party and contributor rights, including patent rights, and no such rights
 * are granted under this license.
 *
 * Copyright (c) 2008, Jerome Fimes, Communications & Systemes <jerome.fimes@c-s.fr>
 * All rights reserved.
 *
 * Modifided for use in "auguste" module by P. M. Larsen, 2019
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *	notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *	notice, this list of conditions and the following disclaimer in the
 *	documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS `AS IS'
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <cstring>
#include <cassert>

#define MAX_LEN 10
bool lup_decompose(int n, double* A, int* P)
{
	double swap_area[MAX_LEN];

	int* tmpPermutations = P;
	int* dstPermutations;
	int k2 = 0, t;
	double temp;
	double p;
	int lLastColum = n - 1;
	int lSwapSize = n * (int)sizeof(double);
	double* lTmpMatrix = A;
	double* lColumnMatrix, * lDestMatrix;
	int offset = 1;
	int lStride = n - 1;

	// Initialize P
	for (int i = 0; i < n; i++)
		*tmpPermutations++ = i;

	// Now make a pivot with column switch
	tmpPermutations = P;
	for (int k = 0; k < lLastColum; k++)
	{
		p = 0.0;

		// Take the middle element
		lColumnMatrix = lTmpMatrix + k;

		// Make permutation with the biggest value in the column
		for (int i = k; i < n; i++)
		{
			temp = ((*lColumnMatrix > 0) ? *lColumnMatrix : -(*lColumnMatrix));
			if (temp > p)
			{
				p = temp;
				k2 = i;
			}

			// Next line
			lColumnMatrix += n;
		}

		// A whole rest of 0 -> non singular
		if (p == 0.0)
			return false;

		// Should we permute?
		if (k2 != k)
		{
			// exchange of line
			// k2 > k
			dstPermutations = tmpPermutations + k2 - k;
			// swap indices
			t = *tmpPermutations;
			*tmpPermutations = *dstPermutations;
			*dstPermutations = t;

			// and swap entire line.
			lColumnMatrix = lTmpMatrix + (k2 - k) * n;
			memcpy(swap_area, lColumnMatrix, lSwapSize);
			memcpy(lColumnMatrix, lTmpMatrix, lSwapSize);
			memcpy(lTmpMatrix, swap_area, lSwapSize);
		}

		// now update data in the rest of the line and line after
		lDestMatrix = lTmpMatrix + k;
		lColumnMatrix = lDestMatrix + n;

		// take the middle element
		temp = *(lDestMatrix++);

		// now compute up data (i.e. coeff up of the diagonal).
		for (int i = offset; i < n; i++)
		{
			// lColumnMatrix;
			// divide the lower column elements by the diagonal value

			// A[i][k] /= A[k][k];
			// p = A[i][k]
			p = *lColumnMatrix / temp;
			*(lColumnMatrix++) = p;

			for (int j = /* k + 1 */ offset; j < n; j++)
			{
				// A[i][j] -= A[i][k] * A[k][j];
				*(lColumnMatrix++) -= p * (*(lDestMatrix++));
			}

			// come back to the k+1th element
			lDestMatrix -= lStride;

			// go to kth element of the next line
			lColumnMatrix += k;
		}

		// offset is now k+2
		++offset;

		// 1 element less for stride
		--lStride;

		// next line
		lTmpMatrix += n;

		// next permutation element
		++tmpPermutations;
	}

	return true;
}

void lup_solve(int n, double* A, int* P, double* b, double* x)
{
	double intermediate_data[MAX_LEN * MAX_LEN];

	int lStride = n + 1;
	double* lLineMatrix = A;
	double* lBeginPtr = x + n - 1;
	int* lCurrentPermutationPtr = P;


	double* lIntermediatePtr = intermediate_data;
	double* lGeneratedData = intermediate_data + n - 1;

	for (int i = 0; i < n; i++)
	{
		double sum = 0;
		double* lCurrentPtr = intermediate_data;
		double* lTmpMatrix = lLineMatrix;

		for (int j = 1; j <= i; j++)
		{
			// sum += A[i][j-1] * y[j-1];
			sum += (*(lTmpMatrix++)) * (*(lCurrentPtr++));
		}

		// y[i] = b[P[i]] - sum;
		*(lIntermediatePtr++) = b[*(lCurrentPermutationPtr++)] - sum;
		lLineMatrix += n;
	}

	// we take the last point of the A
	lLineMatrix = A + n * n - 1;

	// and we take after the last point of the destination vector
	double* lDestPtr = x + n;


	assert(n != 0);
	for (int k = (int)n - 1; k != -1; k--)
	{
		double sum = 0;
		double* lTmpMatrix = lLineMatrix;
		double u = *(lTmpMatrix++);
		double* lCurrentPtr = lDestPtr--;

		for (int j = (int)(k + 1); j < n; ++j)
		{
			// sum += A[k][j] * x[j]
			sum += (*(lTmpMatrix++)) * (*(lCurrentPtr++));
		}

		// x[k] = (y[k] - sum) / u;
		*(lBeginPtr--) = (*(lGeneratedData--) - sum) / u;
		lLineMatrix -= lStride;
	}
}

