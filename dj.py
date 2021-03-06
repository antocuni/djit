import dis
import linecache
import textwrap

class DJ(object):

    def __init__(self, fn):
        self.b = dis.Bytecode(fn)
        print(self.b.info())
        ## print()
        ## print(self.b.dis())

    @classmethod
    def compile(cls, fn):
        self = cls(fn)
        self.emit()
        return self

    def emit(self):
        co = self.b.codeobj
        assert co.co_argcount == 1
        print(textwrap.dedent(f"""
            static PyObject* {co.co_name}(PyObject* self, PyObject* n)
            {{
                PyObject *consts[] = {{Py_None, ...}};
                PyObject *stack[{co.co_stacksize}];
                Py_ssize_t sp = 0;
                PyObject *f_localsplus[{co.co_nlocals}] = {{n}};
            """))
        for op in self.b:
            if op.starts_line is not None:
                src = linecache.getline(co.co_filename, op.starts_line)
                print()
                print('    // %s' % src, end='')

            if op.is_jump_target:
                print(' l_%d:' % op.offset) # emit a C label
            #
            if 'JUMP' in op.opname:
                line = '%s(l_%s);' % (op.opname, op.arg)
            elif op.arg is not None:
                line = '%s(%s);' % (op.opname, op.arg)
            else:
                line = '%s();' % (op.opname)
            line = line.ljust(25)
            print('    %s // %s' % (line, op._disassemble()))
        print("}")
