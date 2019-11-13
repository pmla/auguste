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


#include <map>
#include <string>
#include "constants.h"


int parse_string(char* name)
{
	std::map<std::string, int> lookup;
	lookup.insert(std::make_pair("triclinic", TRICLINIC));
	lookup.insert(std::make_pair("primitive triclinic", TRICLINIC));
	lookup.insert(std::make_pair("aP", TRICLINIC));

	lookup.insert(std::make_pair("monoclinic", MONOCLINIC));
	lookup.insert(std::make_pair("primitive monoclinic", MONOCLINIC));
	lookup.insert(std::make_pair("mP", MONOCLINIC));

	lookup.insert(std::make_pair("base-centred monoclinic", BASEMONOCLINIC));
	lookup.insert(std::make_pair("base-centered monoclinic", BASEMONOCLINIC));
	lookup.insert(std::make_pair("mS", BASEMONOCLINIC));
	lookup.insert(std::make_pair("mC", BASEMONOCLINIC));

	lookup.insert(std::make_pair("orthorhombic", ORTHORHOMBIC));
	lookup.insert(std::make_pair("primitive orthorhombic", ORTHORHOMBIC));
	lookup.insert(std::make_pair("oP", ORTHORHOMBIC));

	lookup.insert(std::make_pair("base-centred orthorhombic", BASECO));
	lookup.insert(std::make_pair("base-centered orthorhombic", BASECO));
	lookup.insert(std::make_pair("oS", BASECO));
	lookup.insert(std::make_pair("oC", BASECO));

	lookup.insert(std::make_pair("body-centred orthorhombic", BCO));
	lookup.insert(std::make_pair("body-centered orthorhombic", BCO));
	lookup.insert(std::make_pair("oF", BCO));

	lookup.insert(std::make_pair("face-centred orthorhombic", FCO));
	lookup.insert(std::make_pair("face-centered orthorhombic", FCO));
	lookup.insert(std::make_pair("oI", FCO));

	lookup.insert(std::make_pair("tetragonal", TETRAGONAL));
	lookup.insert(std::make_pair("primitive tetragonal", TETRAGONAL));
	lookup.insert(std::make_pair("tP", TETRAGONAL));

	lookup.insert(std::make_pair("body-centred tetragonal", BCT));
	lookup.insert(std::make_pair("body-centered tetragonal", BCT));
	lookup.insert(std::make_pair("tI", BCT));

	lookup.insert(std::make_pair("rhombohedral", RHOMBOHEDRAL));
	lookup.insert(std::make_pair("primitive rhombohedral", RHOMBOHEDRAL));
	lookup.insert(std::make_pair("hP", RHOMBOHEDRAL));

	lookup.insert(std::make_pair("hexagonal", HEXAGONAL));
	lookup.insert(std::make_pair("primitive hexagonal", HEXAGONAL));
	lookup.insert(std::make_pair("hR", HEXAGONAL));

	lookup.insert(std::make_pair("cubic", CUBIC));
	lookup.insert(std::make_pair("primitive cubic", CUBIC));
	lookup.insert(std::make_pair("cP", CUBIC));

	lookup.insert(std::make_pair("body-centred cubic", BCC));
	lookup.insert(std::make_pair("body-centered cubic", BCC));
	lookup.insert(std::make_pair("bcc", BCC));
	lookup.insert(std::make_pair("BCC", BCC));
	lookup.insert(std::make_pair("cF", BCC));

	lookup.insert(std::make_pair("face-centred cubic", FCC));
	lookup.insert(std::make_pair("face-centered cubic", FCC));
	lookup.insert(std::make_pair("fcc", FCC));
	lookup.insert(std::make_pair("FCC", FCC));
	lookup.insert(std::make_pair("cI", FCC));

	std::string sname = std::string(name);
	std::map<std::string, int>::iterator it = lookup.find(sname);
	if (it != lookup.end()) {
		return it->second;
	}

	return -1;
}

