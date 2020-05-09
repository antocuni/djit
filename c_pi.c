#include <Python.h>
#include <stdio.h>

#include "opcodes.h"

static PyObject *const_0_long;
static PyObject *const_1_long;
static PyObject *const_0_float;
static PyObject *const_1_float;
static PyObject *const_4_float;

#define CHECK(x) { if ((x) == NULL) return NULL; }

static PyObject* compute_pi(PyObject* self, PyObject* iterations)
{
    PyObject *delta = PyNumber_TrueDivide(const_1_float, iterations);
    CHECK(delta);

    PyObject *inside = const_0_float;
    Py_INCREF(inside);

    PyObject *x = const_0_float;
    Py_INCREF(x);

    while(1) {
        int cond = PyObject_RichCompareBool(x, const_1_float, Py_LT);
        if (cond == -1)
            return NULL;
        if (!cond)
            break;

        PyObject *y = const_0_float;
        Py_INCREF(y);

        while(1) {
            int cond2 = PyObject_RichCompareBool(y, const_1_float, Py_LT);
            if (cond2 == -1)
                return NULL;
            if (!cond2)
                break;
            PyObject *xx = PyNumber_Multiply(x, x);
            CHECK(xx);
            PyObject *yy = PyNumber_Multiply(y, y);
            CHECK(yy);
            PyObject *xxyy = PyNumber_Add(xx, yy);
            CHECK(xxyy);

            int cond3 = PyObject_RichCompareBool(xxyy, const_1_float, Py_LT);
            if (cond3 == -1)
                return NULL;
            if (cond3) {
                PyObject *new_inside = PyNumber_Add(inside, const_1_float);
                CHECK(new_inside);
                Py_DECREF(inside);
                inside = new_inside;
            }
            Py_DECREF(xx);
            Py_DECREF(yy);
            Py_DECREF(xxyy);

            PyObject *new_y = PyNumber_Add(y, delta);
            CHECK(new_y);
            Py_DECREF(y);
            y = new_y;
        }

        PyObject *new_x = PyNumber_Add(x, delta);
        CHECK(new_x);
        Py_DECREF(x);
        x = new_x;
    }

    PyObject *total = PyNumber_Multiply(iterations, iterations);
    CHECK(total);

    PyObject *tmp = PyNumber_TrueDivide(inside, total);
    CHECK(tmp);
    PyObject *result = PyNumber_Multiply(tmp, const_4_float);
    Py_DECREF(tmp);
    CHECK(result);
    return result;
}

static PyObject* sum_numbers(PyObject* self, PyObject* n)
{
    PyObject *tot = const_0_long;
    Py_INCREF(tot);
    PyObject *i = const_0_long;
    Py_INCREF(i);
    while(1) {
        int cond = PyObject_RichCompareBool(i, n, Py_LT);
        if (cond == -1)
            return NULL;
        if (!cond)
            break;

        PyObject *tmp = PyNumber_Add(tot, i);
        Py_DECREF(tot);
        tot = tmp;

        PyObject *tmp2 = PyNumber_Add(i, const_1_long);
        Py_DECREF(i);
        i = tmp2;
    }
    Py_DECREF(i);
    return tot;
}

#include "sum_numbers_opcode.c"

static PyMethodDef Methods[] = {
    {"compute_pi", (PyCFunction)compute_pi, METH_O, ""},
    {"sum_numbers", (PyCFunction)sum_numbers, METH_O, ""},
    {"sum_numbers2", (PyCFunction)sum_numbers2, METH_O, ""},
    {NULL, NULL, 0, NULL}
};


static struct PyModuleDef moduledef = {
    PyModuleDef_HEAD_INIT,
    "c_pi",
    "Module Doc",
    -1,
    Methods,
    NULL,
    NULL,
    NULL,
    NULL,
};

PyMODINIT_FUNC
PyInit_c_pi(void)
{
    PyObject* m;
    m = PyModule_Create(&moduledef);
    if (m == NULL)
        return NULL;
    const_0_long = PyLong_FromLong(0);
    CHECK(const_0_long);
    const_1_long = PyLong_FromLong(1);
    CHECK(const_1_long);
    const_0_float = PyFloat_FromDouble(0);
    CHECK(const_0_float);
    const_1_float = PyFloat_FromDouble(1);
    CHECK(const_1_float);
    const_4_float = PyFloat_FromDouble(4);
    CHECK(const_4_float);
    return m;
}
