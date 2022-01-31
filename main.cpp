#include<iostream>
#include"cloud.h"
#include<fstream>

using namespace std;



int main(){
    const string IMGSOURCE = "./imgs/img1/";
    ifstream fin(IMGSOURCE+"/cloudSize");
    int sizeX, sizeY, sizeZ; fin >> sizeX >> sizeY >> sizeZ;

    Cloud c(sizeX,sizeY,sizeZ);
    for(int angle : {0,20,40,60})
        for (int i = 0; i < 25; i++){
            cout << angle << ":[" << 4*i << "%]\n";
            for (int j : {0,25,50,75}){
                int photo = i + j;
                string path = IMGSOURCE + to_string(angle)+"/pbm/"+to_string(photo)+".pbm";
                ifstream fin(IMGSOURCE+to_string(angle)+"/camPos");
                float cameraDist; fin >> cameraDist; 
                c.crop(path, cameraDist, -3.6*photo,-angle);
            }
        }

    c.findFaces();

    

    for(int i = 0; i < 100; i++)
       c.smoothMesh();
    

for (int i = 0; i < 100; i+=5)
    for(int j = 0; j < 80; j+=20){
        ifstream fin(IMGSOURCE+to_string(j)+"/camPos");
        float cameraDist; fin >> cameraDist;
        string source = IMGSOURCE + to_string(j) + "/ppm/" + to_string(i)+".ppm";
        c.color(source, cameraDist, j,i);
      
    }



    c.writeMesh(IMGSOURCE+"model.ply");
    cout << "done" << endl;

    return 0;    
}