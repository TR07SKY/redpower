/*-
 * Copyright 2018 Cryply team
 * Copyright 2021 redpower team
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * This file was originally written by Cryply team as part of the Cryply
 * coin.
 */

#define PY_SSIZE_T_CLEAN

#include <Python.h>
#include "yespower.h"
#include "sysendian.h"

#include "sha256.h"
#include "sha256.c"
#include "yespower-opt.c"

static const yespower_params_t redpower = {
    .version = YESPOWER_1_0,
    .N = 2048,
    .r = 16,
    .pers = "RED is because communism, workers equality",
    .perslen = 42
};

void yespower_hash(const char *input, char *output)
{
    yespower_tls(input, 80, &redpower, (yespower_binary_t *) output);
}

static PyObject *yespower_getpowhash(PyObject *self, PyObject *args)
{
    char *output;
    PyObject *value;
#if PY_MAJOR_VERSION >= 3
    PyBytesObject *input;
#else
    PyStringObject *input;
#endif
    if (!PyArg_ParseTuple(args, "S", &input))
        return NULL;
    Py_INCREF(input);
    output = PyMem_Malloc(32);

#if PY_MAJOR_VERSION >= 3
    yespower_hash((char *)PyBytes_AsString((PyObject*) input), output);
#else
    yespower_hash((char *)PyString_AsString((PyObject*) input), output);
#endif
    Py_DECREF(input);
#if PY_MAJOR_VERSION >= 3
    value = Py_BuildValue("y#", output, 32);
#else
    value = Py_BuildValue("s#", output, 32);
#endif
    PyMem_Free(output);
    return value;
}

static PyMethodDef YespowerMethods[] = {
    { "hash", yespower_getpowhash, METH_VARARGS, "Returns the proof of work hash using yespower" },
    { NULL, NULL, 0, NULL }
};

#if PY_MAJOR_VERSION >= 3
static struct PyModuleDef YespowerModule = {
    PyModuleDef_HEAD_INIT,
    "yespower_hash",
    "...",
    -1,
    YespowerMethods
};

PyMODINIT_FUNC PyInit_redpower(void) {
    return PyModule_Create(&YespowerModule);
}

#else

PyMODINIT_FUNC initredpower(void) {
    (void) Py_InitModule("redpower", YespowerMethods);
}
#endif
