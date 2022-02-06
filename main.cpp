#include<iostream>
#include"cloud.h"
#include<fstream>

using namespace std;



int main(){
    const string IMGSOURCE = "./imgs/img9/";
    ifstream inSize(IMGSOURCE+"/cloudSize");
    int sizeX, sizeY, sizeZ; inSize >> sizeX >> sizeY >> sizeZ;
    cout << sizeX << " " << sizeY << " " << sizeZ << endl;
    Cloud c(sizeX,sizeY,sizeZ);
    for(int angle : {0}){
        ifstream inCamera(IMGSOURCE+to_string(angle)+"/camPos");
        float cameraX,cameraY,cameraZ; inCamera >> cameraX >> cameraY >> cameraZ;
        ifstream inCenter(IMGSOURCE+to_string(angle)+"/cloudCenter");
        float centerX, centerY, centerZ; inCenter >> centerX >> centerY >> centerZ;
        
        for (int i =0 ; i < 25;i+=100){
            cout << angle << ":[" << 4*i << "%]\n";
            for (int j : {50}){
                int photo = i+ j;
                string path = IMGSOURCE + to_string(angle)+"/pbm/"+to_string(photo)+".pbm"; 
                c.crop(path, {cameraX, cameraY, cameraZ},{centerX+7, centerY, centerZ}, -3.6*(photo),-angle);
            }
        }
    }

    c.findFaces();

    

    for(int i = 0; i < 300; i++)
       c.smoothMesh();

//    cout << "here!"<< endl;
//    c.decimate(2);
//    cout << "here"<<endl;

//    c.setNormVects();
/*
for (int i = 0; i < 100; i++)
    for(int j : {12,40}){
        ifstream inCamera(IMGSOURCE+to_string(j)+"/camPos");
        float cameraX,cameraY,cameraZ; inCamera >> cameraX >> cameraY >> cameraZ;
        ifstream inCenter(IMGSOURCE+to_string(j)+"/cloudCenter");
        float centerX, centerY, centerZ; inCenter >> centerX >> centerY >> centerZ;
        string source = IMGSOURCE + to_string(j) + "/ppm/" + to_string(i)+".ppm";
        c.color(source, {cameraX, cameraY, cameraZ},{centerX, centerY, centerZ}, j,i);
      
    }
*/
    c.writeMesh(IMGSOURCE+"model.ply");
    cout << "done" << endl;

    return 0;    
}