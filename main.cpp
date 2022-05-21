#include "InputFunctions.h"
#include "ValidatingFunctions.h"
#include "SolvingFunctions.h"
#include <iostream>
#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif
using namespace std;

int main()
{
    // Declaring and filling a 9x9 grid with zeroes
    vector<vector<int>> grid {9, vector<int>(9)};
    
    // Inserting predefined sudoku grid in grid vector
    // NOTE: any blank spaces in grid must be filled with zeroes
    // and each number must have a space after it (except the ninth which
    // should have a line feed)
    inputSquare(grid);

    // Logic for funsies
    while (!validSudoku(grid))
    {
        // Try to find solutions for each number
        for (int num = 1; num <= 9; num++)
        {
            // Loop through each 3x3 box in sudoku grid

        }
        // printGrid(grid);
        sleep(1);
    }
    
    return 0;
}