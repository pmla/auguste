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


#include <stdint.h>
#include <cstdlib>


static uint64_t ipow(uint64_t base, int exp)
{
	uint64_t result = 1;
	for (;;)
	{
		if (exp & 1)
			result *= base;
		exp >>= 1;

		if (!exp)
			break;
		base *= base;
	}

	return result;
}

bool unimodular_too_large(int* L)
{
	for (int i=0;i<9;i++)
		if (abs(L[i]) > 68)
			return true;
	return false;
}

uint64_t unimodular_hash(int* L)
{
	//absolute values must be less than or equal to 68
	//log2(sum([137**i for i in range(1, 10)])) = 63.892857988354926
	//this fits in 64 bits

	uint64_t hash = 0;
	for (int i=0;i<9;i++)
	{
		uint64_t base = L[i] + 68;
		uint64_t r = ipow(base, i + 1);
		hash += r;
	}

	return hash;
}

