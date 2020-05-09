enum cmp_op {PyCmp_LT=Py_LT, PyCmp_LE=Py_LE, PyCmp_EQ=Py_EQ, PyCmp_NE=Py_NE,
                PyCmp_GT=Py_GT, PyCmp_GE=Py_GE, PyCmp_IN, PyCmp_NOT_IN,
                PyCmp_IS, PyCmp_IS_NOT, PyCmp_EXC_MATCH, PyCmp_BAD};

static PyObject *
cmp_outcome(int op, PyObject *v, PyObject *w)
{
    int res = 0;
    switch (op) {
    case PyCmp_IS:
        res = (v == w);
        break;
    case PyCmp_IS_NOT:
        res = (v != w);
        break;
    case PyCmp_IN:
        res = PySequence_Contains(w, v);
        if (res < 0)
            return NULL;
        break;
    case PyCmp_NOT_IN:
        res = PySequence_Contains(w, v);
        if (res < 0)
            return NULL;
        res = !res;
        break;
    case PyCmp_EXC_MATCH:
        if (PyTuple_Check(w)) {
            Py_ssize_t i, length;
            length = PyTuple_Size(w);
            for (i = 0; i < length; i += 1) {
                PyObject *exc = PyTuple_GET_ITEM(w, i);
                if (!PyExceptionClass_Check(exc)) {
                    /* _PyErr_SetString(tstate, PyExc_TypeError, */
                    /*                  CANNOT_CATCH_MSG); */
                    return NULL;
                }
            }
        }
        else {
            if (!PyExceptionClass_Check(w)) {
                /* _PyErr_SetString(tstate, PyExc_TypeError, */
                /*                  CANNOT_CATCH_MSG); */
                return NULL;
            }
        }
        res = PyErr_GivenExceptionMatches(v, w);
        break;
    default:
        return PyObject_RichCompare(v, w, op);
    }
    v = res ? Py_True : Py_False;
    Py_INCREF(v);
    return v;
}


#define PUSH(x) (stack[sp++] = x)
#define POP() (stack[--sp])

#define GETITEM(array, i) (array[i])
#define SETLOCAL(oparg, value) (f_localsplus[oparg] = value)
#define GETLOCAL(oparg) (f_localsplus[oparg])

#define LOAD_CONST(oparg) {                   \
    PyObject *value = GETITEM(consts, oparg); \
    Py_INCREF(value);                         \
    PUSH(value);                              \
    }

#define STORE_FAST(oparg) {                     \
    PyObject *value = POP();                    \
    Py_XDECREF(GETLOCAL(oparg));                \
    SETLOCAL(oparg, value);                     \
    }

#define LOAD_FAST(oparg) {                      \
    PyObject *value = GETLOCAL(oparg);          \
    if (value == NULL) return NULL;             \
    Py_INCREF(value);                           \
    PUSH(value);                                \
    }


// XXX we should probably DECREF the locals
#define RETURN_VALUE() return POP()

#define INPLACE_ADD() {                                                 \
    PyObject *right = POP();                                            \
    PyObject *left = POP();                                             \
    PyObject *sum;                                                      \
    sum = PyNumber_InPlaceAdd(left, right);                             \
    Py_DECREF(left);                                                    \
    Py_DECREF(right);                                                   \
    PUSH(sum);                                                          \
    if (sum == NULL)                                                    \
        return NULL;                                                    \
    }

#define COMPARE_OP(oparg) {                                             \
    PyObject *right = POP();                                            \
    PyObject *left = POP();                                             \
    PyObject *res = cmp_outcome(oparg, left, right);                    \
    Py_DECREF(left);                                                    \
    Py_DECREF(right);                                                   \
    PUSH(res);                                                          \
    if (res == NULL) return NULL;                                       \
    }


#define POP_JUMP_IF_FALSE(target) {             \
    PyObject *cond = POP();                     \
    int err;                                    \
    if (cond == Py_False) {                     \
        Py_DECREF(cond);                        \
        goto target;                            \
    }                                           \
    err = PyObject_IsTrue(cond);                \
    Py_DECREF(cond);                            \
    if (err > 0)                                \
        ;                                       \
    else if (err == 0)                          \
        goto target;                            \
    else                                        \
        return NULL;                            \
    }

#define JUMP_ABSOLUTE(target) { goto target; }
