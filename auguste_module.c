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

	PyObject* obj_Bcont = PyArray_ContiguousFromAny(obj_B, NPY_DOUBLE, 1, 3);
	if (obj_Bcont == NULL)
		return error(PyExc_TypeError, "Invalid input data: B");

	if (PyArray_NDIM(obj_Bcont) != 2			//two-dimensional
		|| PyArray_DIM(obj_Bcont, 0) != 3		//first dim is 3
		|| PyArray_DIM(obj_Bcont, 1) != 3)		//second dim is 3
		return error(PyExc_TypeError, "Input must have dimensions 3x3: B");

	double* B = (double*)PyArray_DATA(obj_Bcont);
	if (B == NULL)
		return NULL;

	double strain = INFINITY, opt[9] = {0};
	int ret = optimize(name, B, search_correspondences, opt, &strain);
	if (ret != 0)
	{
		if (ret == INVALID_BRAVAIS_TYPE)
			return error(PyExc_TypeError, "unrecognized bravais_type");
		else
			return error(PyExc_TypeError, "symmetrization failed");
	}

	npy_intp dim[2] = {3, 3};
	PyObject* arr_opt = PyArray_SimpleNew(2, dim, NPY_DOUBLE);
	memcpy(PyArray_DATA((PyArrayObject*)arr_opt), opt, 9 * sizeof(double));

	PyObject* result = Py_BuildValue("dO", strain, arr_opt);
	Py_DECREF(arr_opt);
	Py_DECREF(obj_Bcont);
	return result;
}

static PyObject* minkowski_reduce(PyObject* self, PyObject* args)
{
	(void)self;

	PyObject* obj_B = NULL;

	if (!PyArg_ParseTuple(args, "O", &obj_B))
		return NULL;

	PyObject* obj_Bcont = PyArray_ContiguousFromAny(obj_B, NPY_DOUBLE, 1, 3);
	if (obj_Bcont == NULL)
		return error(PyExc_TypeError, "Invalid input data: B");

	assert (PyArray_TYPE(obj_Bcont) == NPY_DOUBLE);
	if (PyArray_NDIM(obj_Bcont) != 2			//two-dimensional
		|| PyArray_DIM(obj_Bcont, 0) != 3		//first dim is 3
		|| PyArray_DIM(obj_Bcont, 1) != 3)		//second dim is 3
		return error(PyExc_TypeError, "Input must have dimensions 3x3: B");

	double* B = (double*)PyArray_DATA(obj_Bcont);
	if (B == NULL)
		return NULL;

	double R[9] = {0};
	int path[9] = {0};
	int ret = minkowski_basis((double (*)[3])B, (double (*)[3])R, (int (*)[3])path);

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
	Py_DECREF(obj_Bcont);
	return result;
}

static PyMethodDef auguste_methods[] = {
	{
		"symmetrize_lattice",
		(PyCFunction)symmetrize_lattice,
		METH_VARARGS | METH_KEYWORDS,
		"Symmetrize a Bravais lattice."
	},
	{
		"minkowski_reduce",
		minkowski_reduce,
		METH_VARARGS,
		"Minkowski-reduce a Bravais lattice basis."
	},
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
	return PyModule_Create(&auguste_definition);
}

#ifdef __cplusplus
}
#endif

