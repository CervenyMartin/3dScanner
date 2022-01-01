#include"points.h"
#include"foto.h"
#include<vector>
#include<fstream>
#include<istream>
#include<tuple>
#include<map>
#include<queue>
#include<set>

using namespace std;

#define CAMERA_X 117
#define CAMERA_y 150
#define CAMERA_Z 801.234
#define SIZE_X 234
#define SIZE_Y 300
#define SIZE_Z 234

position camera;
position size;
position center;
ofstream fout("model.ply");

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

struct cmp {
    bool operator() (pair<Point*,int> a, pair<Point*,int> b) const {
        return sqrt(
                   pow(a.first->getPosition().x-camera.x,2)
                +  pow(a.first->getPosition().y-camera.y,2)
                +  pow(a.first->getPosition().z-camera.z,2)
                ) > sqrt(pow(b.first->getPosition().x-camera.x,2)
                +  pow(b.first->getPosition().y-camera.y,2)
                +  pow(b.first->getPosition().z-camera.z,2)
                );
    }
};



class Cloud{
    private:
        queue<Point*> points;
        vector<vector<vector<bool> > > pointCloudModel;
        position camera;
        position size;
        position center;
    public:
        vector<position> vertex;
        Cloud(){
            camera={CAMERA_X,CAMERA_y,CAMERA_Z};
            size={SIZE_X, SIZE_Y, SIZE_Z};
            center={size.x/2,size.y/2,size.z/2};
            for(int iy = 0; iy < size.y; iy++)       // pushing points into queue
                for(int ix = 0; ix < size.x; ix++)
                    for(int iz = 0; iz < size.z; iz++){
                        Point* pointerP = new Point(ix,iy,iz);
                        points.push(pointerP);
                    }
            
            pointCloudModel.resize(size.x);  // initialize point cloud model
            for (vector<vector<bool> >& v : pointCloudModel){
                v.resize(size.y);
            for (vector<bool>& w : v)
                w.resize(size.z,false);
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
            if (min(min(size.x-pos.x,size.y-pos.y),size.z-pos.z)==0)
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
            for (float ix = 0; ix < size.x; ix++)
                for (float iy = 0; iy < size.y; iy++)
                    for (float iz = 0; iz < size.z; iz++)
                        if(countOpositNeighbours({ix,iy,iz})>=5){
                            pointCloudModel[ix][iy][iz] = !pointCloudModel[ix][iy][iz];
                            
                        }
        }


        void findFaces(){
            cout << points.size() << endl;
//            for(int i = 0; i < 5; i++)
            //solveSingleCubes();
            markPTM();
            for (int ix = 0; ix < size.x; ix++)
                for (int iy = 0; iy < size.y; iy++)
                    for (int iz = 0; iz < size.z; iz++) 
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
    private:
        position camera={CAMERA_X,CAMERA_y,CAMERA_Z};
        position size={SIZE_X, SIZE_Y, SIZE_Z};
        position center={size.x/2,size.y/2,size.z/2};
    public:
        map<tuple<float,float,float>, int> mp;
        vector<Point*> vertex;
        vector<int> faces;

        Mesh(vector<position>& src){
            int currentIndex = 0;
            
            for(position p: src){
                if(mp.find({p.x,p.y,p.z})==mp.end()){
                    mp.insert({{p.x,p.y,p.z}, currentIndex});
                    vertex.push_back(new Point(p.x,p.y,p.z));
                    currentIndex++;
                }
                faces.push_back(mp.find({p.x,p.y,p.z})->second);
            }

            colorize();
        }

        void setSmoothVectors(){
            for(int i = 0; i < faces.size(); i+= 4){
                for(int j = 0; j < 4; j++){
                    vertex[faces[i+j]]->addSmoothVector({vertex[faces[i+(j+1)%4]]->getPosition().x,vertex[faces[i+(j+1)%4]]->getPosition().y,vertex[faces[i+(j+1)%4]]->getPosition().z});
                    vertex[faces[i+j]]->addSmoothVector({vertex[faces[i+(j+3)%4]]->getPosition().x,vertex[faces[i+(j+3)%4]]->getPosition().y,vertex[faces[i+(j+3)%4]]->getPosition().z});
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





        void colorize(){
            
            vector<vector<set<pair<Point*,int>, cmp> > > grid;
            grid.resize(SIZE_X);
            for(vector<set<pair<Point*,int>, cmp> >& i : grid)
                i.resize(SIZE_Y);

            for (Point* v : vertex){
                pair<int,int> where = v->project(center,camera,-40,30);
          
                grid[where.first][where.second].insert({v,
                        int(sqrt(
                               pow(v->getPosition().x-camera.x,2)
                            +  pow(v->getPosition().y-camera.y,2)
                            +  pow(v->getPosition().z-camera.z,2)
                        ))
                });
            }

            for (int i = 1; i < SIZE_X-1; i++)
                for(int j = 1; j < SIZE_Y-1; j++)
                    if(!grid[i][j].empty()){
                    int nearest = grid[i][j].begin()->second;
                    for(int a : {-1,1}) for(int b : {-1,1})
                        if(!grid[i+a][j+b].empty())
                           nearest = min(nearest, grid[i+a][j+b].begin()->second);
                    for (pair<Point*,int> v:grid[i][j]){
                            if (v.second-nearest < 7){
                                v.first->color={0,0,0};
                            }
                    }

                }
            

        }


        void writeMesh(){
            fout << "ply\n";
            fout << "format ascii 1.0\n";
            fout << "comment author: Martin Cerveny\n";
            fout << "element vertex "<< vertex.size() << endl;
            fout << "property float x\n";
            fout << "property float y\n";
            fout << "property float z\n";
            fout << "property uchar red\n";
            fout << "property uchar green\n";
            fout << "property uchar blue\n";
            fout << "element face " << faces.size()/4 << endl;
            fout << "property list uchar int vertex_index\n";
            fout << "end_header\n"; 

            for(Point* p : vertex)
                fout << p->getPosition().x << " " << p->getPosition().z << " " << p->getPosition().y <<" "<< p->color.x << " " << p->color.y <<" "<< p->color.z << endl;
               
            for(int i = 0; i < faces.size(); i+=4){
                fout << "4";
                for(int j = 0; j < 4; j++)
                    fout << " " << faces[i+j];
                fout << endl;
            }
        }
        

};
