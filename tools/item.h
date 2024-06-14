#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <utility>
#include <algorithm>
#include <bitset>
#include <set>
#define BIT bitset<1000>
using namespace std;

struct item
{
    int value;
    int wheight;
    BIT forbidden;

    bool Compativel(BIT sol){
        return (forbidden & sol) == 0;
    }
};

struct Mochila
{
    int lucro = 0, peso = 0;
    BIT s{0};

    void remove_element(int i, int lucroE, int pesoE){
        s[i] = 0;
        lucro -= lucroE;
        peso -= pesoE;
    }

    void insert_element(int i, int lucroE, int pesoE){
        s[i] = 1;
        lucro += lucroE;
        peso += pesoE;
    }

    bool operator<(const Mochila &a) const{
        for(int i = 0; i < s.size(); i++){
            if(this->s[i] > a.s[i]) return false;
            else if(this->s[i] < a.s[i]) return true;
        }
        return true;
    }
};