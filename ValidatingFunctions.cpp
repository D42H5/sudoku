#include "ValidatingFunctions.h"
#include <vector>
#include <iostream>
using namespace std;


// Returns true if row passed has number appear exactly n times
// NOTE:: n should either be 1 or 0 for purposes of validity checking (1) or 
// finding solutions (0)
bool checkRowWithNum(std::vector<std::vector<int>> &vec, int num, int row, int n)
{
    int count {0};

    // Looping through passed row in vec
    for (int temp : vec[row])
    {
        // Adding to count if num is found
        if (temp == num)
            count++;
    }

    // Returning true if number only appeared once (count == 1)
    return (count == n) ? true : false;
}


// Returns true if col passed has number appear exactly n times 
bool checkColWithNum(std::vector<std::vector<int>> &vec, int num, int col, int n)
{
    int count {0};

    // Looping through passed col in vec
    for (int row = 0; row < 9; row++)
    {
        // Adding to count if num is found
        if (vec[row][col] == num)
            count++;
    }

    // Returning true if number only appeared once (count == 1)
    return (count == n) ? true : false;
}

// Fun times with recursion x 2 >:]

// Recursive function to check all rows in the sudoku grid
bool checkAllRows(std::vector<std::vector<int>> &grid, int num, int row)
{
    int count {row};

    // If gets past row 8, must be true
    // Base case
    if (count == 9)
        { return true;}
    
    // If current row passes, call next function with incremented row
    if (checkRowWithNum(grid, num, count, 1))
        { return checkAllRows(grid, num, ++count);}

    // Else not valid so num is not correct yet
    else
        { return false;}
}

// Recursive function to check all cols in the sudoku grid
bool checkAllCols(std::vector<std::vector<int>> &grid, int num, int col)
{
    int count {col};

    // If gets past col 8, must be true
    // Base case
    if (count == 9)
        { return true;}
    
    // If current row passes, call next function with incremented row
    if (checkColWithNum(grid, num, count, 1))
        { return checkAllCols(grid, num, ++count);}

    // Else not valid so num is not correct yet
    else
        { return false;}
}

// Checks entire grid to see if valid or not
bool validSudoku(std::vector<std::vector<int>> &grid)
{
    bool tracker {true};

    // Check each number for validity
    // Stop loop if tracker becomes false at some point
    for (int i = 1; i <= 9 && tracker; i++)
    {
        // if either rows or column checks fails, set tracker to false 
        if (!checkAllCols(grid, i, 0) || !checkAllRows(grid, i, 0))
            { tracker = false;}
    }

    return tracker;
}

// Function to print sudoku grid
void printGrid(std::vector<std::vector<int>> &grid)
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

