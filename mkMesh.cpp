#include<iostream>
#include<fstream>
#include<math.h>
#include<vector>
#include<string>
#include<queue>
#include<algorithm>

using namespace std;
#define FOTO_X 234
#define FOTO_Y 300
#define PI 3.14159
#define STEP 4

ofstream foutstl ("points.stl");
ofstream foutobj ("points.obj");
class Point{
    public:
        int x, y, z;
        void rotateY(float alpha, int axisX, int axisZ){
            int oldX = x-axisX; int oldZ = z-axisZ;
            alpha /= 180; alpha *= PI;
            x = nearbyint(oldX *  cos(alpha) + oldZ * sin(alpha));
            z = nearbyint(-oldX * sin(alpha) + oldZ * cos(alpha));
            x += axisX; z += axisZ;
        }

        void rotateX(float alpha, int axisY, int axisZ){
            int oldZ = z-axisZ; int oldY= y-axisY;
            alpha /= 180; alpha *= PI;
            z = nearbyint(oldZ * cos(alpha) + oldY * sin(alpha));
            y = nearbyint(-oldZ * sin(alpha) + oldY * cos(alpha));
            y += axisY; z += axisZ;
        }

        Point(int X, int Y, int Z){
            x = X; y = Y; z = Z;
        }

        bool equals(Point p){
            if(p.x == x && p.y == y && p.z == z)
                return true;
            return false;
        }

        int distance2(Point p){
            return pow(x - p.x, 2) + pow(y - p.y, 2) + pow(z - p.z,2);
        }

 

        
};

class PointCloud{
    private:
        vector<vector<vector<bool> > > cloud;
        vector<vector<bool> > img;
        int sizeX, sizeY, sizeZ;
        int centerX, centerY, centerZ;
        
    public:
        PointCloud(int x, int y, int z){
            sizeX=x; sizeY=y; sizeZ=z;
            centerX = (sizeX+1)/2; centerY = (sizeY+1)/2; centerZ = (sizeZ+1)/2;
            cloud.resize(sizeX);
            for(vector<vector<bool> >& X : cloud){          //resizing cloud
                X.resize(sizeY);
                for(vector<bool>& Y : X)
                    Y.resize(sizeZ, 1);    
            }
          
            img.resize(FOTO_Y);                             //resizing canvas
            for(vector<bool>& r : img) r.resize(FOTO_X); 
        }

        bool getPointValue(int x, int y, int z){
            return cloud[x][y][z];
        }

        void setPointValue(int x, int y, int z, bool val){
            cloud[x][y][z] = val;
        }

        void writePoints(){
            for(int y = 0; y < sizeY; y++)
                for(int z = 0; z < sizeZ; z++)
                    for (int x = 0; x < sizeX; x++)
                        if (getPointValue(x, y, z))
                            foutobj << "v " << x << " " << y << " " << z << endl;
                    
        }
        void loadImg(string picture){
            ifstream fin(picture);
            string foto=""; fin >> foto;            
            for (int i = 0; i < FOTO_Y; i++)
                for(int j = 0; j < FOTO_X; j++){
                    if (foto[i*FOTO_X+j]=='0') img[i][j]=0;
                    else img[i][j]=1;
                
                }   
        }
        void crop(float alphaX, float alphaY, string picture){
            Point p(0,0,0);
            loadImg(picture);
            for(int y = 0; y < sizeY; y++)
                for(int z = 0; z < sizeZ; z++)
                    for (int x = 0; x < sizeX; x++){
                        p.x = x; p.y = y; p.z = z;
                        p.rotateY(alphaY, centerX, centerZ);
                        p.rotateX(alphaX, centerY, centerZ);
                        //p.x -= int((sizeX-FOTO_X)/2);   // asi zarovnani na stred ale neovereno
                        //p.y -= int((sizeY-FOTO_Y)/2);
                        if (p.y < 0 || p.y > img.size()-1) setPointValue(x,y,z,0);
                        else if (p.x < 0 || p.x > img[0].size()-1) setPointValue(x,y,z,0);
                        else if(img[FOTO_Y-p.y-1][p.x]==1) setPointValue(x, y, z, 0);
                        
                    }
        }
        
        vector<Point> findNeighbours(Point p){
            Point q(p.x,p.y,p.z);
            vector<Point> neighbours;
            for(int i : {0, 1, -1})
                for(int j : {0, 1, -1})
                    for(int k : {0, 1, -1}){
                        if((i==0 && j == 0 && k == 0)) continue;
                        q.x = p.x + i;
                        q.y = p.y + j;
                        q.z = p.z + k;
                        if (0 <= q.x < sizeX && 0 <= q.y < sizeY && 0 <= q.z < sizeZ)
                            if (getPointValue(q.x, q.y, q.z)){
                                neighbours.push_back(q);
                                
                            }
                            
                    }
            return neighbours;
        }

