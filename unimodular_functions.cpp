#include <cstdint>
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

