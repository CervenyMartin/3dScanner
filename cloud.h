#include"points.h"
#include"foto.h"
#include<vector>
#include<fstream>
#include<istream>
#include<tuple>
#include<map>
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

*/

const vector<tuple<int, int, int> >  SITES_INDEXES = { {0,-1,0}, {0,1,0}, {1,0,0}, {-1,0,0}, {0,0,-1}, {0,0,1} };
const vector<vector<tuple<float, float, float> > > SITES_VERTES={
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
        int centerX, centerY, centerZ;
    public:
        vector<Point> pc;
        vector<tuple<float, float, float> > vertex;
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

        int coord2index(int X, int Y, int Z){
            return X*sizeY*sizeZ + Y*sizeZ + Z;
        }


        void findFaces(){
            int neighbourIndex;
            for(Point p : pc)
                if(p.getPointValue())
                    for(int i = 0; i < 6; i++){
                            neighbourIndex = coord2index(p.x+get<0>(SITES_INDEXES[i]), p.y+get<1>(SITES_INDEXES[i]), p.z+get<2>(SITES_INDEXES[i]));
                            if(!pc[neighbourIndex].getPointValue()){
                                for (tuple<float, float, float> bod : SITES_VERTES[i]){
                                    vertex.push_back({get<0>(bod)+p.x, get<1>(bod)+p.y, get<2>(bod)+p.z});
                                }
                            }
                        
                        
                    }


        }

        /*void write(){
            float rx=45;
            for (Point p : pc){
                if(p.getPointValue())
                    fout << "v " << p.x << " " << p.y << " " << p.z << endl;
            }
        }*/
};


class Mesh{
    public:
        map<tuple<float,float,float>, int> mp;
        vector<Point*> vertex;
        vector<int> faces;
        Mesh(vector<tuple<float,float, float> >& src){
            int currentIndex = 1;
            for(tuple<float,float,float> p: src){
                if(mp.find(p)==mp.end()){
                    mp.insert({p, currentIndex});
                    vertex.push_back(new Point(get<0>(p),get<1>(p), get<2>(p)));
                    currentIndex++;
                }
                faces.push_back(mp.find(p)->second);
            }
        }

        void writeMesh(){
            for(Point* p : vertex)
                fout << "v " << p->x << " " << p->y << " " << p->z << endl;
            for(int i = 0; i < faces.size(); i+=4){
                fout << "f";
                for(int j = 0; j < 4; j++)
                    fout << " " << faces[i+j];
                fout << endl;
            }
        }

};