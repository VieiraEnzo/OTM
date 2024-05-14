#include "./tools/file.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <utility>
using namespace std;


int main(){
    string name = "./dckp_instances/500/dckp_1_sum.txt";
    int nI, nP, C;
    vector<int> profits, wheights;
    vector<pair<int,int>> ForfeitPairs;
    read_file(name , nI, nP, C, profits, wheights, ForfeitPairs);

    cout << nI << endl;
}