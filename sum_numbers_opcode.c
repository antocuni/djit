static PyObject* sum_numbers2(PyObject* self, PyObject* n)
{
    PyObject *consts[] = {Py_None, const_0_long, const_1_long};
    PyObject *stack[2];
    Py_ssize_t sp = 0;
    PyObject *f_localsplus[3] = {n};


    //     tot = 0
    LOAD_CONST(1);            //  26           0 LOAD_CONST               1 (0)
    STORE_FAST(1);            //               2 STORE_FAST               1 (tot)

    //     i = 0
    LOAD_CONST(1);            //  27           4 LOAD_CONST               1 (0)
    STORE_FAST(2);            //               6 STORE_FAST               2 (i)

    //     while i < n:
 l_8:
    LOAD_FAST(2);             //  28     >>    8 LOAD_FAST                2 (i)
    LOAD_FAST(0);             //              10 LOAD_FAST                0 (n)
    COMPARE_OP(0);            //              12 COMPARE_OP               0 (<)
    POP_JUMP_IF_FALSE(l_34);  //              14 POP_JUMP_IF_FALSE       34

    //         tot += i
    LOAD_FAST(1);             //  29          16 LOAD_FAST                1 (tot)
    LOAD_FAST(2);             //              18 LOAD_FAST                2 (i)
    INPLACE_ADD();            //              20 INPLACE_ADD
    STORE_FAST(1);            //              22 STORE_FAST               1 (tot)

    //         i += 1
    LOAD_FAST(2);             //  30          24 LOAD_FAST                2 (i)
    LOAD_CONST(2);            //              26 LOAD_CONST               2 (1)
    INPLACE_ADD();            //              28 INPLACE_ADD
    STORE_FAST(2);            //              30 STORE_FAST               2 (i)
    JUMP_ABSOLUTE(l_8);       //              32 JUMP_ABSOLUTE            8

    //     return tot
 l_34:
    LOAD_FAST(1);             //  31     >>   34 LOAD_FAST                1 (tot)
    RETURN_VALUE();           //              36 RETURN_VALUE
}
