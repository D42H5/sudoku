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

    // Logic for funsies
    do
    {
        // Clearing any numbers from the toRemove vector
        toRemove.clear();

        // Resetting the changes made tracker
        madeChange = false;

        // Try to find solutions for each number
        for (int num : toCheck)
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
                        findSpots(grid, coords, num, rowStart, colStart, 1);

                        // If coords is empty, no spots were found or too many were found
                        // So just continue
                        if (coords.empty())
                            { continue; }

                        // Else solution found! So update sudoku grid
                        else
                        {
                            // FIXME : DELETE LATER
                            cout << "Found solution for " << num << " with regular logic\n";
                            grid[ coords[0][0] ][ coords[0][1] ] = num; 
                            madeChange = true;
                        }
                    }
                }
            }
        } 

        // If no changes have been made, use some additional logic
        if (!madeChange)
        {
            // Loop through each 3x3 box in sudoku grid
            for (int rowStart = 0; rowStart < 9; rowStart += 3)
            {
                for (int colStart = 0; colStart < 9; colStart += 3)
                {
                    // Apply logic lol
                    if (appliedLogic(grid, toCheck, rowStart, colStart))
                        { madeChange = true; }
                }
            }
        }


        // If no changes still, use ADVANCED SCIENCE AND TECHNOLOGY
        if (!madeChange)
        {
            // FIXME : remove cout later
            cout << endl << endl << "Applying ADVANCED MATH AND SCIENCE\n";

            // Loop through every possible cell in the damn grid
            for (int row = 0; row < 9 && !madeChange; row++)
            {
                for (int col = 0; col < 9 && !madeChange; col++)
                {
                    // BEHOLD
                    // Making sure to only pass in empty cells
                    if (grid[row][col] == 0)
                    {
                        cout << endl << "Passing grid[" << row << "][" << col << "] to advancedLogic\n";
                        // If, using super-painfully made logic, a change is made, update madeChange
                        if( advancedLogic(grid, toCheck, row, col) )
                            {   // FIXME : DELETE LATER
                                cout << "Found solution for " << grid[row][col] << " with advanced logic\n";    
                                madeChange = true; }
                    }
                }
            }
        }

        for (int num : toCheck)
        {
            // After looping through entire sudoku grid, check to see if current num is completely finished
            // If num is finished, add it to the toRemove vector
            if (checkAllCols(grid, num, 0) && checkAllRows(grid, num, 0))
                { toRemove.push_back(num); }
        }

        // Remove any numbers from toCheck that are in the toRemove vector
        for (int temp : toRemove)
            { toCheck.erase( find(toCheck.begin(), toCheck.end(), temp) ); }

        // FIXME : remove later, need for printing atm
        cout << "Next Sudoku Grid:" << endl;
        printGrid(grid); 
        cout << endl << endl; 

        // FIXME : DELETE LATER... LET'S ME PAUSE FOR DEBUGGING
        char c;
        cin >> c;


    // Repeat while changes are being made and sudoku grid isn't solved yet
    } while (!validSudoku(grid) && madeChange);
    
    // If exited because no changes were made,
    // Announce that my brain is not big enough to code this yet :/
    if (!madeChange)
    {   cout << "Sorry, this puzzle is too big brain for this code right now...";
        cout << "try coming back later to see if this code can solve it then!" << endl; }
    
    // Else sudoku was able to be solved!
    else
    {
        cout << "Solution:" << endl;
        printGrid(grid);
    }

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