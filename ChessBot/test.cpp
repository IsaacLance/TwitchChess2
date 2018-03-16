#include <Python/Python.h>
#include <iostream>

using namespace std;
int main(int argc, char *argv[])
{
    setenv("PYTHONPATH",".",1);
    Py_SetProgramName(argv[0]);
    Py_Initialize();

    //PyRun_SimpleString("import moveMaker");
    
    PyObject* module = PyImport_ImportModule("moveMaker");
    if (module == nullptr)
    {
        PyErr_Print();
        exit(1);
    }
    
    PyObject* klass = PyObject_GetAttrString(module, "MoveMaker");
    if (klass == nullptr)
    {
        PyErr_Print();
        exit(1);
    }
    
    PyObject* instance = PyInstance_New(klass, NULL, NULL);
    if (instance == nullptr)
    {
        PyErr_Print();
        exit(1);
    }
    
    PyObject* result = PyObject_CallMethod(instance, "signin", NULL);
    assert(result != NULL);
    if (result == nullptr)
    {
        PyErr_Print();
        exit(1);
    }
    result = PyObject_CallMethod(instance, "startComputerGame", NULL);
    if (result == nullptr)
    {
        PyErr_Print();
        exit(1);
    }
    result = PyObject_CallMethod(instance, "move", "(ss)", "A2", "A4");
    if (result == nullptr)
    {
        PyErr_Print();
        exit(1);
    }



}
