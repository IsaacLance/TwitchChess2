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
    if (module == nullptr)
    {
        PyErr_Print();
        exit(1);
    }
    
    PyObject* instance = PyInstance_New(klass, NULL, NULL);
    if (module == nullptr)
    {
        PyErr_Print();
        exit(1);
    }
    
    //PyObject* result = PyObject_CallMethod(instance, "add", "(ii)", 1, 2);
    //assert(result != NULL);
    


    Py_Finalize();
    return 0;
}
