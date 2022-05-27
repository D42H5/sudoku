#include "InputFunctions.h"

// Fills in sudoku array
void inputSquare(std::vector<std::vector<int>> &vec)
{
    std::ifstream infile;
    infile.open("altGrid.txt");

    int temp,
        row {0},
        col {0};

    while (infile >> temp)
    {
        vec[row][col] = temp;
        col++;
        if (col == 9)
        {
            col = 0;
            row++;
        }
    }
}