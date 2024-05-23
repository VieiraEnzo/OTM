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

int num;
int nI, nP, nC;
vector<item> items;
vector<pair<double,int>> SortedPairs;

struct LCR
{   
    //HMAX VAI DAR MERDA!!!!
    //SE REMOVER TEM QUE ATUALIZAR
    //
    //Problema: se existir um gap, e a aleatoriedade permitir, é possivel que hmin nn seja grande o 
    // suficiente para atualizar ci <= hamax + alfa*(hmin- hmax)
    int r = 0;
    double hmin, hmax, alfa;
    set<pair<double,int>> elementos;

    LCR (double alfa) : alfa(alfa){

        hmin = SortedPairs[0].first;
        hmax = SortedPairs[SortedPairs.size()-1].first;
        for(r; r < nI; r++) {
            if(SortedPairs[r].first <= hmax + alfa * (hmin - hmax)){
                elementos.insert(SortedPairs[r]);
            }else{break;}
        }
    }

    int get_element(){
        cout << elementos.size() << " " << nI << " " << r << " " << hmin << "\n";
        auto it = elementos.begin();
        int rp = rand() % elementos.size();
        advance(it,rp);
        auto removido = *it;
        elementos.erase(it);
        hmin = (*elementos.begin()).first;
        atualizarLCR();
        return removido.second;
    }

    void atualizarLCR(){
        for(; r < nI; r++){
            if(SortedPairs[r].first <= hmax + alfa * (hmin - hmax)){
                elementos.insert(SortedPairs[r]);
            }else{break;}
        }
    }

    //Precisa, saber o menor elemento
    //Sabendo o menor, atualizar o LCR com os novos elementos
    //Escolher um elemento aleatório
    //Tirar elemento aleatório

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
    Mochila mochila;
    buildSortedPairs();
    for(int i = 0; i < maxIt; i++){
        num++;
        cout << "constuctive: " << num << "\n";
        constructive(alfa, mochila);
        cout << "Local: " << num << "\n";
        int sol = localSearch(mochila);
        LucroMax = max(LucroMax, sol);
        printf("Iretation %d: LucroMax = %d\n", i+1, sol);
    }
    
    return LucroMax;
}



int main(){
    string name = "./dckp_instances/500/dckp_1_sum.txt";
    double alfa = 0.75;
    srand (time(NULL));
    

    printf("Reading files...\n");
    read_file(name , nI, nP, nC, items);
    printf("Files read\n");

    printf("Running GRASP\n");
    GRASP(alfa, 100);

}