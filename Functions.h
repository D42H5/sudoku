#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include <vector>

bool checkRowWithNum(std::vector<std::vector<int>> &, int, int, int n);
bool checkColWithNum(std::vector<std::vector<int>> &, int, int, int n);
bool checkAllRows(std::vector<std::vector<int>> &, int, int);
bool checkAllCols(std::vector<std::vector<int>> &, int, int);
bool validSudoku(std::vector<std::vector<int>> &);

// Solving stuffs
bool checkBox(std::vector<std::vector<int>> &, int, int, int);
bool findSpots(std::vector<std::vector<int>> &, std::vector<std::vector<int>> &, int, int, int, int);
bool appliedLogic(std::vector<std::vector<int>> &, std::vector<int> &, int, int);
bool advancedLogic(std::vector<std::vector<int>> &, std::vector<int> &, int, int);
std::vector<std::vector<int>> rowHypos(std::vector<std::vector<int>> &, std::vector<int> &, int, int);
std::vector<std::vector<int>> colHypos(std::vector<std::vector<int>> &, std::vector<int> &, int, int);
int findPrevThree(int);

#endif