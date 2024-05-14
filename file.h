#ifndef FILE_H
#define FILE_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <utility>

// Function declaration
void read_file(std::string name, int &nI, int &nP, int &C, 
               std::vector<int> &profits, std::vector<int> &wheights,
               std::vector<std::pair<int,int>> &ForfeitPairs);

#endif
