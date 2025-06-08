#include "AtoB.h"
#include <iostream>
#include <thread>
#include <chrono>

const int ROWS = 20;
const int COLS = 40;

using namespace std;

void AtoB::unos()
{
    cout << "Unesi redak i stupac pocetne tocke (A): ";
    cin >> startRow >> startCol;
    cout << "Unesi redak i stupac krajnje tocke (B): ";
    cin >> endRow >> endCol;

    currRow = startRow;
    currCol = startCol;
}

void AtoB::crtaj()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif

    for (int i = 1; i <= ROWS; ++i) {
        for (int j = 1; j <= COLS; ++j) {
            if (i == startRow && j == startCol)
                cout << "A";
            else if (i == endRow && j == endCol)
                cout << "B";
            else if (i == currRow && j == currCol)
                cout << "x";
            else
                cout << ".";
        }
        cout << endl;
    }
}

void AtoB::pokreni()
{
    while (currRow != endRow || currCol != endCol) {
        crtaj();
        this_thread::sleep_for(chrono::milliseconds(100));

        if (currRow < endRow) currRow++;
        else if (currRow > endRow) currRow--;

        else if (currCol < endCol) currCol++;
        else if (currCol > endCol) currCol--;
    }

    crtaj();
    cout << "Stigli smo do cilja!" << endl;
}
