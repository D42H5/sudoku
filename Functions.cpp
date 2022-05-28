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

// Makes code more readable... checks cell with checkBox, checkRowWithNum, and checkColWithNum
bool cellCheck(std::vector<std::vector<int>> &grid, int num, int row, int col)
{
     return (checkBox(grid, num, findPrevThree(row), findPrevThree(col)) && checkRowWithNum(grid, num, row, 0) && checkColWithNum(grid, num, col, 0)) ? true : false;
}

// Changes coords vector to have row and col in a box that would be the solution for num
bool findSpots(std::vector<std::vector<int>> &grid, std::vector<std::vector<int>> &coords, std::vector<std::vector<int>> &sameRow, std::vector<std::vector<int>> &sameCol, int num, int rowStart, int colStart, int n)
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
    // Stopping loop early if more than n possible spots are found
    for (int row = rowStart; row < rowStart + 3 && count <= n; row++)
    {
        // FIXME
        // std::cout << "Checking row " << row << std::endl;

        for (int col = colStart; col < colStart + 3 && count <= n; col++)
        {
            // FIXME
            // std::cout << "Checking col " << col << std::endl;

            // If current spot is 0 (i.e. empty) no other num in the same row or col, and sameRow is empty (or sameCol, but if 
            // I pass one with data, I will pass the other with data too), current coordinates are possible solution
            if (grid[row][col] == 0 && checkRowWithNum(grid, num, row, 0) && checkColWithNum(grid, num, col, 0) && sameRow.empty())
            { 
                // Add solution to coords vector
                count++;
                coords.push_back({row, col});
            }

            // Else if current cell is empty, no other num in same row or col, and sameRow is NOT empty... have some fun :)
            else if (grid[row][col] == 0 && checkRowWithNum(grid, num, row, 0) && checkColWithNum(grid, num, col, 0) && !sameRow.empty())
            {
                bool inSame {false};
                // Checking doubles in sameRow and sameCol to see if num is in either of them
                for (auto &vec : sameRow)
                {
                    if (vec[0] == row && vec[2] == num)
                    {
                        // FIXME
                        // std::cout << "Not a solution because found " << num << " in row " << vec[0] << std::endl;
                        inSame = true;
                    }
                }
                for (auto &vec : sameCol)
                {
                    if (vec[1] == col && vec[2] == num)
                    {
                        // FIXME
                        // std::cout << "Not a solution because found " << num << " in col " << vec[1] << std::endl;
                        inSame = true;
                    }
                }

                // If inSame == false, then a possible solution has been found!
                if (!inSame)
                {
                    // FIXME 
                    // std::cout << "Found possible solution at : grid[" << row << "][" << col << "]\n";
                    count++;
                    coords.push_back( {row, col} );
                }
                
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
bool appliedLogic(std::vector<std::vector<int>> &grid, std::vector<std::vector<int>> &rowCoords, std::vector<std::vector<int>> &colCoords, std::vector<int> &nums, int rowStart, int colStart)
{
    // Make vector to hold numbers at each empty spot
    std::vector<std::vector<int>> spots;
    
    // Use count to keep track of what space is currently being checked
    int count {0};

    // Using bool in case more than 1 applied logic solution is found
    bool madeChange {false};

    // Loop through box and at each empty spot, find all nums that fit
    for (int row = rowStart; row < rowStart + 3; row++)
    {
        for (int col = colStart; col < colStart + 3; col++)
        {
            // If not 0 (empty), continue to next spot
            if (grid[row][col] != 0)
                { continue; }
            
            // Else add coords and find all possible solutions for that spot

            // Adding coords to 
            spots.push_back( {row, col} );
            
            // Looping through numbers in nums vector
            for (int num : nums)
            {
                // If num is a solution at current space and both row and col coords are empty, add it to current 
                // space vector in the spots vector
                if (cellCheck(grid, num, row, col) && rowCoords.empty() && colCoords.empty())
                    { spots[count].push_back(num); }
                
                // Else if cellCheck is true and both row and col coords are NOT empty, run some additional checks 
                // This is for use with hypothetical numbers passed in as coords vec
                else if (cellCheck(grid, num, row, col) && !rowCoords.empty() && !colCoords.empty())
                {
                    bool notSolution {false};
                    // If vec[1] == num and vec[0] == row, don't add to spot
                    for (auto &vec : rowCoords)
                    {
                        // If same num and row, AND not in the same 3 columns as current box
                        if (vec[2] == num && vec[0] == row && (vec[1] < findPrevThree(col) && vec[1] > findPrevThree(col) + 2))
                            { notSolution = true; 
                              break; }
                    }

                    // If vec[1] == num and vec[0] == col, don't add to spot
                    for (auto &vec : colCoords)
                    {
                        if (vec[2] == num && vec[1] == col && (vec[0] < findPrevThree(row) && vec[0] > findPrevThree(row) + 2))
                            { notSolution = true; 
                              break; }
                    }

                    // If notSolution == false still, then is a possible solution, so add to spots
                    if (!notSolution)
                        { spots[count].push_back(num); }
                }
            }
            // Up count for tracking the next empty space
            ++count;
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
                // std::cout << "Found solution for " << vec[2] << " with applied logic\n";

                grid[ vec[0] ][ vec[1] ] = vec[2]; 

                // Return true because change was made
                madeChange = true;
            }
    }
    
    // Making it here means no change, so return false;
    return madeChange;
}

// it's bigger brain time

bool advancedLogic(std::vector<std::vector<int>> &grid, std::vector<int> &nums, int row, int col)
{
    // Vector to hold possible solutions in passed spot
    std::vector<int> spot;

    // For holding unique numbers from row and col hypotheticals (later)
    std::vector<int> uniqueNums;

    // For holding unique numbers in doubles vector
    std::vector<int> uniques;

    // For holding double numbers
    std::vector<std::vector<int>> doubles;

    // For holding double numbers that appear in the same row/col as each other
    std::vector<std::vector<int>> sameRow;
    std::vector<std::vector<int>> sameCol;

    // I swear to god I'm finished with adding vectors to this function
    std::vector<std::vector<int>> tempCoords;

    // Returning false if current cell is not empty
    if (grid[row][col] != 0)
        { return false; }

    // Get possible solutions for current coords
    for (int num : nums)
    {
        if (cellCheck(grid, num, row, col))
        {
            spot.push_back( {num} );
        }
    }

    // FIXME ?
    // Stop using advancedLogic if there are more than (let's say) 3 possible solutions in a cell
    if ((int)spot.size() > 3)
        {   // FIXME : DELETE LATER
            // std::cout << "Returning false... too many solutions\n";
            return false; }

    // FIXME : DELETE LATER
    // std::cout << "Possible solutions: ";
    // for (int num : spot)
    //     std::cout << num << " ";
    // std::cout << std::endl;

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
    // std::cout << "\nRow box doubles:\n";
    
    // Using same to keep track of doubles that appear in same row 2x
    int same {0};

    for (int cStart = 0; cStart < 3; cStart++)
    {
        // FIXME : DELETE LATER
        // std::cout << "\nBox: " << cStart << std::endl;

        // Getting new doubles vector to do some checking with... also seeing if any changes were made
        if (findDoubles(grid, nums, doubles, uniques, findPrevThree(row), cStart * 3))
            {   // Change was made if in here, so return true
                return true; }

        // FIXME : DELETE LATER
        //     std::cout << "\nNew doubles vector:\n";
        //     for (auto &vec : doubles)
        //     {
        //         for (int num : vec)
        //             std::cout << num << " ";
        //         std::cout << std::endl;
        //     }

        // // FIXME : DELETE LATER
        // std::cout << "Uniques:\n";
        // for (int num : uniques)
        //     std::cout << num << " ";
        // std::cout << std::endl;

        for (int uniq : uniques)
        {
            int count {0};
            bool eraseTwo {false};

            for (auto &vec : doubles)
            {
                std::vector<int>::iterator iter { std::find(vec.begin() + 2, vec.end(), uniq) };
                // If find unique number and vec[0] == row, up count
                if (iter != vec.end() && vec[0] == row)
                    count++;

                if (iter != vec.end())
                    sameRow.push_back( {vec[0], vec[1], uniq} );
            }

            // Doing stuffs with doubles in same row as each other
            if (sameRow[same][0] == sameRow[same + 1][0])
            {
                // FIXME : DELETE LATER
                // std::cout << "Erasing 1 vector:\n";
                // for (int num : sameRow[same + 1])
                //     std::cout << num << " ";
                // std::cout << std::endl;

                sameRow.erase(sameRow.begin() + same);
                same++;
            }
            
            // If not in same row as each other, delete both
            else
            { 
                //FIXME : DELETE LATER
                // std::cout << "Erasing 2 vectors:\n";
                // for (int num : sameRow[same])
                //     std::cout << num << " ";
                // std::cout << std::endl;
                // for (int num : sameRow[same + 1])
                //     std::cout << num << " ";
                // std::cout << std::endl;

                sameRow.erase(sameRow.begin() + same + 1);
                sameRow.erase(sameRow.begin() + same);
                eraseTwo = true;
            }

            // FIXME : DELETE LATER
            // std::cout << "After deleting from sameRow:\n";
            // for (auto &vec : sameRow)
            // {
            //     for (int num : vec)
            //         std::cout << num << " ";
            //     std::cout << std::endl;
            // }

            // std::cout << "Same = " << same << std::endl;

            // If num in spots (possible solutions), and in same row, subtract from count to not try to add to unique nums
            if (!eraseTwo)
            {
                std::vector<int>::iterator iter { std::find(spot.begin(), spot.end(), uniq) };
                if (iter != spot.end() && sameRow[same - 1][0] == row)
                    count--;
            }
            
            // If count == 2, try to add to uniqueNums
            if (count == 2)
            {
                std::vector<int>::iterator iter { std::find(uniqueNums.begin(), uniqueNums.end(), uniq) };
                if (iter == uniqueNums.end())
                    uniqueNums.push_back( {uniq} );
            }
        }
    }

    // Checking doubles in column boxes
    // FIXME
    // std::cout << "\n\nCol box doubles:\n";

    // Reset same for reuse
    same = 0;
    for (int rStart = 0; rStart < 3; rStart++)
    {
        // FIXME : DELETE LATER
        // std::cout << "\nBox: " << rStart << std::endl;

        if (findDoubles(grid, nums, doubles, uniques, rStart * 3, findPrevThree(col)))
            {   // Change was made if in here, so return true
                return true; }

        // FIXME : DELETE LATER
        //     std::cout << "\nNew doubles vector:\n";
        //     for (auto &vec : doubles)
        //     {
        //         for (int num : vec)
        //             std::cout << num << " ";
        //         std::cout << std::endl;
        //     }

        // // FIXME : DELETE LATER
        // std::cout << "Uniques:\n";
        // for (int num : uniques)
        //     std::cout << num << " ";
        // std::cout << std::endl;

        // Loop through unique numbers from doubles and see if they are in same col 2x
        for (int uniq : uniques)
        {
            int count {0};
            bool eraseTwo {false};

            for (auto &vec : doubles)
            {
                std::vector<int>::iterator iter { std::find(vec.begin() + 2, vec.end(), uniq) };
                // If find unique number and vec[1] == col, up count
                if (iter != vec.end() && vec[1] == col)
                    count++;
                
                if (iter != vec.end())
                    sameCol.push_back( {vec[0], vec[1], uniq} );
            }

            // Doing stuffs with doubles in same col as each other
            if (sameCol[same][1] == sameCol[same + 1][1])
            {
                // FIXME : DELETE LATER
                // std::cout << "Erasing 1 vector:\n";
                // for (int num : sameCol[same + 1])
                //     std::cout << num << " ";
                // std::cout << std::endl;

                sameCol.erase(sameCol.begin() + same);
                same++;
            }
            
            // If not in same row as each other, delete both
            else
            { 
                //FIXME : DELETE LATER
                // std::cout << "Erasing 2 vectors:\n";
                // for (int num : sameCol[same])
                //     std::cout << num << " ";
                // std::cout << std::endl;
                // for (int num : sameCol[same + 1])
                //     std::cout << num << " ";
                // std::cout << std::endl;

                sameCol.erase(sameCol.begin() + same + 1);
                sameCol.erase(sameCol.begin() + same);
                eraseTwo = true;
            }

            // FIXME : DELETE LATER
            // std::cout << "\nAfter deleting from sameCol:\n";
            // for (auto &vec : sameCol)
            // {
            //     for (int num : vec)
            //         std::cout << num << " ";
            //     std::cout << std::endl;
            // }

            // std::cout << "Same = " << same << std::endl;

            // If num in spots (possible solutions), and in same col, subtract from count to not try to add to unique nums
            if (!eraseTwo)
            {
                std::vector<int>::iterator iter { std::find(spot.begin(), spot.end(), uniq) };
                if (iter != spot.end() && sameCol[same - 1][1] == col)
                    count--;
            }

            // If count == 2, try to add to uniqueNums
            if (count == 2)
            {
                std::vector<int>::iterator iter { std::find(uniqueNums.begin(), uniqueNums.end(), uniq) };
                if (iter == uniqueNums.end())
                    uniqueNums.push_back( {uniq} );
            }

        }
    }


    // Sorting uniqueNums
    // Removing any double numbers and removing zeros because idk why they are appearing and this is easier lol
    std::sort(uniqueNums.begin(), uniqueNums.end());

    for (int num : uniqueNums)
    {
        std::vector<int>::iterator iter { std::find(uniqueNums.begin(), uniqueNums.end(), num) };

        std::vector<int>::iterator iter2 { std::find(iter + 1, uniqueNums.end(), num) };
        while (iter2 != uniqueNums.end())
            { uniqueNums.erase(iter2);
              iter2 = std::find(iter + 1, uniqueNums.end(), num); }

        if (num == 0)
         { uniqueNums.erase(iter); }
    }
    
    // uniqueNums.erase(std::unique(uniqueNums.begin(), uniqueNums.end()), uniqueNums.end());

    // FIXME : DELETE LATER
    // std::cout << "uniqueNums = ";
    // for (int num : uniqueNums)
    //     { std::cout << num << " "; }
    // std::cout << std::endl;

    // If size of uniqueNums == 8, then solution is actually there
    if ((int)uniqueNums.size() == 8)
    {
        for (int num : spot)
        {
            // std::cout << "Trying num = " << num << std::endl;

            // Try to find current num in uniqueNums vector
            std::vector<int>::iterator iter { std::find(uniqueNums.begin(), uniqueNums.end(), num) };
            // If not found (would be uniqueNums.end(), then that is solution)
            if (iter == uniqueNums.end())
                { // FIXME : DELETE LATER
                //   std::cout << "Changing grid[" << row << "][" << col << "] to " << num << std::endl;

                  grid[row][col] = num;

                  // Change was made so return true
                  return true; }
        } 
    }

    // FIXME : DELETE LATER
    // std::cout << "\nSending sameRow:\n";
    // for (auto &vec : sameRow)
    // {
    //     for (int num : vec)
    //         std::cout << num << " ";
    //     std::cout << std::endl;
    // }
    // std::cout << "\nand sameCol:\n";
    // for (auto &vec : sameCol)
    // {
    //     for (int num : vec)
    //         std::cout << num << " ";
    //     std::cout << std::endl;
    // }
    // std::cout << "To normal logic then applied logic" << std::endl << std::endl;

    // In case more than 1 spot :)
    bool madeChange {false};
    for (int num : nums)
    {
        // std::cout << "In normal logic in advancedLogic\n";

        if (findSpots(grid, tempCoords, sameRow, sameCol, num, findPrevThree(row), findPrevThree(col), 1))
        {
            // std::cout << "HOLY YES PLEASE! Changing grid[" << row << "][" << col << "] to " << num << std::endl;
            grid[ tempCoords[0][0] ][ tempCoords[0][1] ] = num;
            madeChange = true;
        }
    }

    // Recreating appliedLogic to be able to incorporate sameRow and sameCol numbers as a Hail Mary before ending advancedLogic
    if (!madeChange && appliedLogic(grid, sameRow, sameCol, nums, findPrevThree(row), findPrevThree(col)))
        { return true; }

    // Making it here means no change, so return false;
    return madeChange;
}

// Finds any doubles and does some magic with them
bool findDoubles(std::vector<std::vector<int>> &grid, std::vector<int> &nums, std::vector<std::vector<int>> &doubles, std::vector<int> &uniques, int rowStart, int colStart)
{
    // Vectors for doing stuffs
    std::vector<std::vector<int>> tempCoords;

    // Dummy coords to be able to use findSpots
    std::vector<std::vector<int>> v1;
    std::vector<std::vector<int>> v2;

    // Boolean to help keep track of status and if changes are made
    bool inDoubles;

    // Clearing doubles and uniques vectors for new data
    doubles.clear();
    uniques.clear();

    // Loop through nums and find any numbers that only have 2 solutions
    for (int num : nums)
    {
        // FIXME : DELETE LATER
        // std::cout << "\nChecking num " << num << "\n";

        // If already in box, skip to next number
        if (!checkBox(grid, num, rowStart, colStart))
            { continue; }

        // If only 2 solutions, add coordinates that are new and push back current num
        if (findSpots(grid, tempCoords, v1, v2, num, rowStart, colStart, 2))
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

    // Vector to hold all unique numbers in doubles vector (that aren't coords)
    for (auto &vec : doubles)
    {
        // num = 2 to start at first non-coordinate number
        for (int num = 2; num < (int)vec.size(); num++)
        {
            std::vector<int>::iterator iter { std::find(uniques.begin(), uniques.end(), vec[num]) };
            // If iter == unique.end(), num not in unique so add
            if (iter == uniques.end())
                { uniques.push_back( vec[num] ); }
        }
    }

    // FIXME : DELTE LATER
    // std::cout << "values before returning:\n";
    // for (auto &vec : values)
    // {
    //     for (int num : vec)
    //         std::cout << num << " ";
    //     std::cout << "\n";
    // }

    // FIXME : DELETE LATER
    // std::cout << "Doubles coords and nums\n";
    // std::cout << std::endl;
    // if (doubles.empty())
    //     std::cout << "Doubles vector empty\n";
        
    // else
    // {
    //     for (auto &vec : doubles)
    //     {   
    //         std::cout << "Coords: [" << vec[0] << "][" << vec[1] << "] - nums: ";
    //         for (int num : vec)
    //             std::cout << num << " ";
    //         std::cout << std::endl;
    //     }
    // }

    // Makes it here, no changes made, return false
    return false;
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