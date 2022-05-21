#include "SolvingFunctions.h"

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