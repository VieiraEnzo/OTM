#include "./tools/file.h"
#include "./tools/statistics.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <utility>
#include <bitset>
#include <chrono>
#include <algorithm>
#include <ext/pb_ds/tree_policy.hpp>
#include <ext/pb_ds/assoc_container.hpp>

#define BIT bitset<1000>

typedef long long ll;

using namespace __gnu_pbds;
using ordered_set = tree<pair<double,int>, null_type, less<pair<double,int>>, rb_tree_tag, tree_order_statistics_node_update>;
using ordered_multiset = tree<ll, null_type, less_equal<ll>, rb_tree_tag, tree_order_statistics_node_update>;

using namespace std;

int nI, nP, nC;
vector<item> items;
vector<pair<double,int>> SortedPairs;

struct LCR
{   
    int inf = 1e9;
    double hmin, hmax, alfa;
    ordered_set elementos;
    

    LCR (double alfa) : alfa(alfa){

        for(int i = 0; i < nI; i++) elementos.insert(SortedPairs[i]);
    }

    int get_element(){
        hmin = (*elementos.begin()).first;
        hmax = (*prev(elementos.end())).first;
        int nb = elementos.order_of_key({hmax + alfa * (hmin - hmax), inf});
        int rp = rand() % nb;
        auto it = elementos.find_by_order(rp);
        int IndRem = (*it).second;
        elementos.erase(it);   
        return IndRem;
    }

};


//Cria SortedPairs e os organiza do mais valiozo para o menos
void buildSortedPairs(){
    for(int i = 0; i < nI ; i++){
        SortedPairs.push_back({(double)items[i].wheight/(double)items[i].value, i});
    }
    sort(SortedPairs.begin(), SortedPairs.end());
}

//Algoritmo construtivo O(nlog), atualmente sem fila bonitinha
void constructive(double alfa, Mochila &mochila){
    //Modificar a razão
    LCR lcr(alfa);

    for(int i = 0; i < nI; i++){
        int it = lcr.get_element();
        if(mochila.peso + items[it].wheight <= nC && items[it].Compativel(mochila.s)){
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
            mochila.s[i] = 0; if(!items[j].Compativel(mochila.s)){mochila.s[i] = 1;continue;}
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
    }

    return smax;
}



int GRASP(double alfa, int maxIt){
    int LucroMax = 0;
    SortedPairs.clear();
    buildSortedPairs();
    for(int i = 0; i < maxIt; i++){
        Mochila mochila;
        constructive(alfa, mochila);
        int sol = localSearch(mochila);
        LucroMax = max(LucroMax, sol);
        // printf("Iretation %d: LucroMax = %d\n", i+1, sol);
    }
    
    return LucroMax;
}





int main(){
    string name = "./dckp_instances/500/";
    string instB = "dckp_", instE = "_sum.txt";
    double alfa = 0.75;
    srand (time(NULL));
    

    for(int i = 1; i <= 10; i++){

        string arq_inp = name + instB + to_string(i) + instE; 
        string arq_out = "./results/500/dckp_" + to_string(i) + "_result.txt"; 

        printf("Reading files...\n");
        read_file(arq_inp, nI, nP, nC, items);
        printf("Files read\n");

        printf("Running GRASP\n");

        vector<double> solucoes, tempos;
        for(int i = 0; i < 30; i++){
            auto start = std::chrono::high_resolution_clock::now();
            int sol = GRASP(alfa, 100);
            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> elapsed = end - start;
            double execution_time = elapsed.count();
            solucoes.push_back(sol);
            tempos.push_back(execution_time);
            printf("Iretation %d\n", i);
        }

        write_solutions(solucoes, tempos, arq_out);
        
    }

}