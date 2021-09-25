#include"points.h"
#include"foto.h"
#include<vector>
#include<fstream>
#include<istream>
using namespace std;
ofstream fout("points.obj");
class Cloud{
    private:
        vector<Point> pc;
        int sizeX, sizeY, sizeZ;
        int centerX, centerY, centerZ;
    public:
        Cloud(int x, int y, int z){
            for(int a = 0; a < x; a++)
                for(int b = 0; b < y; b++)
                    for(int c = 0; c < z; c++)
                        pc.push_back(Point(a,b,c));
            
            centerX = int(x/2);
            centerY = int(y/2);
            centerZ = int(z/2);

            sizeX = x; sizeY = y; sizeZ = z;
        }

        void crop(string picture, float angleY, float angleX){
            Foto img(picture);
            Point q(0,0,0);
            for(Point& p : pc){
                q = p.rotatedY(angleY, centerX, centerZ);
                q = q.rotatedX(angleX, centerY, centerZ);
                if(!img.getBitValue(int(q.x),int(q.y)))
                    p.setPointValue(0);
            }
            
        }
        int getID(int x, int y, int z){
            int id = 0;
            id += x*sizeY*sizeZ;
            id += y*sizeZ;
            id += z;
            return id;
        }
        Point& getPoint(int id){
            return pc[id];
        }
        
        void write(){
            float rx=45;
            for (Point p : pc){
                if(p.getPointValue())
                    fout << "v " << p.x << " " << p.y << " " << p.z << endl;
            }
        }
};