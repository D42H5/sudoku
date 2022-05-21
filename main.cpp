#include "InputFunctions.h"
#include "Functions.h"
#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif
#include <iostream>
#include <vector>

using namespace std;

void printGrid(vector<vector<int>> &grid);

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
    do
    {
        // Try to find solutions for each number
        for (int num = 1; num <= 9; num++)
        {
            // Loop through each 3x3 box in sudoku grid
            for (int rowStart = 0; rowStart < 9; rowStart += 3)
            {
                for (int colStart = 0; colStart < 9; colStart += 3)
                {
                    // Making vector to store coordinates of solutions
                    vector<vector<int>> coords;

                    // Check current box and see if num is not in it
                    if (checkBox(grid, num, rowStart, colStart))
                    {
                        // If num is not in current box, look for solutions
                        findSpots(grid, coords, num, rowStart, colStart);

                        // If coords is empty, no spots were found or too many were found
                        // So just continue
                        if (coords.empty())
                            { continue; }
                        
                        // Else solution found! So update sudoku grid
                        else
                            { grid[ coords[0][0] ][ coords[0][1] ] = num; }
                    }

                    // If num found in current 3x3 box, continue to next box
                    else
                        { continue; }
                }
            }
        } 
        printGrid(grid);
        sleep(1);
    } while (!validSudoku(grid));
    
    return 0;
}

// Function to print sudoku grid
void printGrid(vector<vector<int>> &grid)
{
    for (int row = 0; row < 9; row++)
    {
        for (int col = 0; col < 9; col++)
        {
            std::cout << grid[row][col] << " ";
        }
        std::cout << std::endl;
    }
}