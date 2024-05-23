#ifndef FILE_H
#define FILE_H

#include "item.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <utility>

// Function declaration
void read_file(std::string name, int &nI, int &nP, int &C, 
                std::vector<item> &items) ;

#endif
