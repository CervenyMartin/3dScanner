# include<iostream>
# include<tuple>
# include<map>
using namespace std;
struct position{
    int x,y,z;
};

int main(){

    map<tuple<int,int,int> ,int> mp;
    tuple<int,int,int> p = {1,2,3};
    mp.insert({p,4});
    cout<<"ok";


    return  0;
}