        bool isInner(Point p){
            if(min(min(p.x, p.y), p.z)==0) return false;
            if(p.x+1 == sizeX || p.y+1 ==sizeY ||  p.z+1 == sizeZ) return false;
            if (!getPointValue(p.x, p.y, p.z + 1)) return false;
            if (!getPointValue(p.x, p.y, p.z - 1)) return false;
            if (!getPointValue(p.x, p.y + 1, p.z)) return false;
            if (!getPointValue(p.x, p.y - 1, p.z)) return false;
            if (!getPointValue(p.x + 1, p.y, p.z)) return false;
            if (!getPointValue(p.x - 1, p.y, p.z)) return false;
            return true;
        }


        void smooth(){
            vector<Point> toBeRemoved;
            Point p(0,0,0);
            for(int y = 0; y < sizeY; y++)
                for(int z = 0; z < sizeZ; z++)
                    for (int x = 0; x < sizeX; x++){
                        p.x = x; p.y = y; p.z = z;
                        if(!isInner(p)){
                            toBeRemoved.push_back(p);
                        }
            }
            for (Point q : toBeRemoved)
                setPointValue(q.x, q.y, q.z, 0);
        }

        

        void removeInnerPoints(){
            vector<Point> toBeRemoved;
            Point p(0,0,0);
            for(int y = 0; y < sizeY; y++)
                for(int z = 0; z < sizeZ; z++)
                    for (int x = 0; x < sizeX; x++){
                        p.x = x; p.y = y; p.z = z;
                        if(isInner(p)){

                            toBeRemoved.push_back(p);
                        }
            }
            for (Point q : toBeRemoved)
                setPointValue(q.x, q.y, q.z, 0);

        }

        


        
    void writeTriangle(float ax, float ay, float az, float bx, float by, float bz, float cx, float cy, float cz){
        foutstl << "facet" << endl;
        foutstl << "outer loop" << endl;
        foutstl << "vertex " << ax << " " << ay << " " << az << endl;
        foutstl << "vertex " << bx << " " << by << " " << bz << endl;
        foutstl << "vertex " << cx << " " << cy << " " << cz << endl;
        foutstl << "endloop" << endl;
        foutstl << "endfacet" << endl;
    } 
    void writeSite(Point p, Point q){
        if(abs(q.x)==1){
            writeTriangle(p.x+q.x*0.5, p.y+0.5, p.z-0.5, p.x+q.x*0.5, p.y-0.5, p.z-0.5, p.x+q.x*0.5, p.y+0.5, p.z+0.5);
            writeTriangle(p.x+q.x*0.5, p.y-0.5, p.z+0.5, p.x+q.x*0.5, p.y-0.5, p.z-0.5, p.x+q.x*0.5, p.y+0.5, p.z+0.5);
        }
        if(abs(q.y)==1){
            writeTriangle(p.x-0.5, p.y+q.y*0.5, p.z-0.5, p.x+0.5, p.y+q.y*0.5, p.z-0.5, p.x+0.5, p.y+q.y*0.5, p.z+0.5);
            writeTriangle(p.x-0.5, p.y+q.y*0.5, p.z-0.5, p.x-0.5, p.y+q.y*0.5, p.z+0.5, p.x+0.5, p.y+q.y*0.5, p.z+0.5);
        }
        if(abs(q.z)==1){
            writeTriangle(p.x-0.5, p.y-0.5, p.z+q.z*0.5, p.x+0.5, p.y-0.5, p.z+q.z*0.5,p.x+0.5, p.y+0.5, p.z+q.z*0.5);
            writeTriangle(p.x-0.5, p.y-0.5, p.z+q.z*0.5, p.x-0.5, p.y+0.5, p.z+q.z*0.5,p.x+0.5, p.y+0.5, p.z+q.z*0.5);
        }
        
    }

    void writeCube(Point p){
            for(Point i : {Point(0, 0, 1), Point(0, 0, -1), Point(0, 1, 0), Point(0,-1,0), Point(1, 0, 0), Point(-1, 0,0)}){
                    if(!getPointValue(p.x+i.x, p.y+i.y, p.z+i.z) && getPointValue(p.x,p.y,p.z))
                        writeSite(p, i);
                }
        
    }

    void makeModelOfCubes(){
        for(int y = 1; y < sizeY-1; y++)
                for(int z = 1; z < sizeZ-1; z++)
                    for (int x = 1; x < sizeX-1; x++)
                        writeCube(Point(x, y, z));
                
    }

      

        
};

int main(){
   
   PointCloud pc(FOTO_X, FOTO_Y, FOTO_X);
   
   for(int a =20; a < 61; a+=200){
        for(int i = 0; i < 100; i+=STEP){
        
            pc.crop(-a,3.6*i,"./imgs/pbms/" + to_string(a) + "/" + to_string(i) + ".pbm");
            cout << a << ": " << i << " done" << endl;
        }
   }

    pc.smooth();
    pc.makeModelOfCubes();
    pc.removeInnerPoints();
    pc.writePoints();
    cout << "done" << endl;
    

    return 0;
}