#include "./tools/file.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <utility>
#include <bitset>
#include <algorithm>
#include <set>
#include <ext/pb_ds/tree_policy.hpp>
#include <ext/pb_ds/assoc_container.hpp>

#define BIT bitset<1000>

typedef long long ll;

using namespace __gnu_pbds;
using ordered_set = tree<pair<double,int>, null_type, less<pair<double,int>>, rb_tree_tag, tree_order_statistics_node_update>;


using namespace std;

int nI, nP, nC;
vector<item> items;
set<Mochila> tabuList;

//Retorna se a solução está no Tabu
bool IsTabu(Mochila &mochila, int i, int j){
    bool valid = false;
    mochila.s[i] = 0; mochila.s[j] = 1;
    if(tabuList.find(mochila) != tabuList.end()) valid = true;
    mochila.s[i] = 1; mochila.s[j] = 0;
    return valid;
}

//Checa a Vizinhança e retorna o melhor e muda a solução
void checkNeighbors(Mochila &mochila){

    int melhor = 0;
    if(IsTabu(mochila, 0 ,0)) melhor = -1e9;
    pair<int,int> par = {-1,-1};
    for(int i = 0; i < nI; i++){ //saindo
        if(!mochila.s[i]) continue;
        for(int j = 0; j < nI; j++){ //entrando
            if(mochila.s[j]) continue; 
            if(mochila.peso - items[i].wheight + items[j].wheight > nC) continue; //Verifica se pegar o item estrapola a mochila
            mochila.s[i] = 0; if(!items[j].Compativel(mochila.s)){mochila.s[i] = 1;continue;} //Se item não for compatível
            mochila.s[i] = 1;
            if(-items[i].value + items[j].value < melhor)continue;  //Se a solução Aumenta o valor da mochila
            if(IsTabu(mochila, i , j)) {continue;} //Se estiver na tabuList 
            melhor = -items[i].value + items[j].value; //Atualiza o melhor valor
            par = {i,j};
        }
    }
    if(par.first == -1) return;
    mochila.remove_element(par.first, items[par.first].value, items[par.first].wheight);
    mochila.insert_element(par.second, items[par.second].value, items[par.second].wheight);
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

//Retorna uma solução construtiva do problema
void construtivo(Mochila Mochila){
    //Para cada dupla, testar com base no guloso básico
    //qual tem a melhor mochila. Com isso fixamos esses 2 elementos, e
    //resolvemos para o subconjunto sem eles.
    // Testar primeiro o de um elemento só em vez de dupla.

}

int Tabu(int itMax, int maxTabuSize){
    int LucroMax = 0;
    Mochila mochila;
    construtivo(mochila);
    tabuList.insert(mochila);
    while (itMax--)
    {
        localSearch(mochila); //local search with Tabu-List
        LucroMax = max(LucroMax, mochila.lucro); //guardo o maior lucro pelas iterações
        tabuList.insert(mochila);
        if(tabuList.size() > maxTabuSize){
            tabuList.erase(tabuList.begin()); //Por enquanto remove uma aletoria/menor lucro
        }
    }
    return LucroMax;
}   

int main(){
    string name = "./dckp_instances/500/dckp_1_sum.txt";
    srand (time(NULL));
    

    printf("Reading files...\n");
    read_file(name , nI, nP, nC, items);
    printf("Files read\n");

    printf("Running Tabu\n");
    Tabu(10, 100);

}