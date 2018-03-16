#include <Python/Python.h>
#include <iostream>
#include <string>
using namespace std;

void move(PyObject* instance, const char* loc1, const char* loc2){
    PyObject* temp = PyObject_CallMethod(instance, "move", "(ss)", loc1, loc2);
}

void startGame(PyObject* instance){
    PyObject* result = PyObject_CallMethod(instance, "startComputerGame", NULL);
}

int** getBoards(PyObject* instance){
    char* board[8][8];
    PyObject* result = PyObject_CallMethod(instance, "getBoard", NULL);
    for (int i=0; i < 8; i++){
        PyObject* row = PyList_GetItem(result, i);
        for (int j=0; j<8; j++){
            PyObject* col = PyList_GetItem(row, j);
            PyObject* objectsRepresentation = PyObject_Repr(col);
            char* a11 = PyString_AsString(objectsRepresentation);
            board[i][j] = a11;
            cout << board[i][j];
        }
        cout << endl;
    }
}

int main(){
    // Needs to take the queue and mutex as arguments
    setenv("PYTHONPATH",".",1);
    Py_SetProgramName("CHESSBOT");
    Py_Initialize();
    PyObject* module = PyImport_ImportModule("moveMaker");
    PyObject* klass = PyObject_GetAttrString(module, "MoveMaker");
    PyObject* instance = PyInstance_New(klass, NULL, NULL);
    PyObject* result = PyObject_CallMethod(instance, "signin", NULL);
    startGame(instance);
    const char* loc1 = "A2";
    const char* loc2 = "A4";
    move(instance, loc1, loc2);
    getBoards(instance);

    // while(true){
        // if can pop run as normal if not continue and try again
        // input will be "A1 A2:Randomname"
        // Need dictionary of names
        // First check if move is valid
        // Dict1 Name: Move
        // Dict2 move: count
        // Hold Max count and then
        //move(instance, "A2", "A4");

    //}

    Py_Finalize();
    return 0;
}