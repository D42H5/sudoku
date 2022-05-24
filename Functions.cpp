#include "Functions.h"
#include <iostream>
#include <iterator>
#include <algorithm>

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
bool appliedLogic(std::vector<std::vector<int>> &grid, std::vector<int> &nums, int rowStart, int colStart)
{
    // Make vector to hold numbers at each empty spot
    std::vector<std::vector<int>> spots;

    // Boolean to see if a change is made
    bool madeChange {false};
    
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
                    if (checkBox(grid, num, rowStart, colStart) && checkRowWithNum(grid, num, row, 0) && checkColWithNum(grid, num, col, 0))
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
            {   
                // std::cout << "Changing grid[" << vec[0] << "][" << vec[1] << "] to " << vec[2] << std::endl;
                grid[ vec[0] ][ vec[1] ] = vec[2]; 
                madeChange = true;
            }
    }
    
    // Return madeChange for checking purposes in main
    return madeChange;
}

// FIXME : make a good comment on this later besides "it's big brain time"
bool advancedLogic(std::vector<std::vector<int>> &grid, std::vector<int> &nums, int row, int col)
{
    // Vector to hold possible solutions in passed spot
    std::vector<int> spot;

    // Make vectors to hold solutions for empty cells in same row and column
    std::vector<std::vector<int>> sameRow = rowHypos(grid, nums, row, col);
    std::vector<std::vector<int>> sameCol = colHypos(grid, nums, row, col);

    // Boolean to see if a change is made
    bool madeChange {false},

    // used to keep track of whether or not a solution in spot vector is nowhere else in row or col hypotheticals
        alone {true};

    // FIXME : DELTE LATER
    std::cout << "Before finding solutions for spot\n";
    // Get possible solutions for current coords
    for (int num : nums)
    {
        if (checkBox(grid, num, findPrevThree(row), findPrevThree(col)) && checkRowWithNum(grid, num, row, 0) && checkColWithNum(grid, num, col, 0))
        {
            spot.push_back( {num} );
        }
    }
    
    // FIXME : DELTE LATER
    std::cout << "Before checking hypotheticals\n";

    // For each num at coords, check if num is in hypothetical sameRow and sameCol
    for (int num : spot)
    {
        // Reset alone boolean for each number
        alone = true;

        // Loop through all hypotheticals in sameRow
        for (auto &vec : sameRow)
        {
            // FIXME : DELTE LATER
            std::cout << "Checking vec in sameRow\n";

            // If more than 2 hypotheticals skip
            if (vec.size() > 2)
                { continue; }

            // Otherwise consider hypotheticals to be answers and check against them
            for (int temp = 0; temp < (int)vec.size() && alone == true; temp++)
            {
                if (vec[temp] == num)
                    { alone = false; }
            }
        }

        // Looping through all hypotheticals in sameCol
        for (auto &vec : sameCol)
        {
            // FIXME : DELTE LATER
            std::cout << "Checking vec in sameCol\n";

            // If more than 2 hypotheticals skip
            if (vec.size() > 2)
                { continue; }
                
            // Start looking after coords
            for (int temp = 0; temp < (int)vec.size() && alone == true; temp++)
            {
                if (vec[temp] == num)
                    { alone = false; }
            }
        }

        // If alone is still true, then solution has been found!
        if (alone)
        {
            std::cout << "Changing grid[" << row << "][" << col << "] to " << num << std::endl;
            grid[row][col] = num;
            madeChange = true;
            break;
        }
    }

    return madeChange;
}

