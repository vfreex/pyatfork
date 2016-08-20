#include <Python.h>

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "atfork.h"

#if PY_MAJOR_VERSION >= 3
#define IS_PY3K 1
#else
#define IS_PY3K 0
#endif

static PyMethodDef catfork_methods[] = {
    {"pthread_atfork", catfork_pthread_atfork, METH_VARARGS, ""},
    {NULL, NULL, 0, NULL},
};


#if IS_PY3K

// Python 3 module initialization
static struct PyModuleDef catfork = {
    PyModuleDef_HEAD_INIT,
    "catfork",
    NULL,
    -1,
    catfork_methods
};

PyMODINIT_FUNC PyInit_catfork(void)
{
    return PyModule_Create(&catfork);
}

#else

// Python 2 module initialization
PyMODINIT_FUNC initcatfork(void)
{
    Py_InitModule("catfork", catfork_methods);
}

#endif /* IS_PY3K */
