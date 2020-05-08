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

static PyObject* sum_numbers2(PyObject* self, PyObject* n)
{
    const Py_ssize_t co_stacksize = 2;
    PyObject *consts[] = {Py_None, const_0_long, const_1_long};
    PyObject *stack[co_stacksize];
    Py_ssize_t sp = 0;
    PyObject *f_localsplus[3] = {n, NULL, NULL};

 l_0: LOAD_CONST              ( 1); /*  (0) */
 l_2: STORE_FAST              ( 1); /*  (tot) */
 l_4: LOAD_CONST              ( 1); /*  (0) */
 l_6: STORE_FAST              ( 2); /*  (i) */
 l_8: LOAD_FAST               ( 2); /*  (i) */
 l_10: LOAD_FAST               ( 0); /*  (n) */
 l_12: COMPARE_OP_LT();
 l_14: POP_JUMP_IF_FALSE       (l_34);
 l_16: LOAD_FAST               ( 1); /*  (tot) */
 l_18: LOAD_FAST               ( 2); /*  (i) */
 l_20: INPLACE_ADD             (  );
 l_22: STORE_FAST              ( 1); /*  (tot) */
 l_24: LOAD_FAST               ( 2); /*  (i) */
 l_26: LOAD_CONST              ( 2); /*  (1) */
 l_28: INPLACE_ADD             (  );
 l_30: STORE_FAST              ( 2); /*  (i) */
 l_32: JUMP_ABSOLUTE           (l_8);
 l_34: LOAD_FAST               ( 1); /*  (tot) */
 l_36: RETURN_VALUE            (  );
}



/*
 25           0 LOAD_CONST               1 (0)
              2 STORE_FAST               1 (tot)

 26           4 LOAD_CONST               1 (0)
              6 STORE_FAST               2 (i)

 27     >>    8 LOAD_FAST                2 (i)
             10 LOAD_FAST                0 (n)
             12 COMPARE_OP               0 (<)
             14 POP_JUMP_IF_FALSE       34

 28          16 LOAD_FAST                1 (tot)
             18 LOAD_FAST                2 (i)
             20 INPLACE_ADD
             22 STORE_FAST               1 (tot)

 29          24 LOAD_FAST                2 (i)
             26 LOAD_CONST               2 (1)
             28 INPLACE_ADD
             30 STORE_FAST               2 (i)
             32 JUMP_ABSOLUTE            8

 30     >>   34 LOAD_FAST                1 (tot)
             36 RETURN_VALUE
*/
