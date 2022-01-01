#include<iostream>
#include"cloud.h"
#include<vector>
#include<fstream>
#define IMGSOURCE "./img2/pbms/"
using namespace std;

int main(){
    vector<int> v;
    Cloud c;
    for(int angle : {0,20,40,60})
        for(int foto = 0; foto<100; foto+=10){
            string path = IMGSOURCE+to_string(angle)+"/"+to_string(foto)+".pbm";
            c.crop(path, foto*3.6, -angle);
            cout << angle << ": " << foto << endl;
        }
  /*  while (!c.points.empty()){
        if (c.points.front()->getPosition().x ==81&&c.points.front()->getPosition().y ==29&&c.points.front()->getPosition().z ==92)
            cout << "in"<<endl;
        c.points.pop();
    }

    return 0;
*/



    c.findFaces();

    cout << "pc ok";

    Mesh m(c.vertex);

    for(int i = 0; i < 100; i++)
       m.smoothMesh();
    m.colorize(0,180);
    m.writeMesh();
    cout << "done" << endl;

    return 0;    
}