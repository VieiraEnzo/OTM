#include "./tools/file.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <utility>
#include <bitset>
#include <algorithm>
#include <set>
#define BIT bitset<1000>
using namespace std;

int nI, nP, nC;
vector<item> items;
vector<pair<double,int>> SortedPairs;


struct Mochila
{
    BIT s{0};
    int peso = 0, lucro = 0;
};



//Cria SortedPairs e os organiza do mais valiozo para o menos
void buildSortedPairs(){
    for(int i = 0; i < nI ; i++){
        SortedPairs.push_back({(double)items[i].value/(double)items[i].wheight, i});
    }
    sort(SortedPairs.rbegin(), SortedPairs.rend());
}

//Algoritmo construtivo O(nlog), atualmente sem fila bonitinha
void constructive(int alfa, Mochila &mochila){
    
    for(int i = 0; i < nI; i++){
        int it = SortedPairs[i].second;
        if(mochila.peso + items[it].wheight <= nC && items[it].canChoose(mochila.s)){
            mochila.peso += items[it].wheight;
            mochila.lucro += items[it].value;
            mochila.s[it] = 1;
        }
    }
}

//Checa a Vizinhança e retorna o melhor e muda a solução
void checkNeighbors(Mochila &mochila){

    int melhor = 0;
    pair<int,int> par = {0,0};
    for(int i = 0; i < nI; i++){ //saindo
        if(!mochila.s[i]) continue;
        for(int j = 0; j < nI; j++){ //entrando
            if(mochila.s[j]) continue;
            if(mochila.peso - items[i].wheight + items[j].wheight > nC) continue;
            mochila.s[i] = 0; if(!items[j].canChoose(mochila.s)){mochila.s[i] = 1;continue;}
            mochila.s[i] = 1;
            if(-items[i].value + items[j].value < melhor)continue;
            melhor = -items[i].value + items[j].value;
            par = {i,j};
        }
    }
    mochila.s[par.first] = !mochila.s[par.first]; mochila.s[par.second] = !mochila.s[par.second];
    mochila.lucro += melhor;
}

//Retorna o Máximo local da solução
int localSearch(Mochila &mochila){

    int i = 0;
    int smax = -1, sit = 0;
    while(smax < sit){
        smax = max(sit, smax);
        checkNeighbors(mochila);
        sit = mochila.lucro;
        cout << i++ << ": "<< sit << "\n";
    }

    return smax;
}



int GRASP(double alfa, int maxIt){
    int LucroMax = 0;
    Mochila mochila;
    buildSortedPairs();
    for(int i = 0; i < maxIt; i++){
        constructive(alfa, mochila);
        int sol = localSearch(mochila);
        LucroMax = max(LucroMax, sol);
        printf("Iretation %d: LucroMax = %d\n", i+1, sol);
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
    GRASP(alfa, 1);

}