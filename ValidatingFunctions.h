#ifndef SOLVING_FUNCTIONS_H
#define SOLVING_FUNCTIONS_H
#include <vector>

bool checkRowWithNum(std::vector<std::vector<int>> &, int, int, int n);
bool checkColWithNum(std::vector<std::vector<int>> &, int, int, int n);
bool checkAllRows(std::vector<std::vector<int>> &, int, int);
bool checkAllCols(std::vector<std::vector<int>> &, int, int);
bool validSudoku(std::vector<std::vector<int>> &);
void printGrid(std::vector<std::vector<int>> &);
bool checkBox(std::vector<std::vector<int>> &, int, int, int);
#endif