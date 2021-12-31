#include"points.h"
#include"foto.h"
#include<vector>
#include<fstream>
#include<istream>
#include<tuple>
#include<map>
#include<queue>
#define ANGLE_OF_CAMERA 0.145
//#define ANGLE_OF_CAMERA2 0.167459963
using namespace std;
ofstream fout("points.obj");

/*                                             7______________6
    ^ y                                       /.      1      /|
    |                                        / .            / |
    |                                       4--.-----------5  |
    |     z                                 | 3.       5   | 2|
    |   /                                   |  .   4       |  |
    |  /                                    |  3...........|..2
    | /                                     | .      0     | /
    |/                                      |.             |/
    --------------> x                       0--------------1




    O                     
                    |
                  . |
                __I_|
                |   |
                |   |



    

*/

const vector<position >  SITES_INDEXES = { {0,-1,0}, {0,1,0}, {1,0,0}, {-1,0,0}, {0,0,-1}, {0,0,1} };
const vector<vector<position > > SITES_VERTICIES={
    { {-.5,-.5,-.5}, { .5,-.5,-.5}, { .5,-.5, .5}, {-.5,-.5, .5} }, //0
    { {-.5, .5,-.5}, { .5, .5,-.5}, { .5, .5, .5}, {-.5, .5, .5} }, //1
    { { .5,-.5,-.5}, { .5,-.5, .5}, { .5, .5, .5}, { .5, .5,-.5} }, //2
    { {-.5,-.5,-.5}, {-.5,-.5, .5}, {-.5, .5, .5}, {-.5, .5,-.5} }, //3
    { {-.5,-.5,-.5}, { .5,-.5,-.5}, { .5, .5,-.5}, {-.5, .5,-.5} }, //4
    { {-.5,-.5, .5}, { .5,-.5, .5}, { .5, .5, .5}, {-.5, .5, .5} }  //5

};


class Cloud{
    private:
        int sizeX, sizeY, sizeZ;
        position center;
        position camera;
        queue<Point*> points;
        vector<vector<vector<bool> > > pointCloudModel;
    public:
        vector<position> vertex;
        Cloud(int x, int y, int z){
            sizeX = x; sizeY = y; sizeZ = z;    // storing size
            center.x = int(x/2);            // calculating center of point cloud
            center.y = int(y/2);
            center.z = int(z/2);
            // calculating position of camera
            camera.x=center.x; camera.y=center.y; camera.z=camera.x/tan(ANGLE_OF_CAMERA);
            for(int ix = 0; ix < x; ix++)       // pushing points into queue
                for(int iy = 0; iy < y; iy++)
                    for(int iz = 0; iz < z; iz++){
                        Point* pointerP = new Point(ix,iy,iz);
                        points.push(pointerP);
                    }
            
            pointCloudModel.resize(sizeX);  // initialize point cloud model
            for (vector<vector<bool> >& v : pointCloudModel){
                v.resize(sizeY);
            for (vector<bool>& w : v)
                w.resize(sizeZ,false);
            }       

            
          
        }

        void crop(string picture, float angleY, float angleX){
            Point* sep = new Point(-1,-1,-1);   // separator
            Foto img(picture);
            points.push(sep);
    
            while(points.front()!=sep){
                pair<int,int> fotoPos = points.front()->project(center,camera,angleX,angleY); 
                if(img.getBitValue(fotoPos.first,fotoPos.second)){
                    points.push(points.front());
                }
                points.pop();
            }
            points.pop();
        }


        void markPTM(){
            while (!points.empty()){
                Point* p = points.front();
                pointCloudModel[p->getPosition().x][p->getPosition().y][p->getPosition().z]=true;
                points.pop();
            }   
        }


        bool isOut(position pos){
            if (min(min(sizeX-pos.x,sizeY-pos.y),sizeZ-pos.z)==0)
                return true;
            if (min(pos.x, min(pos.y, pos.z))==-1)
                return true;
            return false;
        }

        int countOpositNeighbours(position where){
            int count = 0;
            for(int i = 0; i < 6; i++){
                position neighbour = {where.x+SITES_INDEXES[i].x,
                                      where.y+SITES_INDEXES[i].y,
                                      where.z+SITES_INDEXES[i].z};
                if (!isOut(neighbour) && !pointCloudModel[neighbour.x][neighbour.y][neighbour.z]==pointCloudModel[where.x][where.y][where.z])
                    count+=1;
                    
            }
            return count;
        }

        void solveSingleCubes(){
            for (float ix = 0; ix < sizeX; ix++)
                for (float iy = 0; iy < sizeY; iy++)
                    for (float iz = 0; iz < sizeZ; iz++)
                        if(countOpositNeighbours({ix,iy,iz})>=5){
                            pointCloudModel[ix][iy][iz] = !pointCloudModel[ix][iy][iz];
                            
                        }
        }


        void findFaces(){
            markPTM();
            for(int i = 0; i < 5; i++)
            solveSingleCubes();
                        
            for (int ix = 0; ix < sizeX; ix++)
                for (int iy = 0; iy < sizeY; iy++)
                    for (int iz = 0; iz < sizeZ; iz++) 
                        if (pointCloudModel[ix][iy][iz])
                            for(int i = 0; i < 6; i++){
                                position neighbour = {ix+SITES_INDEXES[i].x,
                                                      iy+SITES_INDEXES[i].y,
                                                      iz+SITES_INDEXES[i].z};
                                
                                if(isOut(neighbour) || !pointCloudModel[neighbour.x][neighbour.y][neighbour.z])
                                  for (position bod : SITES_VERTICIES[i])
                                        vertex.push_back({bod.x+ix, bod.y+iy, bod.z+iz});
                                   
                                }
                            

        }

};


class Mesh{
    public:
        map<tuple<float,float,float>, int> mp;
        vector<Point*> vertex;
        vector<int> faces;
        Mesh(vector<position>& src){
            int currentIndex = 1;
            for(position p: src){
                if(mp.find({p.x,p.y,p.z})==mp.end()){
                    mp.insert({{p.x,p.y,p.z}, currentIndex});
                    vertex.push_back(new Point(p.x,p.y,p.z));
                    currentIndex++;
                }
                faces.push_back(mp.find({p.x,p.y,p.z})->second);
            }

        }

        void setSmoothVectors(){
            for(int i = 0; i < faces.size(); i+= 4){
                for(int j = 0; j < 4; j++){
                    vertex[faces[i+j]-1]->addSmoothVector({vertex[faces[i+(j+1)%4]-1]->getPosition().x,vertex[faces[i+(j+1)%4]-1]->getPosition().y,vertex[faces[i+(j+1)%4]-1]->getPosition().z});
                    vertex[faces[i+j]-1]->addSmoothVector({vertex[faces[i+(j+3)%4]-1]->getPosition().x,vertex[faces[i+(j+3)%4]-1]->getPosition().y,vertex[faces[i+(j+3)%4]-1]->getPosition().z});
                }
            }

        }

        void smoothMesh(){
            for(Point* p : vertex)
                p->resetSmoothVector();

            setSmoothVectors();
            for(Point* p : vertex)
                p->moveBySmoothVector();
        }


        void writeMesh(){

            cout << vertex.size() <<endl;
            for(Point* p : vertex){
                fout << "v " << p->getPosition().x << " " << p->getPosition().y << " " << p->getPosition().z << endl;
               
            }
            for(int i = 0; i < faces.size(); i+=4){
                fout << "f";
                for(int j = 0; j < 4; j++)
                    fout << " " << faces[i+j];
                fout << endl;
            }
        }
        

};
