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


#ifndef TEMPLATES_H
#define TEMPLATES_H

#include <cmath>
#include "constants.h"

const double template_monoclinic[][3][3] =
{
	{	{   1,    0,    0},
		{   0,    0,    0},
		{   0,    0,    0},	},

	{	{   0,    0,    0},
		{   0,    1,    0},
		{   0,    0,    0},	},

	{	{   0,    0,    0},
		{   0,    0,    0},
		{   0,    0,    1},	},

	{	{   0,    0,    1},
		{   0,    0,    0},
		{   0,    0,    0},	},

};

const double template_basemonoclinic[][3][3] =
{
	{	{ 0.5,    0,    0},
		{   0,    0,    0},
		{   0,    0,    0},	},

	{	{   0,    0,    0},
		{ 0.5,    1,    0},
		{   0,    0,    0},	},

	{	{   0,    0,    0},
		{   0,    0,    0},
		{   0,    0,    1},	},

	{	{   0,    0,    1},
		{   0,    0,    0},
		{   0,    0,    0},	},

};

const double template_orthorhombic[][3][3] =
{
	{	{   1,    0,    0},
		{   0,    0,    0},
		{   0,    0,    0},	},

	{	{   0,    0,    0},
		{   0,    1,    0},
		{   0,    0,    0},	},

	{	{   0,    0,    0},
		{   0,    0,    0},
		{   0,    0,    1},	},

};

const double template_baseco[][3][3] =
{
	{	{ 0.5, -0.5,    0},
		{   0,    0,    0},
		{   0,    0,    0},	},

	{	{   0,    0,    0},
		{ 0.5,  0.5,    0},
		{   0,    0,    0},	},

	{	{   0,    0,    0},
		{   0,    0,    0},
		{   0,    0,    1},	},

};

const double template_bco[][3][3] =
{
	{	{-0.5,  0.5,  0.5},
		{   0,    0,    0},
		{   0,    0,    0},	},

	{	{   0,    0,    0},
		{ 0.5, -0.5,  0.5},
		{   0,    0,    0},	},

	{	{   0,    0,    0},
		{   0,    0,    0},
		{ 0.5,  0.5, -0.5},	},

};

const double template_fco[][3][3] =
{
	{	{   0,    1,    1},
		{   0,    0,    0},
		{   0,    0,    0},	},

	{	{   0,    0,    0},
		{   1,    0,    1},
		{   0,    0,    0},	},

	{	{   0,    0,    0},
		{   0,    0,    0},
		{   1,    1,    0},	},

};

const double template_tetragonal[][3][3] =
{
	{	{   1,    0,    0},
		{   0,    1,    0},
		{   0,    0,    0},	},

	{	{   0,    0,    0},
		{   0,    0,    0},
		{   0,    0,    1},	},

};

const double template_bct[][3][3] =
{
	{	{-0.5,  0.5,  0.5},
		{ 0.5, -0.5,  0.5},
		{   0,    0,    0},	},

	{	{   0,    0,    0},
		{   0,    0,    0},
		{ 0.5,  0.5, -0.5},	},

};

const double template_rhombohedral[][3][3] =
{
	{	{   1,    0,    0},
		{   0,    1,    0},
		{   0,    0,    1},	},

	{	{   0,    1,    1},
		{   1,    0,    1},
		{   1,    1,    0},	},

};

const double template_hexagonal[][3][3] =
{
	{	{   1,  0.5,    0},
		{   0,sqrt(3)/2,0},
		{   0,    0,    0},	},

	{	{   0,    0,    0},
		{   0,    0,    0},
		{   0,    0,    1},	},

};

const double template_cubic[][3][3] =
{
	{	{   1,    0,    0},
		{   0,    1,    0},
		{   0,    0,    1},	},

};

const double template_bcc[][3][3] =
{
	{	{  -1,    1,    1},
		{   1,   -1,    1},
		{   1,    1,   -1},	},

};

const double template_fcc[][3][3] =
{
	{	{   0,    1,    1},
		{   1,    0,    1},
		{   1,    1,    0},	},

};

const int template_sizes[14] = {0, 4, 4, 3, 3, 3, 3, 2, 2, 2, 2, 1, 1, 1};

const double* templates[14] = {
	NULL,
	template_monoclinic[0][0],
	template_basemonoclinic[0][0],
	template_orthorhombic[0][0],
	template_baseco[0][0],
	template_bco[0][0],
	template_fco[0][0],
	template_tetragonal[0][0],
	template_bct[0][0],
	template_rhombohedral[0][0],
	template_hexagonal[0][0],
	template_cubic[0][0],
	template_bcc[0][0],
	template_fcc[0][0],
};

#endif

