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

    // If num in box, return false
    if (!checkBox(grid, num, rowStart, colStart))
        { return false; }

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
    // FIXME : DELETE LATER
    // std::cout << "Possible solutions for " << num << " are:\n";
    // for (auto &vec : coords)
    //     { std::cout << "[" << vec[0] << "][" << vec[1] << "]\n"; }
    // std::cout << "\n";

    // If count not equal to n, then too many solutions or not enough (and if n == 1, then not enough means no solutions)
    // This allows to come back later when one of possible spots has been taken by another number
    if (count != n)
        { return false; }
    
    // At this point, coords is either empty because more than n solutions or 0 solutions
    // Or it has exactly n solutions, thus allowing a spot to be chosen or hypothetical to be verified
    return true;
} 

// ------------------------------------------------------------------------
// Various forms of bullshit COMMENCE

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
                // FIXME: DELETE LATER
                std::cout << "Found solution for " << vec[2] << " with applied logic\n";

                grid[ vec[0] ][ vec[1] ] = vec[2]; 
                madeChange = true;
            }
    }
    
    // Return madeChange for checking purposes in main
    return madeChange;
}

// it's bigger brain time

bool advancedLogic(std::vector<std::vector<int>> &grid, std::vector<int> &nums, int row, int col)
{
    // Vector to hold possible solutions in passed spot
    std::vector<int> spot;

    // Vector to hold final "solution" <- in quotes because of case of more than 1 solution (false solutions)
    // std::vector<int> solutions;

    // Make vectors to hold solutions for empty cells in same row and column
    std::vector<std::vector<int>> sameRow = rowHypos(grid, nums, row, col);
    std::vector<std::vector<int>> sameCol = colHypos(grid, nums, row, col);

    // For holding unique numbers from row and col hypotheticals (later)
    std::vector<int> uniqueNums;

    // For holding doubles
    std::vector<std::vector<int>> doubles;

    // Boolean to see if a change is made
    bool madeChange {false};

    // Get possible solutions for current coords
    for (int num : nums)
    {
        if (checkBox(grid, num, findPrevThree(row), findPrevThree(col)) && checkRowWithNum(grid, num, row, 0) && checkColWithNum(grid, num, col, 0))
        {
            spot.push_back( {num} );
        }
    }

    // FIXME ?
    // Stop using advancedLogic if there are more than (let's say) 3 possible solutions in a cell
    if ((int)spot.size() > 2)
        {   // FIXME : DELETE LATER
            std::cout << "Returning false... too many solutions\n";
            return false; }

    // FIXME : DELETE LATER
    std::cout << "Possible solutions: ";
    for (int num : spot)
        std::cout << num << " ";
    std::cout << std::endl;

    // Loop through all hypotheticals in sameRow
    for (auto &vec : sameRow)
    {
        // If more than 2 hypotheticals skip
        if (vec.size() > 2)
            { continue; }

        // Add all nums in vec that are not in uniqueNums to uniqueNums
        findUnique(vec, uniqueNums);
    }

    // Looping through all hypotheticals in sameCol
    for (auto &vec : sameCol)
    {
        // If more than 2 hypotheticals skip
        if (vec.size() > 2)
            { continue; }

        // Add all nums in vec that are not in uniqueNums to uniqueNums
        findUnique(vec, uniqueNums);
    }

    // Finalizing uniqueNums vector
    // Add given numbers in row and col
    for (int tempNum = 0; tempNum < 9; tempNum++)
    {
        // Checking row spot
        if (grid[row][tempNum] != 0)
        {
            std::vector<int>::iterator iter { std::find(uniqueNums.begin(), uniqueNums.end(), grid[row][tempNum]) };
            if (iter == uniqueNums.end())
                { uniqueNums.push_back( {grid[row][tempNum]} ); }
        }

        // Checking column spot
        if (grid[tempNum][col] != 0)
        {
            std::vector<int>::iterator iter { std::find(uniqueNums.begin(), uniqueNums.end(), grid[tempNum][col]) };
            if (iter == uniqueNums.end())
                { uniqueNums.push_back( {grid[tempNum][col]} ); }
        }
    }

    // Add any given numbers from current box to uniqueNums that aren't already in it
    for (int tempRow = findPrevThree(row); tempRow < findPrevThree(row) + 3; tempRow++)
    {
        for (int tempCol = findPrevThree(col); tempCol < findPrevThree(col) + 3; tempCol++)
        {
            if (grid[tempRow][tempCol] != 0)
            {
                // If num not in uniqueNums vector, add it
                std::vector<int>::iterator iter { std::find(uniqueNums.begin(), uniqueNums.end(), grid[tempRow][tempCol]) };
                if (iter == uniqueNums.end())
                    { uniqueNums.push_back( {grid[tempRow][tempCol]} ); }
            }
        }
    }

    // Finding doubles numbers in each box in row/col and adding to unique nums if fall in same row/col
    // Doing row boxes first
    // FIXME
    std::cout << "Row box doubles:\n";
    for (int cStart = 0; cStart < 3 && !madeChange; cStart++)
    {
        // Getting new doubles vector to do some checking with... also seeing if any changes were made
        if (findDoubles(grid, nums, doubles, findPrevThree(row), cStart * 3))
            { madeChange = true; 
              break; }

        for (auto &vec : doubles)
        {
            // If double returns a spot in the same row, fill in any new unique numbers
            if (vec[0] == row)
            {
                for (int num : vec)
                {
                    std::vector<int>::iterator iter { std::find(uniqueNums.begin(), uniqueNums.end(), num) };
                    if (iter == uniqueNums.end())
                        { uniqueNums.push_back( {num} ); }
                }
            }
        }
    }

    // Checking doubles in column boxes
    // FIXME
    std::cout << "Col box doubles:\n";
    for (int rStart = 0; rStart < 3 && !madeChange; rStart++)
    {
        if (rStart * 3 == findPrevThree(row))
            { continue; }
            
        if (findDoubles(grid, nums, doubles, rStart * 3, findPrevThree(col)))
            { madeChange = true;
              break; }

        for (auto &vec : doubles)
        {
            // If double returns a spot in the same col, fill in any new unique numbers
            if (vec[1] == col)
            {
                for (int num : vec)
                {
                    std::vector<int>::iterator iter { std::find(uniqueNums.begin(), uniqueNums.end(), num) };
                    if (iter == uniqueNums.end())
                        { uniqueNums.push_back( {num} ); }
                }
            }
        }
    }

    // Removing any solution from uniqueNums if it appears twice in current box
    for (int num : spot)
    {
        // If appears any amount of times other than once in box, try to find / remove from uniqueNums
        if (!findSpots(grid, doubles, num, findPrevThree(row), findPrevThree(col), 1))
        {
            std::vector<int>::iterator iter { std::find(uniqueNums.begin(), uniqueNums.end(), num) };
            if (iter != uniqueNums.end())
                { uniqueNums.erase(iter); }
        }
    }

    // And removing zeros because idk why they are appearing and this is easier lol
    for (int num : uniqueNums)
    {
        if (num == 0)
        {   
            std::vector<int>::iterator iter { std::find(uniqueNums.begin(), uniqueNums.end(), num) };
            uniqueNums.erase(iter); 
        }
    }


    // FIXME : DELETE LATER
    std::cout << "uniqueNums = ";
    for (int num : uniqueNums)
        { std::cout << num << " "; }
    std::cout << std::endl;

    // If size of uniqueNums == 8, then solution is actually there
    if ((int)uniqueNums.size() == 8 && !madeChange)
    {
        for (int num = 1; num <= 9 && !madeChange; num++)
        {
            std::cout << "Trying num = " << num << std::endl;
            // Try to find current num in uniqueNums vector
            std::vector<int>::iterator iter { std::find(uniqueNums.begin(), uniqueNums.end(), num) };
            // If not found (would be uniqueNums.end(), then that is solution)
            if (iter == uniqueNums.end())
                { // FIXME : DELETE LATER
                  std::cout << "Changing grid[" << row << "][" << col << "] to " << num << std::endl;
                  grid[row][col] = num; 
                  madeChange = true; }
        } 
    }

    return madeChange;
}

