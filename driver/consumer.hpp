#include <Python/Python.h>
#include <iostream>
#include <string>
using namespace std;

void move(PyObject* instance, string loc1, string loc2);
void startGame(PyObject* instance);
void getBoards(PyObject* instance, string board[8][8]);
void start();
void kill();
bool isValid(string board[8][8], string moveFrom, string moveTo);
