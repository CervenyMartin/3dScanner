
#include<iostream>
#include<vector>
#include<set>
using namespace  std;

int main(){
    set<tuple<float,float,float>,cmp > s;
    s.insert({float(1.7),float(2.7),float(3.8)});

   
    return 0;
}