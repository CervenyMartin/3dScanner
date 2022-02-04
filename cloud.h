#include"points.h"
#include"photo.h"
#include<vector>
#include<fstream>
#include<istream>
#include<tuple>
#include<map>
#include<queue>
#include<set>

using namespace std;

const vector<position >  SITES_INDEXES = { {0,-1,0}, {0,1,0}, {1,0,0}, {-1,0,0}, {0,0,-1}, {0,0,1} };
const vector<vector<position > > SITES_VERTICIES={
    { {-.5,-.5,-.5}, { .5,-.5,-.5}, { .5,-.5, .5}, {-.5,-.5, .5} },
    { {-.5, .5,-.5}, { .5, .5,-.5}, { .5, .5, .5}, {-.5, .5, .5} },
    { { .5,-.5,-.5}, { .5,-.5, .5}, { .5, .5, .5}, { .5, .5,-.5} },
    { {-.5,-.5,-.5}, {-.5,-.5, .5}, {-.5, .5, .5}, {-.5, .5,-.5} },
    { {-.5,-.5,-.5}, { .5,-.5,-.5}, { .5, .5,-.5}, {-.5, .5,-.5} },
    { {-.5,-.5, .5}, { .5,-.5, .5}, { .5, .5, .5}, {-.5, .5, .5} }

};



class Cloud{
    private:
        position size;
        queue<Point*> cubes;
        vector<vector<vector<bool> > > pointCloudModel;
        map<tuple<float,float,float>, int> mp;
        vector<Point*> verticies;
        vector<int> faces;
    public:
        Cloud(int sizeX, int sizeY, int sizeZ){            
            size={float(sizeX), float(sizeY), float(sizeZ)};
           
            for(int ix = 0; ix < size.x; ix++)       // pushing cubes into queue
                for(int iy = 0; iy < size.y; iy++)
                    for(int iz = 0; iz < size.z; iz++){
                        Point* pointerP = new Point(ix,iy,iz);
                        cubes.push(pointerP);
                    }
            
            pointCloudModel.resize(size.x);  // initialize point cloud model
            for (vector<vector<bool> >& v : pointCloudModel){
                v.resize(size.y);
            for (vector<bool>& w : v)
                w.resize(size.z,false);
            }        
        }

        void crop(string picture, position camera, position center, float angleZ, float angleX){
            PBMphoto img(picture);
            Point* sep = new Point(-1,-1,-1);   // separator
            
            cubes.push(sep);
            while(cubes.front()!=sep){
                pair<int,int> fotoPos = cubes.front()->project(center,camera,size,angleX,angleZ); 
                if(img.getBitValue(fotoPos.first,fotoPos.second))
                    cubes.push(cubes.front());
                
                cubes.pop();
            }
            cubes.pop();
        }


        void markPTM(){
            while (!cubes.empty()){
                Point* p = cubes.front();
                pointCloudModel[p->getPosition().x][p->getPosition().y][p->getPosition().z]=true;
                cubes.pop();
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
                        if(countOpositNeighbours({ix,iy,iz})>=5)
                            pointCloudModel[ix][iy][iz] = !pointCloudModel[ix][iy][iz];
                         
        }


        void findFaces(){
            cout << cubes.size() << endl;
            int currentIndex = 0;
            for(int i = 0; i < 5; i++)
               solveSingleCubes();
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
                                  for (position bod : SITES_VERTICIES[i]){
                                        position p = {bod.x+ix,bod.y+iy,bod.z+iz}; 
                                        if(mp.find({p.x,p.y,p.z})==mp.end()){
                                            mp.insert({{p.x,p.y,p.z}, currentIndex});
                                            verticies.push_back(new Point(p.x,p.y,p.z));
                                            currentIndex++;
                                        }
                                        faces.push_back(mp.find({p.x,p.y,p.z})->second);
                                  }

    
                            }
                            

        }

        void setSmoothVectors(){
            for(int i = 0; i < faces.size(); i+= 4){
                for(int j = 0; j < 4; j++){
                    verticies[faces[i+j]]->addSmoothVector({verticies[faces[i+(j+1)%4]]->getPosition().x,verticies[faces[i+(j+1)%4]]->getPosition().y,verticies[faces[i+(j+1)%4]]->getPosition().z});
                    verticies[faces[i+j]]->addSmoothVector({verticies[faces[i+(j+3)%4]]->getPosition().x,verticies[faces[i+(j+3)%4]]->getPosition().y,verticies[faces[i+(j+3)%4]]->getPosition().z});
                }
            }

        }


        void smoothMesh(){
            for(Point* p : verticies)
                p->resetSmoothVector();

            setSmoothVectors();
            for(Point* p : verticies)
                p->moveBySmoothVector();
        }


        void color(string source, position camera, position center, float angleX, float angleZ){
            vector<vector<vector<pair<Point*,int> > > > grid;
            grid.resize(size.z);
            for(vector<vector<pair<Point*,int> > >& i : grid)
                i.resize(size.x);
            for (Point* v : verticies){
                pair<int,int> where = v->project(center,camera,size,-angleX,angleZ*(-3.6));
                if(where.first >= 0 && where.first < size.x && where.second >=0 && where.second < size.z){
                    position rotated = v->rotate(center,-angleX,angleZ*(-3.6));
                    int dist = sqrt(
                                   pow(rotated.x-camera.x,2)
                                +  pow(rotated.y-camera.y,2)
                                +  pow(rotated.z-camera.z,2)
                                );
                    if(!grid[where.second][where.first].empty() && dist < grid[where.second][where.first].front().second){
                        grid[where.second][where.first].push_back(grid[where.second][where.first].front());
                        grid[where.second][where.first][0]={v,dist};
                    } else grid[where.second][where.first].push_back({v,dist});

                }
            }

            PPMphoto f(source);
            
            for (int i = 2; i < size.z - 2; i++)
                for(int j = 2; j < size.x - 2; j++)
                    if(!grid[i][j].empty()){
                    int nearest = grid[i][j].begin()->second;
                    for(int a : {-2,-1,1,2}) for(int b : {-2,-1,1,2})
                        if(!grid[i+a][j+b].empty())
                           nearest = min(nearest, grid[i+a][j+b].begin()->second);
                    for (pair<Point*,int> v:grid[i][j]){
                            if (v.second-nearest < 6){
                                array<int,3> col = f.getBitValue(j,i);
                                v.first->add2AverageColor({float(col[0]),float(col[1]),float(col[2])});
                            }
                    }

                }


        }


        void writeMesh(string source){
            ofstream fout(source);

            for (Point* p: verticies)
                p->setAverageColor();

            fout << "ply\n";
            fout << "format ascii 1.0\n";
            fout << "comment author: Martin Cerveny\n";
            fout << "element vertex "<< verticies.size() << endl;
            fout << "property float x\n";
            fout << "property float y\n";
            fout << "property float z\n";
            fout << "property uchar red\n";
            fout << "property uchar green\n";
            fout << "property uchar blue\n";
            fout << "element face " << faces.size()/4 << endl;
            fout << "property list uchar int verticies_index\n";
            fout << "end_header\n"; 

            for(Point* p : verticies)
                fout << p->getPosition().x << " " << p->getPosition().y << " " << p->getPosition().z <<" "<< p->color.x << " " << p->color.y <<" "<< p->color.z << endl;
               
            for(int i = 0; i < faces.size(); i+=4){
                fout << "4";
                for(int j = 0; j < 4; j++)
                    fout << " " << faces[i+j];
                fout << endl;
            }
        }
       
};
