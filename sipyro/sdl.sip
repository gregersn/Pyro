%DefaultEncoding "UTF-8"

namespace Pyro {
%TypeHeaderCode
#include "pyro/sdl.h"
#include <Python.h>
#include <stdio.h>
%End
    void init();
    void run(SIP_PYCALLABLE setup, SIP_PYCALLABLE draw);
%MethodCode

        Py_XDECREF(pySetup_Handler);
        pySetup_Handler = a0;
        Py_INCREF(pySetup_Handler);

        Py_XDECREF(pyDraw_Handler);
        pyDraw_Handler = a1;
        Py_INCREF(pyDraw_Handler);

        Pyro::run(py_setup, py_draw);

%End

};

%ModuleCode
static PyObject *pySetup_Handler = 0;
static PyObject *pyDraw_Handler = 0;

    void py_setup() {
        PyObject *res;

        SIP_BLOCK_THREADS

        res = sipCallMethod(0, pySetup_Handler, "");
        Py_XDECREF(res);

        if (res != NULL && res != Py_None)
        {
            PyErr_SetString(PyExc_TypeError, "invalid result type from Setup handler");
            res = NULL;
        }

        if (res == NULL)
            PyErr_Print();

        SIP_UNBLOCK_THREADS
    }

    void py_draw() {
        PyObject *res;

        SIP_BLOCK_THREADS

        res = sipCallMethod(0, pyDraw_Handler, "");
        Py_XDECREF(res);

        if (res != NULL && res != Py_None)
        {
            PyErr_SetString(PyExc_TypeError, "invalid result type from Draw handler");
            res = NULL;
        }

        if (res == NULL)
            PyErr_Print();

        SIP_UNBLOCK_THREADS

    }
%End