// Finds any doubles and does some magic with them
bool findDoubles(std::vector<std::vector<int>> &grid, std::vector<int> &nums, std::vector<std::vector<int>> &doubles, int rowStart, int colStart)
{
    // Vectors for doing stuffs
    std::vector<std::vector<int>> tempCoords;

    // Boolean to help keep track of status and if changes are made
    bool inDoubles,
        madeChange {false};

    // Clearing doubles vector for new coordinates
    doubles.clear();

    // Loop through nums and find any numbers that only have 2 solutions
    for (int num : nums)
    {
        // FIXME : DELETE LATER
        // std::cout << "\nChecking num " << num << "\n";

        // If only 2 solutions, add coordinates that are new and push back current num
        if (findSpots(grid, tempCoords, num, rowStart, colStart, 2))
        { 
            for (auto &vec : tempCoords)
            {
                inDoubles = false; 

                for (auto &vec2 : doubles)
                {
                    // If in doubles, change boolean to be true and push back current num
                    if (vec[0] == vec2[0] && vec[1] == vec2[1])
                    { 
                        inDoubles = true; 
                        vec2.push_back( {num} );
                    }
                }

                // If not in doubles, push back new coordinates and num in doubles vector
                if (!inDoubles)
                    { doubles.push_back( {vec[0], vec[1], num} ); }
            }
        }
    }

    // FIXME : maybe might have to add possible removal of numbers in coords that have 3 nums that are hypothetical solutions
    std::vector<int> toRemove;
    for (auto &vec : doubles)
    {
        // If vector size == 3, add num to additional vector for removal later
        if ((int)vec.size() == 3)
            { toRemove.push_back( {vec[2] } ); }
    }

    // Count used to keep track of current vector in doubles
    int count {0};
    for (auto &vec : doubles)
    {
        for (int num : toRemove)
        {
            std::vector<int>::iterator iter { std::find(vec.begin() + 2, vec.end(), num) };
            // If iter returns a positions that is not the end of the vector AND size of vector is 3 (1 solution)
            // Erase vector
            if (iter != vec.end() && (int)vec.size() == 3)
                { doubles.erase(doubles.begin() + count); }
            
            // If iter is not the end of vec and vec size is not 3, just remove number
            else if (iter != vec.end())
                { vec.erase(iter); }
        }

        count++;
    }

    // FIXME : DELETE LATER
    // std::cout << "Doubles coords and nums\n";
    // std::cout << std::endl;
    if (doubles.empty())
        std::cout << "Doubles vector empty\n";
    
    else
    {
        for (auto &vec : doubles)
        {   
            std::cout << "Coords: [" << vec[0] << "][" << vec[1] << "] - nums: ";
            for (int num : vec)
                std::cout << num << " ";
            std::cout << std::endl;
        }
    }

    return madeChange;
}

