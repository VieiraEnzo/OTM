#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <utility>
#include <bitset>
#include <algorithm>
#include <set>
using namespace std;

struct item
{
    int value;
    int wheight;
    vector<bool> forbiten;

    bool canChoose(vector<bool> sol){
        for(int i = 0; i < sol.size(); i++){
            if(forbiten[i] && sol[i]){
                return false;
            }
        }
        return true;
    }
};
