#include "./tools/file.h"
#include "./tools/statistics.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <utility>
#include <bitset>
#include <algorithm>
#include <set>
#include <queue>
#include <chrono>
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
queue<Mochila> tabuQueue;

vector<pair<double,int>> SortedPairs;

void buildSortedPairs(int razaoValue, int razaoPeso){
  for(int i = 0; i < nI ; i++){
    //mudar os pesos da razão!!
      SortedPairs.push_back({(double)pow(items[i].value, razaoValue)/(double)pow(items[i].wheight,razaoPeso), i});
  }
  sort(SortedPairs.rbegin(), SortedPairs.rend());
}


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

void construtivo_fraco(double alfa, Mochila &mochila){
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

void construtivo_forte(double alfa, Mochila& mochila, int iteracoes){
  vector <int> freq(nI, 0);
  for(int i = 0; i < iteracoes; i++){
    Mochila tmp;
    construtivo_fraco(alfa, tmp);
    for(int i = 0; i < nI; i++){
      if(tmp.s[i] == 1){
        freq[i] += 1;
      }
    }
  }
  vector <int> tmp(nI);
  for(int i = 0; i < nI; i++){
    tmp[i] = i;
  }
  sort(tmp.begin(), tmp.end(), [&](int i1, int i2){return freq[i1] > freq[i2];});
  for(int i = 0; i < nI; i++){
    int item = tmp[i];
    if(items[item].Compativel(mochila.s) && mochila.peso + items[item].wheight <= nC/2){
      mochila.insert_element(item, items[item].value, items[item].wheight);
    }
  }

  auto cmp = [&](int i1, int i2){
    return (double)items[i1].value/items[i1].wheight > (double)items[i2].value/items[i2].wheight;
  };
  sort(tmp.begin(), tmp.end(), cmp);
  for(int i = 0; i < nI; i++){
    int item = tmp[i];
    if(items[item].Compativel(mochila.s) && mochila.peso + items[item].wheight <= nC){
      mochila.insert_element(item, items[item].value, items[item].wheight);
    }
  }
}

void init(){
    SortedPairs.clear();
    buildSortedPairs(1,1);
}

int Tabu(int itMax, int maxTabuSize, int itConstru){
    int LucroMax = 0;
    Mochila mochila;
    init();
    construtivo_forte(0.75, mochila, itConstru);
    tabuList.insert(mochila);
    tabuQueue.push(mochila);
    while (itMax--)
    {
        localSearch(mochila); //local search with Tabu-List
        LucroMax = max(LucroMax, mochila.lucro); //guardo o maior lucro pelas iterações
        tabuList.insert(mochila);
        tabuQueue.push(mochila);
        if(tabuQueue.size() > maxTabuSize){
            tabuList.erase(tabuQueue.front()); //Por enquanto remove uma aletoria/menor lucro
            tabuQueue.pop();
        }
    }
    return LucroMax;
}   

int main(int argc, char **argv){

    if(argc < 2){ cout << "Argumentos não suficientes\n";}
    string TamInst = argv[1];
    string name = "./dckp_instances/" + TamInst + "/";
    string instB = "dckp_", instE = "_sum.txt";
    srand (time(NULL));
    const int MaxTabuSize = 1000;

    for(int i = 1; i <= 10; i++){

        string arq_inp = name + instB + to_string(i) + instE;
        string arq_out = "./results/Tabu/" + TamInst + "/dckp_" + to_string(i) + "_result.txt"; 

        printf("Reading files...\n");
        items.clear();
        nI = nP = nC = 0;
        read_file(arq_inp, nI, nP, nC, items);
        printf("Files read\n");

        printf("Running Tabu\n");

        vector<double> solucoes, tempos;
        for(int i = 0; i < 30; i++){
            auto start = std::chrono::high_resolution_clock::now();
            int sol = Tabu(100, MaxTabuSize, 1000);
            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> elapsed = end - start;
            double execution_time = elapsed.count();
            solucoes.push_back(sol);
            tempos.push_back(execution_time);
            printf("Iretation %d\n", i);
        }
        
        vector<string> params;
        params.push_back("Tamanho da lista tabu = " + to_string(MaxTabuSize));
        write_solutions(params, solucoes, tempos, arq_out);
        
    }

}