std::vector<std::vector<int>> rowHypos(std::vector<std::vector<int>> &grid, std::vector<int> &nums, int row, int col)
{
    // 2D Vector to return with all the values
    std::vector<std::vector<int>> values;

    // Vector to hold coords
    std::vector<std::vector<int>> coords;

    // count for adding values to the right place in rowValues 
    int count {0},
    // appear for helping with removal of excess values
        appear {0};

    // Loop through row initially and find all empty spaces
    for (int temp = 0; temp < 9; temp++)
    {
        if (grid[row][temp] == 0 && temp != col)
            { values.push_back( {0} ); }
    }

    // Loop through column and find solutions in empty spaces
    for (int num : nums)
    {
        // Resetting for new numbers
        appear = 0;
        count = -1; // I know this looks weird, but it 
        // let's me just increment count by 1 without extra steps (for index usage)

        // Loop through each cell in column
        for (int check = 0; check < 9; check++)
        {
            // If num in row already, break out of loop row checking loop
            if (checkRowWithNum(grid, num, row, 1))
                { break; }

            // IF COORDS MATCH PASSED ROW, CONTINUE AND DEAL WITH IT LATER
            else if (check == col)
                { continue; }

            // If space is empty, see if num is a solution
            else if (grid[row][check] == 0)
            {
                // Update count to keep track of current empty cell
                count++;

                // If num already in box,skip
                if (!checkBox(grid, num, findPrevThree(row), findPrevThree(check)) )
                    { continue; }

                // If passes column check, is a possible solution b/c passed row, col and box check
                else if (checkColWithNum(grid, num, check, 0))
                {
                    // If first element in vector in values is 0, change 0 to num
                    if (values[count].at(0) == 0)
                        { values[count].at(0) = num; }

                    // Else add num to existing vector
                    else
                        { values[count].push_back( {num} ); }

                    // Please don't get rid of appear in the future :)
                    appear++;
                }
            }
        }

        // Removing num from values if appears more than twice or only once
        if (appear > 2)
        {
            for (auto &vec : values)
            {
                // Find and erase each instance of num from vectors in sameCol
                std::vector<int>::iterator iter { std::find(vec.begin(), vec.end(), num) };
                // If iter points to beginning of vec and size is 1, change to 0
                if (iter == vec.begin() && (int)vec.size() == 1)
                    { vec.at(0) = 0; }
                
                // Else if iter doesn't point to the end of vec, erase the element iter points to
                else if (iter != vec.end())
                    { vec.erase(iter); }
            }
        }
    }

    // FIXME : DELTE LATER
    // std::cout << "returning row values:\n";
    // for (auto &vec : values)
    // {
    //     for (int num : vec)
    //         std::cout << num << " ";
    //     std::cout << "\n";
    // }

    return values;
}

