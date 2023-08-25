%DefaultEncoding "UTF-8"

namespace Pyro {
%TypeHeaderCode
#include "pyro/runner.h"
%End
    bool get_keypressed();
    int get_key();
    void set_keypressed(SIP_PYCALLABLE keypressed);
    %MethodCode
        Py_XDECREF(pyKeypressed_Handler);
        pyKeypressed_Handler = a0;
        Py_INCREF(pyKeypressed_Handler);

        Pyro::set_keypressed(py_keypressed);
    %End
    
};

%ModuleCode
static PyObject *pyKeypressed_Handler = 0;
    void py_keypressed() {
        PyObject *res;

        SIP_BLOCK_THREADS

        res = sipCallMethod(0, pyKeypressed_Handler, "");
        Py_XDECREF(res);

        if (res != NULL && res != Py_None)
        {
            PyErr_SetString(PyExc_TypeError, "invalid result type from Keypressed Handler");
            res = NULL;
        }

        if (res == NULL)
            PyErr_Print();
        
        SIP_UNBLOCK_THREADS
    }
%End
