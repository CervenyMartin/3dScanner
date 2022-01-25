#include<iostream>
#include"cloud.h"
#include<vector>
#include<fstream>
#define IMGSOURCE "./imgs/img1/"
using namespace std;

int main(){
    vector<int> v;
    Cloud c;
    for(int angle : {0})
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

    c.color(IMGSOURCE, 0,0);
    c.color(IMGSOURCE, 0,25);
    c.color(IMGSOURCE, 0,50);
    c.color(IMGSOURCE, 0,75);

    c.writeMesh();
    cout << "done" << endl;

    return 0;    
}