std::vector<std::vector<int>> colHypos(std::vector<std::vector<int>> &grid, std::vector<int> &nums, int row, int col)
{
    // 2D Vector to return with all the values
    std::vector<std::vector<int>> values;

    // Vector to hold coords
    std::vector<std::vector<int>> coords;

    // count for adding values to the right place in rowValues 
    int count {0},
    // appear for helping with removal of excess values
        appear {0};

    // Loop through row initially and find all empty spaces
    for (int temp = 0; temp < 9; temp++)
    {
        if (grid[temp][col] == 0 && temp != row)
            { values.push_back( {0} ); }
    }

    // Loop through column and find solutions in empty spaces
    for (int num : nums)
    {
        // Resetting for new numbers
        appear = 0;
        count = -1; // I know this looks weird, but it 
        // let's me just increment count by 1 without extra steps (for index usage)

        // Loop through each cell in column
        for (int check = 0; check < 9; check++)
        {
            // If num in row already, break out of loop row checking loop
            if (checkColWithNum(grid, num, col, 1))
                { break; }

            // IF COORDS MATCH PASSED ROW, CONTINUE AND DEAL WITH IT LATER
            else if (check == row)
                { continue; }

            // If space is empty, see if num is a solution
            else if (grid[check][col] == 0)
            {
                // Update count to keep track of current empty cell
                count++;

                // If num already in box or more than 2 possible solutions with num, skip
                if (!findSpots(grid, coords, num, findPrevThree(check), findPrevThree(col), 2) || !checkBox(grid, num, findPrevThree(check), findPrevThree(col)) )
                    { continue; }

                // If passes column check, is a possible solution b/c passed row, col and box check
                else if (checkRowWithNum(grid, num, check, 0))
                {
                    // If first element in vector in values is 0, change 0 to num
                    if (values[count].at(0) == 0)
                        { values[count].at(0) = num; }

                    // Else add num to existing vector
                    else
                        { values[count].push_back( {num} ); }

                    // Please don't get rid of appear in the future :)
                    appear++;
                }
            }
        }

        // Removing num from values if appears more than twice or only once
        if (appear > 2)
        {
            for (auto &vec : values)
            {
                // Find and erase each instance of num from vectors in sameCol
                std::vector<int>::iterator iter { std::find(vec.begin(), vec.end(), num) };
                // If iter points to beginning of vec and size is 1, change to 0
                if (iter == vec.begin() && (int)vec.size() == 1)
                    { vec.at(0) = 0; }
                
                // Else if iter doesn't point to the end of vec, erase the element iter points to
                else if (iter != vec.end())
                    { vec.erase(iter); }
            }
        }
    }

    // FIXME : DELTE LATER
    // std::cout << "returning col values:\n";
    // for (auto &vec : values)
    // {
    //     for (int num : vec)
    //         std::cout << num << " ";
    //     std::cout << "\n";
    // }

    return values;
}

int findPrevThree(int num)
{
    while (num % 3 != 0)
        { --num; }

    return num;
}

// Finds all unique numbers in a 1d vector and adds them to newVec
void findUnique(std::vector<int> &orig, std::vector<int> &newVec)
{
    // Loop through vector
    for (int num : orig)
    {
        // If num not in newVec vector, add it
        std::vector<int>::iterator iter { std::find(orig.begin(), orig.end(), num) };
        if (iter == orig.end() && num != 0)
        {
            newVec.push_back( {num} );
        }
    }
}