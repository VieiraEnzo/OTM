#include "./tools/file.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <utility>
#include <bitset>
#include <algorithm>
#include <set>
using namespace std;

int nI, nP, nC;
vector<item> items;
vector<pair<double,int>> SortedPairs;

//Retorna o Lucro total de uma solução
int profit(vector<bool> &sol){
    int ans = 0;
    for(int i = 0; i < nI; i++){
        if(sol[i]){
            ans += items[i].value;
        }
    }
    return ans;
}

//Retorna se existe algum que i é proibido com
bool Forbitten(int i, vector<int> chosen){
    for(int it : chosen){
        if(items[i].forbiten[it]){
            return true;
        }
    }
    return false;
}

//Cria SortedPairs e os organiza do mais valiozo para o menos
void buildSortedPairs(){
    for(int i = 0; i < nI ; i++){
        SortedPairs.push_back({(double)items[i].value/(double)items[i].wheight, i});
    }
    sort(SortedPairs.rbegin(), SortedPairs.rend());
}

//Algoritmo construtivo O(nlog), atualmente sem fila bonitinha
vector<bool> constructive(int alfa){
    int peso = 0;
    vector<int> chosen;
    for(int i = 0; i < nI; i++){
        int it = SortedPairs[i].second;
        if(peso + items[it].wheight <= nC && !Forbitten(it, chosen)){
            peso += items[it].wheight;
            chosen.push_back(it);
        }
    }

    vector<bool> ans(nI, false);
    for(int it : chosen) ans[it] = true;

    return ans;
}

//Checa a Vizinhança e retorna o melhor
int checkNeighbors(){
    for(int i = 0; i < nI; i++){
        
    }
    return 3;
}

//Retorna o Máximo local da solução
int localSearch(vector<bool> &sol){

    int smax = -1, sit = 0;
    while(smax < sit){
        smax = max(sit, smax);
        sit = checkNeighbors();
    }

    return smax;
}



int GRASP(int alfa, int maxIt){
    int LucroMax = 0;
    buildSortedPairs();
    for(int i = 0; i < maxIt; i++){
        vector<bool> s1 = constructive(alfa);
        int sol = localSearch(s1);
        LucroMax = max(LucroMax, sol);
        printf("Iretation %d: LucroMax = %d\n", i+1, LucroMax);
    }
    
    return LucroMax;
}


int main(){
    string name = "./dckp_instances/500/dckp_1_sum.txt";
    double alfa = 0.75;

    printf("Reading files...\n");
    read_file(name , nI, nP, nC, items);
    printf("Files read\n");

    printf("Running GRASP\n");
    GRASP(alfa, 10);

}