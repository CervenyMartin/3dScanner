#include<iostream>
#include"cloud.h"
#include<vector>
#include<fstream>
#define IMGSOURCE "./imgs/img1/"
using namespace std;

int main(){
    vector<int> v;
    Cloud c;
    for(int angle : {0,20,40,60})
        for (int i = 0; i < 25; i++){
            cout << angle << ":[" << 4*i << "%]\n";
            for (int j : {0,25,50,75}){
                int photo = i + j;
                string path = IMGSOURCE "pbms/"+ to_string(angle)+"/"+to_string(photo)+".pbm";
                c.crop(path, -3.6*photo,-angle);
            }
        }

    c.findFaces();

    

    for(int i = 0; i < 100; i++)
       c.smoothMesh();
    

for (int i = 0; i < 100; i+=5)
    for(int j = 0; j < 80; j+=20){
    c.color(IMGSOURCE, j,i);
    c.color(IMGSOURCE, j,i);
    c.color(IMGSOURCE, j,i);
    c.color(IMGSOURCE, j,i);
}


    c.writeMesh();
    cout << "done" << endl;

    return 0;    
}