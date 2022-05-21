#include "SolvingFunctions.h"

// Function to see if a 3x3 box doesn't have num
bool checkBox(std::vector<std::vector<int>> &grid, int num, int rowStart, int colStart)
{
    // Keep track of number of times num appears
    int count {0};

    for (int row = rowStart; row < rowStart + 3; row++)
    {
        for (int col = colStart; col < colStart + 3; col++)
        {
            if (grid[row][col] == num)
                count++;
        }
    }

    return (count == 0) ? true : false;
}