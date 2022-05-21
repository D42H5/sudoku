#include "InputFunctions.h"
#include "Functions.h"
#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif
#include <iostream>
#include <algorithm>

using namespace std;

void printGrid(vector<vector<int>> &grid);

int main()
{
    // Declaring and filling a 9x9 grid with zeroes
    vector<vector<int>> grid {9, vector<int>(9)};

    // Declaring a vector to keep track of the numbers that aren't finished
    vector<int> toCheck {1, 2, 3, 4, 5, 6, 7, 8, 9};

    // Declaring a vector to store the finished numbers to be removed from the toCheck vector 
    vector<int> toRemove;
    
    // Using a bool to control whether or not logic is applied to find new solutions
    bool madeChange {false};


    // Inserting predefined sudoku grid in grid vector
    // NOTE: any blank spaces in grid must be filled with zeroes
    // and each number must have a space after it (except the ninth which
    // should have a line feed)
    inputSquare(grid);

    // FIXME : Remove later... need for printing atm
    int count {0};
    int box {0};

    // Logic for funsies
    do
    {
        // Clearing any numbers from the toRemove vector
        toRemove.clear();

        // Try to find solutions for each number
        for (int num : toCheck)
        {   
            // FIXME : remove later
            box = 0;
            cout << num << endl;


            // Loop through each 3x3 box in sudoku grid
            for (int rowStart = 0; rowStart < 9; rowStart += 3)
            {
                for (int colStart = 0; colStart < 9; colStart += 3)
                {
                    // FIXME : remove later 
                    ++box;


                    // Making vector to store coordinates of solutions
                    vector<vector<int>> coords;

                    // Check current box and see if num is not in it
                    if (checkBox(grid, num, rowStart, colStart))
                    {
                        // FIXME : remove later
                        cout << "I'm checking box " << box << endl;


                        // If num is not in current box, look for solutions
                        findSpots(grid, coords, num, rowStart, colStart);

                        // If coords is empty, no spots were found or too many were found
                        // So just continue
                        if (coords.empty())
                            { continue; }
                        
                        // Else solution found! So update sudoku grid
                        else
                            {
                                // FIXME : remove later
                                cout << "Changing grid[" << coords[0][0] << "][" << coords[0][1] << "] to " << num << endl;
                                grid[ coords[0][0] ][ coords[0][1] ] = num; }
                    }

                    // If num found in current 3x3 box, continue to next box
                    else
                        { continue; }
                }
            }
            // FIXME : remove later
            cout << endl;


            // After looping through entire sudoku grid, check to see if current num is completely finished
            // If num is finished, add it to the toRemove vector
            if (checkAllCols(grid, num, 0) && checkAllRows(grid, num, 0))
                { toRemove.push_back(num); }
        } 

        // Remove any numbers from toCheck that are in the toRemove vector
        for (int temp : toRemove)
            { toCheck.erase( find(toCheck.begin(), toCheck.end(), temp) ); }

        // FIXME : remove later, need for printing atm
        if (++count == 1)
        { 
            cout << "Next Sudoku Grid:" << endl;
            printGrid(grid); 
            cout << endl << endl; 
            count = 0; 
            sleep(2);
        }
    } while (!validSudoku(grid));
    
    cout << "Final Grid:" << endl;
    printGrid(grid);
    return 0;
}

// Function to print sudoku grid
void printGrid(vector<vector<int>> &grid)
{
    for (int row = 0; row < 9; row++)
    {
        for (int col = 0; col < 9; col++)
        {
            cout << grid[row][col] << " ";
            if (col == 2 || col == 5)
                { cout << "| "; }
        }
        cout << std::endl;
        if (row == 2 || row == 5)
            { cout << "---------------------" << endl;}
    }
}