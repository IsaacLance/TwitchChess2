#include <Python/Python.h>
#include <iostream>
#include <string>
using namespace std;

void move(PyObject* instance, string coor);
void startGame(PyObject* instance);
void updateBoard(PyObject* instance, string board[8][8]);
void start();
void kill();
bool checkK(int moveFromCol, int moveFromRow, int moveToCol, int moveToRow, string info2);
bool checkB(string board[8][8], int moveFromCol, int moveFromRow, int moveToCol, int moveToRow);
bool checkN(string board[8][8], int moveFromCol, int moveFromRow, int moveToCol, int moveToRow);
bool checkR(string board[8][8], int moveFromCol, int moveFromRow, int moveToCol, int moveToRow);
bool checkP(string board[8][8], int moveFromCol, int moveFromRow, int moveToCol, int moveToRow, string info2);
bool isValid(string board[8][8], string coor);
string getPiece(string board[8][8], string loc);
