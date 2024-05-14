#include "file.h"

void read_file(std::string name, int &nI, int &nP, int &C, 
               std::vector<int> &profits, std::vector<int> &wheights,
               std::vector<std::pair<int,int>> &ForfeitPairs) {
    // Your function implementation here
    // Opening file
    std::ifstream myfile;
    myfile.open(name);

    if (!myfile.is_open()) {
        std::cerr << "Error: Unable to open file " << name << std::endl;
        return;
    }

    myfile >> nI >> nP >> C;
    profits.resize(nI);
    wheights.resize(nI);
    ForfeitPairs.resize(nP);

    for(int i = 0; i < nI; i++) myfile >> profits[i];
    for(int i = 0; i < nI; i++) myfile >> wheights[i];
    
    for(int i = 0; i < nP; i++){
        int a,b; myfile >> a >> b;
        ForfeitPairs[i] = {a,b};
    }

    myfile.close();
}
