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
using ordered_multiset = tree<ll, null_type, less_equal<ll>, rb_tree_tag, tree_order_statistics_node_update>;

using namespace std;

int nI, nP, nC;
vector<item> items;
vector<pair<double,int>> SortedPairs;

void buildSortedPairs(){
    for(int i = 0; i < nI ; i++){
        SortedPairs.push_back({(double)items[i].value/(double)items[i].wheight, i});
    }
    sort(SortedPairs.rbegin(), SortedPairs.rend());
}

//Resolve o problema da mochila Fracionaria
int Resolve_Fracionaria(){
    buildSortedPairs();
    int cap = nC, lucro = 0;
    for(auto a : SortedPairs){
        int i  = a.second;
        if(cap <= 0) break;
        if(items[i].wheight >= cap){
            lucro += items[i].value;
            cap -= items[i].wheight;
        }else{
            lucro += a.first * (double)cap;
            cap = 0;
        }
    }
    return lucro;
}



//Checa a Vizinhança e retorna o melhor e muda a solução
void checkNeighbors(Mochila &mochila){

    int melhor = 0;
    pair<int,int> par = {-1,-1};
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

//Faz um Pertubação Aleatória na solução
void Pertubation(Mochila &mochila){
    //Gera uma lista de caras que podem entrar
    // se estiver vazia tiramos um aleatorio
    // se não, escolhemos aleatoriamente para colocar ou botar
    vector<int> PodemEntrar, PodemSair;
    for(int i = 0; i < nI; i++){
        if(mochila.s[i] == 1) {PodemSair.push_back(i); continue;}
        if( mochila.peso + items[i].wheight <= nC && items[i].Compativel(mochila.s) ){ PodemEntrar.push_back(i);}
    }

    int operação = rand()%2;

    if(PodemEntrar.size() == 0){
        int rp = rand() % PodemSair.size();
        int elem = PodemSair[rp];
        mochila.remove_element(elem, items[elem].value, items[elem].wheight);
        return;
    }
    if(PodemSair.size() == 0){
        int rp = rand() % PodemEntrar.size();
        int elem = PodemEntrar[rp];
        mochila.insert_element(elem, items[elem].value, items[elem].wheight);
        return;
    }

    if(operação){
        int rp = rand() % PodemSair.size();
        int elem = PodemSair[rp];
        mochila.remove_element(elem, items[elem].value, items[elem].wheight);
    }else{
        int rp = rand() % PodemEntrar.size();
        int elem = PodemEntrar[rp];
        mochila.insert_element(elem, items[elem].value, items[elem].wheight);
    }

}

//Decide se a solução nova vai ser aceita ou não,
//deve balancear diversidade com otimalidade
Mochila CriterioAceitacao(Mochila &mochila, Mochila &nova){
    //Resolver a mochila fracionaria
    // e usar o resultado (razao do lucro atual com o da fracionaria) como probabilidade
    // para pegar a solução
    int lucroFrac =  Resolve_Fracionaria();
    int rp = rand() % lucroFrac;
    if(nova.lucro <= rp){
        return nova;
    }else{
        return mochila;
    }

}
//Retorna uma solução construtiva do problema
void construtivo(Mochila Mochila){
    //Para cada dupla, testar com base no guloso básico
    //qual tem a melhor mochila. Com isso fixamos esses 2 elementos, e
    //resolvemos para o subconjunto sem eles.
    // Testar primeiro o de um elemento só em vez de dupla.

}


void ILS(int itMax){
    Mochila mochila;
    construtivo(mochila);
    localSearch(mochila);
    for(int i = 0; i < itMax; i++){
        Mochila nova = mochila;
        Pertubation(nova);
        localSearch(nova);
        mochila = CriterioAceitacao(mochila, nova);
        printf("Iretation %d: LucroMax = %d\n", i+1, mochila.lucro);
    }
}

int main(){
    string name = "./dckp_instances/500/dckp_1_sum.txt";
    srand (time(NULL));
    

    printf("Reading files...\n");
    read_file(name , nI, nP, nC, items);
    printf("Files read\n");

    printf("Running ILS\n");
    ILS(10);

}