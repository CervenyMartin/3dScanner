#include<vector>
#include<fstream>
#include<istream>
#include<map>
#include<tuple>
using namespace std;
ofstream fout("points.obj");

class Mesh{
    private:
    public:
        map<tuple<int,int,int>, int> mp;
        vector<Point*> vertex;
        vector<int> faces;
        Mesh(vector<tuple<int,int, int> >& src){
            int currentIndex = 1;
            for(tuple<int,int,int> p: src){
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