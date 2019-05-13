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


#include <Python.h>
#include <ndarraytypes.h>
#include <arrayobject.h>
#include <stdbool.h>
#include "symmetrization.h"
#include "minkowski_reduction.h"
#include "constants.h"


#ifdef __cplusplus
extern "C" {
#endif

static PyObject* error(PyObject* type, const char* msg)
{
	PyErr_SetString(type, msg);
	return NULL;
}

static void transpose(int n, double* A)
{
	for (int i=0;i<n;i++)
		for (int j=i+1;j<n;j++)
		{
			double temp = A[i * n + j];
			A[i * n + j] = A[j * n + i];
			A[j * n + i] = temp;
		}
}

static bool get_unit_cell(PyObject* obj_B, double* BT)
{
	PyObject* obj_Bcont = PyArray_ContiguousFromAny(obj_B, NPY_DOUBLE, 1, 3);
	if (obj_Bcont == NULL)
		return error(PyExc_TypeError, "Invalid input data: B");

	if (PyArray_NDIM(obj_Bcont) != 2			//two-dimensional
		|| PyArray_DIM(obj_Bcont, 0) != 3		//first dim is 3
		|| PyArray_DIM(obj_Bcont, 1) != 3)		//second dim is 3
	{
		error(PyExc_TypeError, "Input must have dimensions 3x3: B");
		Py_DECREF(obj_Bcont);
		return false;
	}

	double* B = (double*)PyArray_DATA(obj_Bcont);
	if (B == NULL)
	{
		error(PyExc_TypeError, "Invalid cell matrix");
		Py_DECREF(obj_Bcont);
		return false;
	}

	memcpy(BT, B, 9 * sizeof(double));
	transpose(3, BT);

	Py_DECREF(obj_Bcont);
	return true;
}

static PyObject* symmetrize_lattice(PyObject* self, PyObject* args, PyObject* kwargs)
{
	(void)self;

	PyObject* obj_B = NULL;
	char* name = NULL;
	int search_correspondences = true;


	static const char *kwlist[] = {	(const char*)"lattice_basis",
					(const char*)"bravais_type",
					(const char*)"search_correspondences", NULL};
	if (!PyArg_ParseTupleAndKeywords(args, kwargs, "Os|p", (char**)kwlist, &obj_B, &name, &search_correspondences))
		return NULL;

	double BT[9] = {0};
	if (!get_unit_cell(obj_B, BT))
		return NULL;

	double strain = INFINITY, optT[9] = {0};
	int ret = optimize(name, BT, search_correspondences, optT, &strain);
	if (ret != 0)
	{
		if (ret == INVALID_BRAVAIS_TYPE)
			return error(PyExc_TypeError, "unrecognized bravais_type");
		else
			return error(PyExc_TypeError, "symmetrization failed");
	}

	npy_intp dim[2] = {3, 3};
	PyObject* arr_opt = PyArray_SimpleNew(2, dim, NPY_DOUBLE);
	double opt[9];
	memcpy(opt, optT, 9 * sizeof(double));
	transpose(3, opt);
	memcpy(PyArray_DATA((PyArrayObject*)arr_opt), opt, 9 * sizeof(double));

	PyObject* result = Py_BuildValue("dO", strain, arr_opt);
	Py_DECREF(arr_opt);
	return result;
}

static PyObject* calculate_vector(PyObject* self, PyObject* args, PyObject* kwargs)
{
	(void)self;

	PyObject* obj_B = NULL;
	static const char *kwlist[] = {	(const char*)"lattice_basis", NULL};
	if (!PyArg_ParseTupleAndKeywords(args, kwargs, "O", (char**)kwlist, &obj_B))
		return NULL;

	double BT[9] = {0};
	if (!get_unit_cell(obj_B, BT))
		return NULL;

	#define NUM_TYPES 14
	double strains[NUM_TYPES] = {	INFINITY, INFINITY, INFINITY, INFINITY, INFINITY, INFINITY, INFINITY,
					INFINITY, INFINITY, INFINITY, INFINITY, INFINITY, INFINITY, INFINITY};

	char* pearson[NUM_TYPES] = {	"aP", "mP", "mS", "oP", "oS", "oF", "oI",
					"tP", "tI", "hP", "hR", "cP", "cF", "cI"};

	for (int i=0;i<NUM_TYPES;i++)
	{
		double strain = 0;
		double dummy_opt[9] = {0};

		int ret = optimize(pearson[i], BT, true, dummy_opt, &strain);
		if (ret != 0)
		{
			if (ret == INVALID_BRAVAIS_TYPE)
				return error(PyExc_TypeError, "unrecognized bravais_type");
			else
				return error(PyExc_TypeError, "symmetrization failed");
		}

		strains[i] = strain;
	}

	npy_intp dim[1] = {NUM_TYPES};
	PyObject* arr_strains = PyArray_SimpleNew(1, dim, NPY_DOUBLE);
	memcpy(PyArray_DATA((PyArrayObject*)arr_strains), strains, NUM_TYPES * sizeof(double));
	return arr_strains;
}

/*
static PyObject* minkowski_reduce(PyObject* self, PyObject* args)
{
	(void)self;

	PyObject* obj_B = NULL;

	if (!PyArg_ParseTuple(args, "O", &obj_B))
		return NULL;

	double BT[9] = {0};
	if (!get_unit_cell(obj_B, BT))
		return NULL;

	double R[9] = {0};
	int path[9] = {0};
	int ret = minkowski_basis((double (*)[3])BT, (double (*)[3])R, (int (*)[3])path);

	npy_intp dim[2] = {3, 3};
	PyObject* arr_R = PyArray_SimpleNew(2, dim, NPY_DOUBLE);
	memcpy(PyArray_DATA((PyArrayObject*)arr_R), R, 9 * sizeof(double));

	PyObject* arr_path = PyArray_SimpleNew(2, dim, NPY_INT);
	memcpy(PyArray_DATA((PyArrayObject*)arr_path), path, 9 * sizeof(int));

	PyObject* result = NULL;
	if (ret == 0)
		result = Py_BuildValue("OO", arr_R, arr_path);
	else
		error(PyExc_TypeError, "Minkowski reduction failed");

	Py_DECREF(arr_R);
	Py_DECREF(arr_path);
	return result;
}
*/

static PyMethodDef auguste_methods[] = {
	{
		"symmetrize_lattice",
		(PyCFunction)symmetrize_lattice,
		METH_VARARGS | METH_KEYWORDS,
		"Symmetrize a Bravais lattice."
	},
	{
		"calculate_vector",
		(PyCFunction)calculate_vector,
		METH_VARARGS | METH_KEYWORDS,
		"Calculate a vector of distances (strains) from all Bravais lattice types."
	},
	//{
	//	"minkowski_reduce",
	//	minkowski_reduce,
	//	METH_VARARGS,
	//	"Minkowski-reduce a Bravais lattice basis."
	//},
	{NULL, NULL, 0, NULL}
};

static struct PyModuleDef auguste_definition = {
	PyModuleDef_HEAD_INIT,
	"auguste",
	"Minimum-strain auguste of Bravais lattices.",
	-1,
	auguste_methods,
	NULL,
	NULL,
	NULL,
	NULL,
};

PyMODINIT_FUNC PyInit_auguste(void)
{
	Py_Initialize();
	import_array();

	PyObject* module = PyModule_Create(&auguste_definition);
	if (module == NULL)
		goto except;

	//Adding module globals
	if (PyModule_AddObject(	module, "names",
				Py_BuildValue("ssssssssssssss",
						"primitive triclinic",
						"primitive monoclinic",
						"base-centred monoclinic",
						"primitive orthorhombic",
						"base-centred orthorhombic",
						"body-centred orthorhombic",
						"face-centred orthorhombic",
						"primitive tetragonal",
						"body-centred tetragonal",
						"primitive rhombohedral",
						"primitive hexagonal",
						"primitive cubic",
						"body-centred cubic",
						"face-centred cubic")))
		goto except;
	goto finally;

except:
	Py_XDECREF(module);
	module = NULL;
finally:
	return module;
}

#ifdef __cplusplus
}
#endif

