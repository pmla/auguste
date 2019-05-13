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

