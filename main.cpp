#include<iostream>
#include"cloud.h"
#include<fstream>

using namespace std;



int main(){
    const string IMGSOURCE = "./imgs/img12/";
    ifstream inSize(IMGSOURCE+"/cloudSize");
    int sizeX, sizeY, sizeZ; inSize >> sizeX >> sizeY >> sizeZ;
    cout << sizeX << " " << sizeY << " " << sizeZ << endl;
    Cloud c(sizeX,sizeY,sizeZ);
    for(int angle : {12,40}){
        ifstream inCamera(IMGSOURCE+to_string(angle)+"/camPos");
        float cameraX,cameraY,cameraZ; inCamera >> cameraX >> cameraY >> cameraZ;
        ifstream inCenter(IMGSOURCE+to_string(angle)+"/cloudCenter");
        float centerX, centerY, centerZ; inCenter >> centerX >> centerY >> centerZ;
        cout << cameraX << " " << cameraY << " " << cameraZ << "\n";
        cout << centerX << " " << centerY << " " << centerZ << "\n";
        for (int i =0 ; i < 25; i++){
            cout << angle << ":[" << 4*i << "%]\n";
            for (int j : {0,25,50,75}){
                int photo = i+ j;
                string path = IMGSOURCE + to_string(angle)+"/pbm/"+to_string(photo)+".pbm"; 
                c.crop(path, {cameraX, cameraY, cameraZ},{centerX, centerY, centerZ}, -3.6*(photo),-angle);
            }
        }
    }

    c.findFaces();

    

    for(int i = 0; i < 300; i++)
       c.smoothMesh();


for (int i = 0; i < 100; i++)
    for(int j : {12}){
        ifstream inCamera(IMGSOURCE+to_string(j)+"/camPos");
        float cameraX,cameraY,cameraZ; inCamera >> cameraX >> cameraY >> cameraZ;
        ifstream inCenter(IMGSOURCE+to_string(j)+"/cloudCenter");
        float centerX, centerY, centerZ; inCenter >> centerX >> centerY >> centerZ;
        string source = IMGSOURCE + to_string(j) + "/ppm/" + to_string(i)+".ppm";
        c.color(source, {cameraX, cameraY, cameraZ},{centerX, centerY, centerZ}, j,i);
      
    }


    c.writeMesh(IMGSOURCE+"model.ply");
    cout << "done" << endl;

    return 0;    
}