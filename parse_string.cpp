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
#include <vector>
#include "constants.h"


typedef struct
{
	const char* name;
	const int type;
} dictionary_t;

int parse_string(char* name)
{
	std::vector< dictionary_t > lookup = {

		{"triclinic", TRICLINIC},
		{"primitive triclinic", TRICLINIC},
		{"aP", TRICLINIC},

		{"monoclinic", MONOCLINIC},
		{"primitive monoclinic", MONOCLINIC},
		{"mP", MONOCLINIC},

		{"base-centred monoclinic", BASEMONOCLINIC},
		{"base-centered monoclinic", BASEMONOCLINIC},
		{"mS", BASEMONOCLINIC},

		{"orthorhombic", ORTHORHOMBIC},
		{"primitive orthorhombic", ORTHORHOMBIC},
		{"oP", ORTHORHOMBIC},

		{"base-centred orthorhombic", BASECO},
		{"base-centered orthorhombic", BASECO},
		{"oS", BASECO},

		{"body-centred orthorhombic", BCO},
		{"body-centered orthorhombic", BCO},
		{"oF", BCO},

		{"face-centred orthorhombic", FCO},
		{"face-centered orthorhombic", FCO},
		{"oI", FCO},

		{"tetragonal", TETRAGONAL},
		{"primitive tetragonal", TETRAGONAL},
		{"tP", TETRAGONAL},

		{"body-centred tetragonal", BCT},
		{"body-centered tetragonal", BCT},
		{"tI", BCT},

		{"rhombohedral", RHOMBOHEDRAL},
		{"primitive rhombohedral", RHOMBOHEDRAL},
		{"hP", RHOMBOHEDRAL},

		{"hexagonal", HEXAGONAL},
		{"primitive hexagonal", HEXAGONAL},
		{"hR", HEXAGONAL},

		{"cubic", CUBIC},
		{"primitive cubic", CUBIC},
		{"cP", CUBIC},

		{"body-centred cubic", BCC},
		{"body-centered cubic", BCC},
		{"bcc", BCC},
		{"cF", BCC},

		{"face-centred cubic", FCC},
		{"face-centered cubic", FCC},
		{"fcc", FCC},
		{"cI", FCC},
	};

	unsigned int n = strlen(name);
	for (auto entry : lookup)
	{
		if (strlen(entry.name) == n && strncmp(name, entry.name, n) == 0)
			return entry.type;
	}

	return -1;
}

