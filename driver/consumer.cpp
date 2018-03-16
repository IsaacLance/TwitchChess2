#include <Python/Python.h>
#include <iostream>
#include <string>
#include <math.h>
#include <stdlib.h>
#include "consumer.hpp"

using namespace std;

void move(PyObject* instance, string coor){
    string loc1 = coor.substr(0, 2);
    string loc2 = coor.substr(3, 2);
    PyObject* temp = PyObject_CallMethod(instance, "move", "(ss)", loc1.c_str(), loc2.c_str());
}

void startGame(PyObject* instance){
    PyObject* result = PyObject_CallMethod(instance, "startComputerGame", NULL);
}

void updateBoard(PyObject* instance, string board[8][8]){
    PyObject* result = PyObject_CallMethod(instance, "getBoard", NULL);
    for (int i=0; i < 8; i++){
        PyObject* row = PyList_GetItem(result, i);
        for (int j=0; j<8; j++){
            PyObject* col = PyList_GetItem(row, j);
            PyObject* objectsRepresentation = PyObject_Repr(col);
            string a11 = PyString_AsString(objectsRepresentation);
            if (a11 != "None"){
                 board[i][j] = a11.substr(2, 2);
            } else{
                board[i][j] = a11;
            }
        }
    }
}

void start(){
    setenv("PYTHONPATH",".",1);
    Py_SetProgramName("CHESSBOT");
    Py_Initialize();
}

void kill(){
    Py_Finalize();
}


bool checkP(string board[8][8], int moveFromCol, int moveFromRow, int moveToCol, int moveToRow, string info2){
    cout << "Pawn" << endl;
    if (moveToRow == moveFromRow - 1 || (moveToRow == moveFromRow - 2 && moveFromRow == 6) && info2 == "None"){
        // If the given row is one above and open

        //cout << "Pawn True" << endl;
        return true;
    } else if (info2.at(1) == 'b' && moveToRow == moveFromRow - 1 && (moveToCol == moveFromCol - 1 || moveToCol == moveFromCol + 1)){
        // If the move is one row above its spot and one col left or right AND there is a black piece there
        //cout << "Pawn True" << endl;
        return true;
    }
    return false;
}

bool checkR(string board[8][8], int moveFromCol, int moveFromRow, int moveToCol, int moveToRow){
    cout << "rook" << endl;
    int change;
    if (moveToRow == moveFromRow || moveToCol == moveFromCol) {
        if (moveFromRow > moveToRow || moveFromCol > moveToCol){
            change = -1;
        } else{
            change = 1;
        } if (moveToRow == moveFromRow){
            for (int i = moveFromCol + change; i < moveToCol; i+=change){
                if (board[moveToRow][i] != "None"){
                    return false;
                }
            }
        } else {
            for (int i = moveFromRow + change; i < moveToRow; i+=change){
                if (board[i][moveToCol] != "None"){
                    return false;
                }
            }
            return true;
        }
    }
}

bool checkN(string board[8][8], int moveFromCol, int moveFromRow, int moveToCol, int moveToRow){
    cout << "Knight" << endl;
    if(moveToRow - 2 == moveFromRow  || moveToCol - 2 == moveFromCol || moveToRow + 2 == moveFromRow || moveToCol + 2 == moveFromCol){
        if(moveToRow - 1 == moveFromRow || moveToCol - 1 == moveFromCol|| moveToRow + 1 == moveFromRow || moveToCol + 1 == moveFromCol){
            cout << "True" << endl;
            return true;
        }
    }
}

bool checkB(string board[8][8], int moveFromCol, int moveFromRow, int moveToCol, int moveToRow){
    // difference of rows and cols must be the same
    cout << "Bishop" << endl;
    int dif1 = moveFromRow - moveToRow;
    int dif2 = moveFromCol - moveToCol;
    int change1;
    int change2;
    if(dif1 > 0 && dif2 > 0){
        // This means that the bishop is moving down to the right
        change1 = 1;
        change2 = 1;
    }
    else if(dif1 < 0 && dif2 < 0){
        // this means the bishop is moving up to the left
        change1 = -1;
        change2 = -1;

    }
    else if(dif1 > 0 && dif2 < 0){
        // this means that the bishop is moving down to the left
        change1 = +1;
        change2 = -1;
    }
    else if(dif1 < 0 && dif2 > 0){
        // Bishop is moving up to the right
        change1 = -1;
        change2 = +1;
    }
    for (int i = 0; i < abs(dif1); i++){
        if (board[moveFromRow+=change1][moveFromCol+=change2] == "None"){
            cout << "FALSE" << endl;
            return false;
        }
    }
    cout << "TRUE" << endl;
    return true;
}

bool checkK(int moveFromCol, int moveFromRow, int moveToCol, int moveToRow, string info2){
    if(abs(moveFromRow - moveToRow) == 1 || abs(moveFromCol - moveToCol) == 1){
        return info2.at(0) == 'b' || info2 == "None";
    }
}

bool isValid(string board[8][8], string coor){
    string boardInd= "ABCDEFGH";
    string moveFrom = coor.substr(0, 2);
    string moveTo = coor.substr(3, 2);
    int moveFromCol = boardInd.find(toupper(moveFrom.at(0)));
    int moveFromRow = 8 - stoi(moveFrom.substr(1, 1));
    int moveToCol = boardInd.find(toupper(moveTo.at(0)));
    int moveToRow = 8 - stoi(moveTo.substr(1, 1));
    cout << moveFromRow << " " << moveFromCol << endl;
    cout << moveToRow << " " << moveToCol << endl;
    for (int i =0; i < 8; i++){
        for (int j = 0; j < 8; j++){
            cout << board[i][j] << " ";
        }
        cout << endl;
    }

    // this is a string of our potential piece "wp"
    string info1 = board[moveFromRow][moveFromCol];
    // this is a string of the spot were going to "bp"
    string info2 = board[moveToRow][moveToCol];
    cout << info1 << " " << info2 << endl;

    if (info1 == "None" || info2.at(0) == 'w' || moveFrom == moveTo){
        // Check that there is a piece at moveFrom and that a friendly piece is not on the square
        cout << "FALSE" << endl;
        return false;
    }

    char piece = info1.at(1);

    if(piece == 'p') {
        // Check if move is row - 1, col or row -1, col +1
        return checkP(board, moveFromCol, moveFromRow, moveToCol, moveToRow, info2);
    }
    else if(piece == 'r') {
        // Check if space is occupied by a piece of the same color (white)
        // Check if move is in same col or same row then check each space up to that space given to move to.
        return checkR(board, moveFromCol, moveFromRow, moveToCol, moveToRow);
    }

    else if(piece == 'n') {
        return checkN(board, moveFromCol, moveFromRow, moveToCol, moveToRow);
    }
    else if(piece == 'b') {
        return checkB(board, moveFromCol, moveFromRow, moveToCol, moveToRow);
    }
    else if(piece == 'q') {
        cout << "Queen" << endl;
        return checkR(board, moveFromCol, moveFromRow, moveToCol, moveToRow) || checkB(board, moveFromCol, moveFromRow, moveToCol, moveToRow);
    }
    else if(piece == 'k') {
        cout << "King" << endl;
        return checkK(moveFromCol, moveFromRow, moveToCol, moveToRow, info2);
    }
}

string getPiece(string board[8][8], string loc){
    string boardInd= "ABCDEFGH";
    int col = boardInd.find(toupper(loc.at(0)));
    int row = 8 - stoi(loc.substr(1, 1));
    return board[row][col];

}
