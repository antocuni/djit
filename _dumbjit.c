#include <stdio.h>
#include <Python.h>
#include <frameobject.h>

static Py_ssize_t g_code_extra_index;
static PyObject* eval_compute_pi(PyFrameObject *f, int throwflag);

void dj_code_extra_free(void* obj) {
}

PyObject *DumbJit_EvalFrame(PyFrameObject *f, int throwflag)
{
    void *jitted = 0;
    if (_PyCode_GetExtra((PyObject*)f->f_code, g_code_extra_index, &jitted))
        return NULL;
    if (jitted) {
        printf("compute_pi!\n");
        return eval_compute_pi(f, throwflag);
    }
    return _PyEval_EvalFrameDefault(f, throwflag);
}


static PyObject* enable(PyObject* self, PyObject* args)
{
	PyThreadState *ts = PyThreadState_Get();
	ts->interp->eval_frame = DumbJit_EvalFrame;
    Py_RETURN_NONE;
}

static PyObject* jit(PyObject* self, PyObject* func)
{
    if (!PyFunction_Check(func)) {
        PyErr_SetString(PyExc_TypeError, "Expected function object");
        return NULL;
    }
    PyObject *co = PyFunction_GetCode(func);
    _PyCode_SetExtra(co, g_code_extra_index, (void*)1);
    Py_RETURN_NONE;
}

static PyMethodDef Methods[] = {
    {"enable", (PyCFunction)enable, METH_NOARGS, ""},
    {"jit", (PyCFunction)jit, METH_O, ""},
    {NULL, NULL, 0, NULL}
};


static struct PyModuleDef moduledef = {
    PyModuleDef_HEAD_INIT,
    "_dumbjit",
    "",
    -1,
    Methods,
    NULL,
    NULL,
    NULL,
    NULL,
};


PyMODINIT_FUNC
PyInit__dumbjit(void)
{
    PyObject* m;
    m = PyModule_Create(&moduledef);
    if (m == NULL)
        return NULL;

    g_code_extra_index = _PyEval_RequestCodeExtraIndex(dj_code_extra_free);
    return m;
}




#include "opcodes.h"

static PyObject* eval_compute_pi(PyFrameObject *f, int throwflag)
{
    PyObject **consts = &(PyTuple_GET_ITEM(f->f_code->co_consts, 0));
    PyObject *stack[3];
    Py_ssize_t sp = 0;
    PyObject *f_localsplus[6] = {f->f_localsplus[0]};


    //     delta = 1.0 / iterations
    LOAD_CONST(1);            //   9           0 LOAD_CONST               1 (1.0)
    LOAD_FAST(0);             //               2 LOAD_FAST                0 (iterations)
    BINARY_TRUE_DIVIDE();     //               4 BINARY_TRUE_DIVIDE
    STORE_FAST(1);            //               6 STORE_FAST               1 (delta)

    //     inside = 0.0
    LOAD_CONST(2);            //  10           8 LOAD_CONST               2 (0.0)
    STORE_FAST(2);            //              10 STORE_FAST               2 (inside)

    //     x = 0.0
    LOAD_CONST(2);            //  11          12 LOAD_CONST               2 (0.0)
    STORE_FAST(3);            //              14 STORE_FAST               3 (x)

    //     while x < 1.0:
 l_16:
    LOAD_FAST(3);             //  12     >>   16 LOAD_FAST                3 (x)
    LOAD_CONST(1);            //              18 LOAD_CONST               1 (1.0)
    COMPARE_OP(0);            //              20 COMPARE_OP               0 (<)
    POP_JUMP_IF_FALSE(l_84);  //              22 POP_JUMP_IF_FALSE       84

    //         y = 0.0
    LOAD_CONST(2);            //  15          24 LOAD_CONST               2 (0.0)
    STORE_FAST(4);            //              26 STORE_FAST               4 (y)

    //         while y < 1.0:
 l_28:
    LOAD_FAST(4);             //  16     >>   28 LOAD_FAST                4 (y)
    LOAD_CONST(1);            //              30 LOAD_CONST               1 (1.0)
    COMPARE_OP(0);            //              32 COMPARE_OP               0 (<)
    POP_JUMP_IF_FALSE(l_74);  //              34 POP_JUMP_IF_FALSE       74

    //             if x*x + y*y < 1.0:
    LOAD_FAST(3);             //  18          36 LOAD_FAST                3 (x)
    LOAD_FAST(3);             //              38 LOAD_FAST                3 (x)
    BINARY_MULTIPLY();        //              40 BINARY_MULTIPLY
    LOAD_FAST(4);             //              42 LOAD_FAST                4 (y)
    LOAD_FAST(4);             //              44 LOAD_FAST                4 (y)
    BINARY_MULTIPLY();        //              46 BINARY_MULTIPLY
    BINARY_ADD();             //              48 BINARY_ADD
    LOAD_CONST(1);            //              50 LOAD_CONST               1 (1.0)
    COMPARE_OP(0);            //              52 COMPARE_OP               0 (<)
    POP_JUMP_IF_FALSE(l_64);  //              54 POP_JUMP_IF_FALSE       64

    //                 inside = inside + 1.0
    LOAD_FAST(2);             //  19          56 LOAD_FAST                2 (inside)
    LOAD_CONST(1);            //              58 LOAD_CONST               1 (1.0)
    BINARY_ADD();             //              60 BINARY_ADD
    STORE_FAST(2);            //              62 STORE_FAST               2 (inside)

    //             y = y + delta
 l_64:
    LOAD_FAST(4);             //  20     >>   64 LOAD_FAST                4 (y)
    LOAD_FAST(1);             //              66 LOAD_FAST                1 (delta)
    BINARY_ADD();             //              68 BINARY_ADD
    STORE_FAST(4);            //              70 STORE_FAST               4 (y)
    JUMP_ABSOLUTE(l_28);      //              72 JUMP_ABSOLUTE           28

    //         x = x + delta
 l_74:
    LOAD_FAST(3);             //  21     >>   74 LOAD_FAST                3 (x)
    LOAD_FAST(1);             //              76 LOAD_FAST                1 (delta)
    BINARY_ADD();             //              78 BINARY_ADD
    STORE_FAST(3);            //              80 STORE_FAST               3 (x)
    JUMP_ABSOLUTE(l_16);      //              82 JUMP_ABSOLUTE           16

    //     total = iterations * iterations
 l_84:
    LOAD_FAST(0);             //  22     >>   84 LOAD_FAST                0 (iterations)
    LOAD_FAST(0);             //              86 LOAD_FAST                0 (iterations)
    BINARY_MULTIPLY();        //              88 BINARY_MULTIPLY
    STORE_FAST(5);            //              90 STORE_FAST               5 (total)

    //     return inside / total * 4.0
    LOAD_FAST(2);             //  23          92 LOAD_FAST                2 (inside)
    LOAD_FAST(5);             //              94 LOAD_FAST                5 (total)
    BINARY_TRUE_DIVIDE();     //              96 BINARY_TRUE_DIVIDE
    LOAD_CONST(3);            //              98 LOAD_CONST               3 (4.0)
    BINARY_MULTIPLY();        //             100 BINARY_MULTIPLY
    RETURN_VALUE();           //             102 RETURN_VALUE
}
