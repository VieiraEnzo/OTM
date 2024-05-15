#include "./tools/file.h"
#include "./tools/item.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <utility>
#include <bitset>
#include <algorithm>
#include <set>
using namespace std;

// struct item
// {
//     int value;
//     int wheight;
//     set<int> forbiten;
// };


int nI, nP, nC;
vector<item> items;
vector<pair<double,int>> SortedPairs;

//Retorna se existe algum que i é proibido com
bool Forbitten(int i, vector<int> chosen){
    for(int it : chosen){
        if(items[i].forbiten[it]){
            return true;
        }
    }
    return false;
}

vector<bool> constructive(int alfa){
    int peso = 0;
    vector<int> chosen;
    for(int i = 0; i < nI; i++){
        int it = SortedPairs[i].second;
        if(peso < nC && !Forbitten(it, chosen)){
            peso += items[it].wheight;
            chosen.push_back(it);
        }
    }

    vector<bool> ans(nI, false);
    for(int it : chosen) ans[it] = true;

    return ans;
}

int localSearch(vector<bool> &s1){
    return 1;
}

//Cria SortedPairs e os organiza do mais valiozo para o menos
void buildSortedPairs(){
    for(int i = 0; i < nI ; i++){
        SortedPairs.push_back({(double)items[i].value/(double)items[i].wheight, i});
    }
    sort(SortedPairs.rbegin(), SortedPairs.rend());
}

int GRASP(int alfa, int maxIt){
    int LucroMax = 0;
    buildSortedPairs();

    for(int i = 0; i < maxIt; i++){
        vector<bool> s1 = constructive(alfa);
        int sol = localSearch(s1);
        LucroMax = max(LucroMax, sol);
    }
    
    return LucroMax;
}


int main(){
    string name = "./dckp_instances/500/dckp_1_sum.txt";
    double alfa = 0.75;

    read_file(name , nI, nP, C, items);
    GRASP(alfa, 100);

}