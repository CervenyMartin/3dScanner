#include<vector>
#include<fstream>
#include<iostream>
using namespace std;
#define FOTOX 234
#define FOTOY 300
class Foto{
    private:
        vector<bool> img;
    public:
        Foto(string path){
            ifstream fin(path);
            string bits=""; fin >> bits;         
            
            for(int y = FOTOY-1; y >=0; y--)
                for(int x = FOTOX-1; x >= 0; x--)
                    if(bits[FOTOX*y+x]=='0') img.push_back(1);
                    else img.push_back(0);
          
        }
         bool getBitValue(int x, int y){
            if(x < 0 || x > FOTOX-1 || y < 0 || y > FOTOY-1) return 0;
            return img[y*FOTOX + x];
        }
};