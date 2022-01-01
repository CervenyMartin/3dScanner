# include<iostream>
#include <set>
#include "points.h"
using namespace std;

int main(){

    set<int> s;
    s.insert(5);
    s.insert(7);
    s.insert(2);
    s.insert(6);
    s.insert(0);
    s.insert(3);
    s.insert(4);
    for(int k : s)
        cout << k << endl;
    return  0;
}