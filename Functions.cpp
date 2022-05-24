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
bool findSpots(std::vector<std::vector<int>> &grid, std::vector<std::vector<int>> &coords, int num, int rowStart, int colStart, int n)
{
    // Keep track of number of possible spots in a box
    int count {0};

    // Clearing coords vector for new coordinates
    coords.clear();

    // Looping through 3x3 box and counting number of possible spots
    // Also updating coords vector with coordinates
    // Stopping loop early if more than one possible spot is found
    for (int row = rowStart; row < rowStart + 3 && count <= n; row++)
    {
        for (int col = colStart; col < colStart + 3 && count <= n; col++)
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
    // If more than n solution, clear
    // This allows to come back later when one of possible spots has been taken by another number
    // Return false for logic purposes
    if (count > n || count == 0)
        { coords.clear(); 
          return false; }
    
    // At this point, coords is either empty because more than n solutions or 0 solutions
    // Or it has exactly n solutions, thus allowing a spot to be chosen or hypothetical to be verified

    return true;
} 

// ------------------------------------------------------------------------
// Various forms of bullshit COMMENCE

// Copies all non-zero cells from grid to hypos and changes them to -1
// Simultaneously finds all possible solutions in empty cells
void updateHypos(std::vector<std::vector<int>> &grid, std::vector<std::vector<std::vector<int>>> &hypos, std::vector<int> &nums)
{
    // Loop through grid
    for (int row = 0; row < 9; row++)
    {
        for (int col = 0; col < 9; col++)
        {
            // Clear currect vector for new info
            hypos[row][col].clear();

            // If grid cell is not empty, update hypos correct cell to only have -1
            if (grid[row][col] != 0)
                { hypos[row][col].push_back( {-1} ); }

            // Else grid cell is empty, find all the possible solutions
            else
            { 
                for (int num : nums)
                {
                    // If solution, add to hypos
                    if (checkBox(grid, num, findPrevThree(row), findPrevThree(col))
                                && checkRowWithNum(grid, num, row, 0) && checkColWithNum(grid, num, col, 0))
                    { 
                        if (hypos[row][col].at(0) == 0)
                            { hypos[row][col].at(0) = num; }
                        else
                            { hypos[row][col].push_back( {num} ); }
                    }
                }   
            }
        }
    }
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

// it's bigger brain time

// bool advancedLogic(std::vector<std::vector<int>> &grid, std::vector<int> &nums, int row, int col)
// {
//     // Vector to hold possible solutions in passed spot
//     std::vector<int> spot;

//     // Make vectors to hold solutions for empty cells in same row and column
//     std::vector<std::vector<int>> sameRow = rowHypos(grid, nums, row, col);
//     std::vector<std::vector<int>> sameCol = colHypos(grid, nums, row, col);

//     // Boolean to see if a change is made
//     bool madeChange {false},

//     // used to keep track of whether or not a solution in spot vector is nowhere else in row or col hypotheticals
//         alone {true};

//     // Get possible solutions for current coords
//     for (int num : nums)
//     {
//         if (checkBox(grid, num, findPrevThree(row), findPrevThree(col)) && checkRowWithNum(grid, num, row, 0) && checkColWithNum(grid, num, col, 0))
//         {
//             spot.push_back( {num} );
//         }
//     }

//     // For each num at coords, check if num is in hypothetical sameRow and sameCol
//     for (int num : spot)
//     {
//         // Reset alone boolean for each number
//         alone = true;

//         // Loop through all hypotheticals in sameRow
//         for (auto &vec : sameRow)
//         {
//             // If more than 2 hypotheticals skip
//             if (vec.size() > 2)
//                 { continue; }

//             // Otherwise consider hypotheticals to be answers and check against them
//             for (int temp = 0; temp < (int)vec.size() && alone == true; temp++)
//             {
//                 if (vec[temp] == num)
//                     { alone = false; }
//             }
//         }

//         // Looping through all hypotheticals in sameCol
//         for (auto &vec : sameCol)
//         {
//             // If more than 2 hypotheticals skip
//             if (vec.size() > 2)
//                 { continue; }
                
//             // Start looking after coords
//             for (int temp = 0; temp < (int)vec.size() && alone == true; temp++)
//             {
//                 if (vec[temp] == num)
//                     { alone = false; }
//             }
//         }

//         // If alone is still true, then solution has been found!
//         if (alone)
//         {
//             std::cout << "Changing grid[" << row << "][" << col << "] to " << num << std::endl;
//             grid[row][col] = num;
//             madeChange = true;
//             break;
//         }
//     }

//     return madeChange;
// }

// std::vector<std::vector<int>> rowHypos(std::vector<std::vector<int>> &grid, std::vector<int> &nums, int row, int col)
// {
//     // 2D Vector to return with all the values
//     std::vector<std::vector<int>> values;

//     // Vector to hold coords
//     std::vector<std::vector<int>> coords;

//     // count for adding values to the right place in rowValues 
//     int count {0},
//     // appear for helping with removal of excess values
//         appear {0};

//     // Loop through row initially and find all empty spaces
//     for (int temp = 0; temp < 9; temp++)
//     {
//         if (grid[row][temp] == 0 && temp != col)
//             { values.push_back( {0} ); }
//     }

//     // Loop through column and find solutions in empty spaces
//     for (int num : nums)
//     {
//         // Resetting for new numbers
//         appear = 0;
//         count = -1; // I know this looks weird, but it 
//         // let's me just increment count by 1 without extra steps (for index usage)

//         // Loop through each cell in column
//         for (int check = 0; check < 9; check++)
//         {
//             // If num in row already, break out of loop row checking loop
//             if (checkRowWithNum(grid, num, row, 1))
//                 { break; }

//             // IF COORDS MATCH PASSED ROW, CONTINUE AND DEAL WITH IT LATER
//             else if (check == col)
//                 { continue; }

//             // If space is empty, see if num is a solution
//             else if (grid[row][check] == 0)
//             {
//                 // Update count to keep track of current empty cell
//                 count++;

//                 // If num already in box or more than 2 possible solutions with num, skip
//                 if (!findSpots(grid, coords, num, findPrevThree(row), findPrevThree(check), 2) || !checkBox(grid, num, findPrevThree(row), findPrevThree(check)) )
//                     { continue; }

//                 // If passes column check, is a possible solution b/c passed row, col and box check
//                 else if (checkColWithNum(grid, num, check, 0))
//                 {
//                     // If first element in vector in values is 0, change 0 to num
//                     if (values[count].at(0) == 0)
//                         { values[count].at(0) = num; }

//                     // Else add num to existing vector
//                     else
//                         { values[count].push_back( {num} ); }

//                     // Please don't get rid of appear in the future :)
//                     appear++;
//                 }
//             }
//         }

//         // Removing num from values if appears more than twice or only once
//         if (appear > 2)
//         {
//             for (auto &vec : values)
//             {
//                 // Find and erase each instance of num from vectors in sameCol
//                 std::vector<int>::iterator iter { std::find(vec.begin(), vec.end(), num) };
//                 // If iter points to beginning of vec and size is 1, change to 0
//                 if (iter == vec.begin() && (int)vec.size() == 1)
//                     { vec.at(0) = 0; }
                
//                 // Else if iter doesn't point to the end of vec, erase the element iter points to
//                 else if (iter != vec.end())
//                     { vec.erase(iter); }
//             }
//         }
//     }

//     // FIXME : DELTE LATER
//     std::cout << "values before returning:\n";
//     for (auto &vec : values)
//     {
//         for (int num : vec)
//             std::cout << num << " ";
//         std::cout << "\n";
//     }

//     return values;
// }

// std::vector<std::vector<int>> colHypos(std::vector<std::vector<int>> &grid, std::vector<int> &nums, int row, int col)
// {
//     // 2D Vector to return with all the values
//     std::vector<std::vector<int>> values;

//     // Vector to hold coords
//     std::vector<std::vector<int>> coords;

//     // count for adding values to the right place in rowValues 
//     int count {0},
//     // appear for helping with removal of excess values
//         appear {0};

//     // Loop through row initially and find all empty spaces
//     for (int temp = 0; temp < 9; temp++)
//     {
//         if (grid[temp][col] == 0 && temp != row)
//             { values.push_back( {0} ); }
//     }

//     // Loop through column and find solutions in empty spaces
//     for (int num : nums)
//     {
//         // Resetting for new numbers
//         appear = 0;
//         count = -1; // I know this looks weird, but it 
//         // let's me just increment count by 1 without extra steps (for index usage)

//         // Loop through each cell in column
//         for (int check = 0; check < 9; check++)
//         {
//             // If num in row already, break out of loop row checking loop
//             if (checkColWithNum(grid, num, col, 1))
//                 { break; }

//             // IF COORDS MATCH PASSED ROW, CONTINUE AND DEAL WITH IT LATER
//             else if (check == row)
//                 { continue; }

//             // If space is empty, see if num is a solution
//             else if (grid[check][col] == 0)
//             {
//                 // Update count to keep track of current empty cell
//                 count++;

//                 // If num already in box or more than 2 possible solutions with num, skip
//                 if (!findSpots(grid, coords, num, findPrevThree(check), findPrevThree(col), 2) || !checkBox(grid, num, findPrevThree(check), findPrevThree(col)) )
//                     { continue; }

//                 // If passes column check, is a possible solution b/c passed row, col and box check
//                 else if (checkRowWithNum(grid, num, check, 0))
//                 {
//                     // If first element in vector in values is 0, change 0 to num
//                     if (values[count].at(0) == 0)
//                         { values[count].at(0) = num; }

//                     // Else add num to existing vector
//                     else
//                         { values[count].push_back( {num} ); }

//                     // Please don't get rid of appear in the future :)
//                     appear++;
//                 }
//             }
//         }

//         // Removing num from values if appears more than twice or only once
//         if (appear > 2)
//         {
//             for (auto &vec : values)
//             {
//                 // Find and erase each instance of num from vectors in sameCol
//                 std::vector<int>::iterator iter { std::find(vec.begin(), vec.end(), num) };
//                 // If iter points to beginning of vec and size is 1, change to 0
//                 if (iter == vec.begin() && (int)vec.size() == 1)
//                     { vec.at(0) = 0; }
                
//                 // Else if iter doesn't point to the end of vec, erase the element iter points to
//                 else if (iter != vec.end())
//                     { vec.erase(iter); }
//             }
//         }
//     }

//     // FIXME : DELTE LATER
//     std::cout << "values before returning:\n";
//     for (auto &vec : values)
//     {
//         for (int num : vec)
//             std::cout << num << " ";
//         std::cout << "\n";
//     }

//     return values;
// }

int findPrevThree(int num)
{
    while (num % 3 != 0)
        { --num; }

    return num;
}