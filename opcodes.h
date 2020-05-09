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

#define COMPARE_OP_LT() {                                               \
    PyObject *right = POP();                                            \
    PyObject *left = POP();                                             \
    PyObject *res = PyObject_RichCompare(left, right, Py_LT);           \
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
