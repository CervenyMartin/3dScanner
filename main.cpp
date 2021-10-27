#include<iostream>
#include"cloud.h"
#include<vector>
#include<fstream>
#define IMGSOURCE "./imgs/pbms/"
using namespace std;

int main(){

    Cloud c(234, 300, 234);

    for(int angle : {0})
        for(int foto = 0; foto < 100; foto += 1000){
            string path = IMGSOURCE+to_string(angle)+"/"+to_string(foto)+".pbm";
            c.crop(path, foto*3.6, -angle);
            cout << angle << ": " << foto << endl;
        }

    c.findFaces();
    Mesh m(c.vertex);
    for(int i = 0; i < 100; i++)
        m.smoothMesh();

    m.writeMesh();
    cout << "done" << endl;


    return 0;    
}