std::vector<std::vector<int>> rowHypos(std::vector<std::vector<int>> &grid, std::vector<int> &nums, int row, int col)
{
    // 2D Vector to return with all the values
    std::vector<std::vector<int>> values;

    // count for adding values to the right place in rowValues 
    int count {0},
    // appear for helping with removal of excess values
        appear {0},
    // first for use in either making new row in rowValues or adding solution to existing row
        first {0};

    // Boolean used to make sure first is handled properly
    bool guide;

    // Loop through column and find solutions in empty spaces
    for (int num : nums)
    {
        // Resetting for new numbers
        appear = 0;
        count = 0;
        guide = true;

        // Loop through each cell in column
        for (int check = 0; check < 9; check++)
        {
            // FIXME : DELTE LATER
            std::cout << "Checking row: " << check << " | num: " << num << std::endl;

            // If num in row already, break out of loop row checking loop
            if (checkRowWithNum(grid, num, row, 1))
            { 
                std::cout << "Num already in row... skipping\n";
                // Updating to handle first properly
                guide = false;
                break; 
            }

            // IF COORDS MATCH PASSED ROW, CONTINUE AND DEAL WITH IT LATER
            else if (check == col)
                { continue; }

            // If space is empty, see if num is a solution
            else if (grid[row][check] == 0)
            {
                // FIXME : DELTE LATER
                std::cout << "Empty box... checking if solution\n";

                if (checkBox(grid, num, findPrevThree(row), findPrevThree(check)) && checkColWithNum(grid, num, check, 0))
                {
                    // FIXME : DELTE LATER
                    std::cout << "Before adding\n";

                    (first == 0) ? values.push_back( {num} ) : values[count++].push_back( {num} );
                    appear++;

                    // FIXME : DELTE LATER
                    std::cout << "Post adding to vec\n";
                }
            }
        }

        // Removing num from rowValues if appears more than twice
        if (appear > 2)
        {
            // FIXME : DELTE LATER
            std::cout << "A WILD NUM APPEARED MORE THAN 2X\n";
            for (auto &vec : values)
            {
                // FIXME : DELTE LATER
                std::cout << "Before erasing\n";

                // Find and erase each instance of num from vectors in sameCol
                std::vector<int>::iterator iter { std::find(vec.begin(), vec.end(), num) };
                if (iter != vec.end())
                    { vec.erase(iter); }

                // FIXME : DELTE LATER
                std::cout << "After erasing\n";
            }
        }
        // If num appears less than twice, increase first to prevent seg faults when using .push_back :)
        else if (guide)
            { first++; }
    }

    // FIXME : DELTE LATER
    std::cout << "Returning to sender...\n";

    return values;
}

std::vector<std::vector<int>> colHypos(std::vector<std::vector<int>> &grid, std::vector<int> &nums, int row, int col)
{
    // 2D Vector to return with all the values
    std::vector<std::vector<int>> values;

    // count for adding values to the right place in rowValues 
    int count {0},
    // appear for helping with removal of excess values
        appear {0},
    // first for use in either making new row in rowValues or adding solution to existing row
        first {0};

    // Loop through column and find solutions in empty spaces
    for (int num : nums)
    {
        // Resetting for new numbers
        appear = 0;
        count = 0;

        // Loop through each cell in column
        for (int check = 0; check < 9; check++)
        {
            // IF COORDS MATCH PASSED ROW, CONTINUE AND DEAL WITH IT LATER
            if (check == row)
                { continue; }

            // If space is empty, see if num is a solution
            else if (grid[check][col] == 0)
            {
                if (checkBox(grid, num, findPrevThree(check), findPrevThree(col)) && checkRowWithNum(grid, num, check, 0) && checkColWithNum(grid, num, col, 0))
                {
                    (first++ == 0) ? values.push_back( {num} ) : values[count++].push_back( {num} );
                    appear++;
                }
            }
        }

        // Removing num from rowValues if appears more than twice
        if (appear > 2)
        {
            for (auto &vec : values)
            {
                // Find and erase each instance of num from vectors in sameCol
                std::vector<int>::iterator iter { std::find(vec.begin(), vec.end(), num) };
                if (iter != vec.end())
                    { vec.erase(iter); }
            }

            // Reset first to 0 if the very first num in nums
            if (first == 1)
                { first = 0; }
        }
    }

    return values;
}

int findPrevThree(int num)
{
    while (num % 3 != 0)
        { --num; }

    return num;
}