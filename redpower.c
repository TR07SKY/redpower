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
#include "redpower.h"
#include "sysendian.h"

#include "sha256.h"
#include "sha256.c"
#include "redpower-opt.c"

static const redpower_params_t redpower_1_0_1 = {
    .version = YESPOWER_1_0,
    .N = 2048,
    .r = 16,
    .pers = "RED is because communism, workers equality",
    .perslen = 42
};

void redpower_hash(const char *input, char *output)
{
    redpower_tls(input, 80, &redpower, (redpower_binary_t *) output);
}

static PyObject *redpower_getpowhash(PyObject *self, PyObject *args)
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
    redpower_hash((char *)PyBytes_AsString((PyObject*) input), output);
#else
    redpower_hash((char *)PyString_AsString((PyObject*) input), output);
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

static PyMethodDef RedpowerMethods[] = {
    { "hash", redpower_getpowhash, METH_VARARGS, "Returns the proof of work hash using redpower" },
    { NULL, NULL, 0, NULL }
};

#if PY_MAJOR_VERSION >= 3
static struct PyModuleDef RedpowerModule = {
    PyModuleDef_HEAD_INIT,
    "redpower_hash",
    "...",
    -1,
    RedpowerMethods
};

PyMODINIT_FUNC PyInit_redpower(void) {
    return PyModule_Create(&RedpowerModule);
}

#else

PyMODINIT_FUNC initredpower(void) {
    (void) Py_InitModule("redpower", RedpowerMethods);
}
#endif
