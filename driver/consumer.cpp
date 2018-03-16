#include <Python/Python.h>
#include <iostream>
#include <string>
#include <math.h>
using namespace std;

void move(PyObject* instance, string loc1, string loc2){
    PyObject* temp = PyObject_CallMethod(instance, "move", "(ss)", loc1.c_str(), loc2.c_str());
}

void startGame(PyObject* instance){
    PyObject* result = PyObject_CallMethod(instance, "startComputerGame", NULL);
}

void getBoards(PyObject* instance, string board[8][8]){
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

bool isValid(string board[8][8], string moveFrom, string moveTo){
    cout << "starting valid" << endl;
    string boardInd= "ABCDEFGH";
    int moveFromCol = boardInd.find(toupper(moveFrom.at(0)));
    int moveFromRow = 8 - stoi(moveFrom.substr(1, 1));
    int moveToCol = boardInd.find(toupper(moveTo.at(0)));
    int moveToRow = 8 - stoi(moveTo.substr(1, 1));
    cout << moveFromRow << " " << moveFromCol << endl;
    cout << moveToRow << " " << moveToCol << endl;

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
        cout << "Pawn" << endl;
        if (moveToRow == moveFromRow - 1 || moveToRow == moveFromRow - 2 && info2 == "None"){
            // If the given row is one above and open

            //cout << "Pawn True" << endl;
            return true;
        } else if (info2.at(1) == 'b' && moveToRow == moveFromRow - 1 && (moveToCol == moveFromCol - 1 || moveToCol == moveFromCol + 1)){
            // If the move is one row above its spot and one col left or right AND there is a black piece there
            //cout << "Pawn True" << endl;
            return true;
        }
    }
    else if(piece == 'r') {
        // Check if space is occupied by a piece of the same color (white)
        // Check if move is in same col or same row then check each space up to that space given to move to.
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
            }
        }
    }

    else if(piece == 'n') {
        // Check that space given is:
        // row - 2, col + 1
        // row - 2, col - 1
        // row + 2, col + 1
        // row + 2, col - 1
        // row + 1, col + 2
        // row - 1, col + 2
        // row + 1, col - 2
        // row - 1, cold -2
        cout << "Knight" << endl;
    }
    else if(piece == 'b') {
        // difference of rows and cols must be the same
        cout << "Bishop" << endl;
    }
    else if(piece == 'q') {
        cout << "Queen" << endl;
    }
    else if(piece == 'k') {
        cout << "King" << endl;
    }
    cout << "False" << endl;
    return false;
}
