#include <iostream>
#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif
#include "validatingFunctions.h"
#include "InputFunctions.h"
using namespace std;

int main()
{
    // Declaring and filling a 9x9 grid with zeroes
    vector<vector<int>> grid {9, vector<int>(9)};
    
    // Inserting predefined sudoku grid in grid vector
    // NOTE: any blank spaces in grid must be filled with zeroes
    // and each number must have a space after it (except the ninth which
    // should have a return)
    inputSquare(grid);
    if (checkBox(grid, 1, 0, 0))
        cout << "Works" << endl;
    /*
    // Logic for funsies
    while (!validSudoku(grid))
    {
        // Try to find additional solutions
        for (int num = 1; num <= 9; num++)
        {

        }
        // printGrid(grid);
        sleep(1);
    }*/
    
    return 0;
}