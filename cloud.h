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



class Cloud{
    private:
        
        vector<vector<vector<bool> > > pointCloudModel;
        position camera;
        position size;
        position center;
    public:
        queue<Point*> points;
        vector<position> vertex;
        Cloud(){
            
            camera={CAMERA_X,CAMERA_y,CAMERA_Z};
            size={SIZE_X, SIZE_Y, SIZE_Z};
            center={size.x/2,size.y/2,size.z/2};
            cout << "center: " << center.x << " " << center.y << " " << center.z << endl;
            cout << "camera: " << camera.x << " " << camera.y << " " << camera.z << endl;
           // Point zk(80.5,28.5,91.5);
            //cout << zk.project(center,camera,0,150).second;
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
            Foto img(picture);
            Point* sep = new Point(-1,-1,-1);   // separator
            
            points.push(sep);
            while(points.front()!=sep){
                pair<int,int> fotoPos = points.front()->project(center,camera,angleX,angleY); 
                if(img.getBitValue(fotoPos.first,fotoPos.second)){
                    points.push(points.front());
                    if(points.front()->getPosition().x == 81 && points.front()->getPosition().y == 29 && points.front()->getPosition().z == 92)
                        cout << "uuuuu " << fotoPos.first <<" "<< fotoPos.second << endl;
                      

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
                                  for (position bod : SITES_VERTICIES[i]){
                                        vertex.push_back({bod.x+ix, bod.y+iy, bod.z+iz});
                                        if (bod.x+ix ==80.5&&bod.y+iy ==28.5&&bod.z+iz==91.5)
                                         cout << "tady !!! taky" << ix << " " << iy << " " << iz <<endl;
                                    }
                                        
                                   
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
                    if (p.x ==80.5&&p.y ==28.5&&p.z ==91.5)
                                         cout << "IN here"<<endl;
                    currentIndex++;
                }
                faces.push_back(mp.find({p.x,p.y,p.z})->second);
            }

            
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


        void colorize(float angleX, float angleY){
            Point z (81,29,92);
            pair<int,int> w = z.project(center,camera,0,0);
            vector<vector<vector<pair<Point*,int> > > > grid;
            grid.resize(SIZE_Y);
            for(vector<vector<pair<Point*,int> > >& i : grid)
                i.resize(SIZE_X);

            for (Point* v : vertex){
                pair<int,int> where = v->project(center,camera,angleX,angleY);
                if(where.first >= 0 && where.first <SIZE_X && where.second >=0 && where.second <SIZE_Y){
                    position rotated = v->rotate(center,camera,angleX,angleY);
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
            cout << "grid created!" << endl;
            ppmFoto f("./0.ppm");
            for (int i = 2; i < SIZE_Y-2; i++)
                for(int j = 2; j < SIZE_X-2; j++)
                    if(!grid[i][j].empty()){
                    int nearest = grid[i][j].begin()->second;
                    for(int a : {-2,-1,1,2}) for(int b : {-2,-1,1,2})
                        if(!grid[i+a][j+b].empty())
                           nearest = min(nearest, grid[i+a][j+b].begin()->second);
                    for (pair<Point*,int> v:grid[i][j]){
                            if (v.second-nearest < 6){
                                array<int,3> col = f.getBitValue(j,i);
                                v.first->color={float(col[0]),float(col[1]),float(col[2])};
//                                  v.first->color={0,0,0};
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
