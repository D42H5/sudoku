#include "Functions.h"
#include <iostream>

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


// Functions for solving stuffs

// Function to see if a 3x3 box doesn't have num
bool checkBox(std::vector<std::vector<int>> &grid, int num, int rowStart, int colStart)
{
    // Keep track of number of times num appears
    int count {0};

    // Looping through 3x3 box and counting number of times num appears
    for (int row = rowStart; row < rowStart + 3; row++)
    {
        for (int col = colStart; col < colStart + 3; col++)
        {
            if (grid[row][col] == num)
                count++;
        }
    }

    // Return true if no instance of num is found (good to continue with trying to find a solution)
    return (count == 0) ? true : false;
}

// Changes coords vector to have row and col in a box that would be the solution for num
void findSpots(std::vector<std::vector<int>> &grid, std::vector<std::vector<int>> &coords, int num, int rowStart, int colStart)
{
    // Keep track of number of possible spots in a box
    int count {0};

    // Clearing coords vector for new coordinates
    coords.clear();

    // Looping through 3x3 box and counting number of possible spots
    // Also updating coords vector with coordinates
    // Stopping loop early if more than one possible spot is found
    for (int row = rowStart; row < rowStart + 3 && count <= 1; row++)
    {
        for (int col = colStart; col < colStart + 3 && count <= 1; col++)
        {
            // If current spot is 0 (i.e. empty) and no other num in the same row or col, current coordinates are possible solution
            if (grid[row][col] == 0 && checkRowWithNum(grid, num, row, 0) && checkColWithNum(grid, num, col, 0))
            { 
                // Add solution to coords vector
                count++;
                coords.push_back({row, col});
            }
        }
    }

    // If more than 1 solution, clear
    // This allows to come back later when one of possible spots has been taken by another number
    if (count > 1)
        { coords.clear(); }
    
    // At this point, coords is either empty because more than 1 solution or 0 solutions
    // Or it has exactly 1 solution, thus being the spot to put down num
} 

// Function to find all empty spots in a box, store possible solutions in new vector,
// and change empty spot to certain num if that num is the only solution in that spot
void appliedLogic(std::vector<std::vector<int>> &grid, std::vector<int> &nums, int rowStart, int colStart)
{
    // Make vector to hold numbers at each empty spot
    std::vector<std::vector<int>> spots;
    
    // Use count to keep track of what space is currently being checked
    int count {0};

    // Loop through box and at each empty spot, find all nums that fit
    for (int row = rowStart; row < rowStart + 3; row++)
    {
        for (int col = colStart; col < colStart + 3; col++)
        {
            // If not 0 (empty), continue to next spot
            if (grid[row][col] != 0)
                { continue; }
            
            // Else add coords and find all possible solutions for that spot
            else
            {
                // Adding coords to 
                spots.push_back( {row, col} );

                // Looping through numbers in nums vector
                for (int num : nums)
                {
                    // If num is a solution at current space, add it to current 
                    // space vector in the spots vector
                    if (checkRowWithNum(grid, num, row, 0) && checkColWithNum(grid, num, col, 0))
                        { spots[count].push_back(num); }
                }

                // Up count for tracking the next empty space
                ++count;
            }
        }
    }
    // Check if one of the empty spots only has 1 number that works
    // i.e., empty space vector has size of 3 (2 for coords and 1 for solution)
    for (auto &vec : spots)
    {
        // Change that spot to that number if so
        if (vec.size() == 3)
            { grid[ vec[0] ][ vec[1] ] = vec[2]; }
    }
    // End function
}

