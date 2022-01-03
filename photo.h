#include<vector>
#include<fstream>
#include<iostream>
#include<tuple>
#include<array>
using namespace std;

class PBMphoto{
    private:
        vector<bool> img;
        string header;
        int sizeX, sizeY;
        string bits;
    public:
        PBMphoto(string path){
            ifstream fin(path);
            fin>> header >> sizeX >> sizeY >> bits;
            for(int y = sizeY-1; y >=0; y--)
                for(int x = sizeX-1; x >= 0; x--)
                    if(bits[sizeX*y+x]=='0') img.push_back(1);
                    else img.push_back(0);
          
        }
        bool getBitValue(int x, int y){
            if(x < 0 || x > sizeX-1 || y < 0 || y > sizeY-1) return 0;
            return img[y*(sizeX) +x];
        }
};


class PPMphoto{
    private:
        int sizeX; int sizeY; int maxVal;
        vector<vector<array<int,3>> > img;
    public:
        PPMphoto(string path){
            ifstream fin(path);
            string header;
            string bits="";
            fin>>header; fin >> sizeX >> sizeY; fin >> maxVal;
            img.resize(sizeY);
            for(auto& y : img) y.resize(sizeX);

            for (int y = sizeY-1; y>=0; y--)
                for(int x = sizeX-1; x>=0; x--){
                    int r,g,b;
                    fin >> r >> g >> b;
                    img[y][x] = {r,g,b};
                }
          
        }
        array<int,3> getBitValue(int x, int y){
             return img[y][x];
         }
};