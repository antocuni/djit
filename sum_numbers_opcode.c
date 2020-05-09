static PyObject* sum_numbers2(PyObject* self, PyObject* n)
{
    const Py_ssize_t co_stacksize = 2;
    PyObject *consts[] = {Py_None, const_0_long, const_1_long};
    PyObject *stack[co_stacksize];
    Py_ssize_t sp = 0;
    PyObject *f_localsplus[3] = {n, NULL, NULL};

    LOAD_CONST              ( 1); /*  (0) */
    STORE_FAST              ( 1); /*  (tot) */
    LOAD_CONST              ( 1); /*  (0) */
    STORE_FAST              ( 2); /*  (i) */
 l_8:
    LOAD_FAST               ( 2); /*  (i) */
    LOAD_FAST               ( 0); /*  (n) */
    COMPARE_OP_LT();
    POP_JUMP_IF_FALSE       (l_34);
    LOAD_FAST               ( 1); /*  (tot) */
    LOAD_FAST               ( 2); /*  (i) */
    INPLACE_ADD             (  );
    STORE_FAST              ( 1); /*  (tot) */
    LOAD_FAST               ( 2); /*  (i) */
    LOAD_CONST              ( 2); /*  (1) */
    INPLACE_ADD             (  );
    STORE_FAST              ( 2); /*  (i) */
    JUMP_ABSOLUTE           (l_8);
 l_34:
    LOAD_FAST               ( 1); /*  (tot) */
    RETURN_VALUE            (  );
}
