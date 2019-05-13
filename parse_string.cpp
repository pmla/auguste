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

