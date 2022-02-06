#include <math.h>
#include <tuple>
#define SMOOTH_COEFICIENT 100
using namespace std;

struct position{float x,y,z;};

struct color{int r, g, b;};

struct vect{
    float x,y,z;
    float size(){
        return sqrt(pow(x,2)+pow(y,2)+pow(z,2));
    }
    vect times(vect w){
        vect result;
        result.x = y*w.z-w.y*z;
        result.y = z*w.x-w.z*x;
        result.z = x*w.y-w.x*y;
        return result;
    }
};




class Point{
    private:
        position coordinates;
        position smoothVector = {0,0,0};
        int photos2color = 0;
        vect neighbourOne={0,0,0};
        vect neighbourTwo={0,0,0};
        vect normVekt;
    public:
        position color = {135,95,15};
        Point(float x, float y, float z){
            coordinates = {x,y,z};
        }

        position getPosition () {return coordinates;}

        float distanceTo(Point* i){
            return sqrt(pow(coordinates.x-i->coordinates.x,2)+pow(coordinates.y-i->coordinates.y,2)+pow(coordinates.z-i->coordinates.z,2));
        }        

        position rotate(position pc, float angleX, float angleZ){
            angleX *= M_PI/180;
            angleZ *= M_PI/180;
            position trnsfrm;
            // around z
            trnsfrm.x = (coordinates.x-pc.x)*cos(angleZ) + (coordinates.y-pc.y)*sin(angleZ) + pc.x;
            trnsfrm.y = (pc.x - coordinates.x)*sin(angleZ) + (coordinates.y-pc.y)*cos(angleZ) + pc.y;
                        
            // around x
            trnsfrm.z = (coordinates.z-pc.z)*cos(angleX) + (trnsfrm.y-pc.y)*sin(angleX) + pc.z;
            trnsfrm.y = (pc.z-coordinates.z)*sin(angleX) + (trnsfrm.y-pc.y) * cos(angleX) + pc.y;
            
            return trnsfrm;

        }

        pair<int,int> project(position pc, position cam, position size, float angleX, float angleZ){
            position pos=rotate(pc,angleX,angleZ);
            pos.x = cam.x +(cam.y/(cam.y-pos.y)*(pos.x-cam.x));
            pos.z = cam.z +(cam.y/(cam.y-pos.y)*(pos.z-cam.z));
            

            pos.x += size.x/2-cam.x;
            pos.z += size.z/2-cam.z;
            return {pos.x, pos.z};
        }

        void addSmoothVector(position v){ /// v neni vektor, ale koncovy bod vektoru, pocatecni je volany bod!!!
            smoothVector.x += v.x-coordinates.x;
            smoothVector.y += v.y-coordinates.y;
            smoothVector.z += v.z-coordinates.z;
        }


        void add2AverageColor(position col){
            color.x += col.x;
            color.y += col.y;
            color.z += col.z;
            photos2color++;
        }
        void setAverageColor(){
            if(photos2color==0){
                color.x = 0; color.y = 0; color.z = 0;
                return;
            }
            color.x = int(color.x/photos2color);
            color.y = int(color.y/photos2color);
            color.z = int(color.z/photos2color);
        }

        void moveBySmoothVector(){
            coordinates.x += smoothVector.x/SMOOTH_COEFICIENT;
            coordinates.y += smoothVector.y/SMOOTH_COEFICIENT;
            coordinates.z += smoothVector.z/SMOOTH_COEFICIENT;
        }
        void resetSmoothVector(){
            smoothVector = {0,0,0};
        }

        void setNeighbours(Point* neighbour){
            vect newVector;
            newVector.x = neighbour->coordinates.x-coordinates.x;
            newVector.y = neighbour->coordinates.y-coordinates.y;
            newVector.z =  neighbour->coordinates.z-coordinates.z;
            //cout << newVector.x << " " << newVector.y << " " << newVector.z << endl;
            if (newVector.size()>neighbourTwo.size()) swap(newVector, neighbourTwo);
            if (neighbourTwo.size()>neighbourOne.size()) swap(neighbourOne, neighbourTwo);

        }

        
        position endOfNormVect(){ 
//            if (neighbourOne.x != 0 && neighbourOne.y !=0 && neighbourOne.z !=0){
 //           cout << neighbourOne.x << " " << neighbourOne.y << " " << neighbourOne.z<<endl;}
  //          cout << neighbourTwo.x << " " << neighbourTwo.y << " " << neighbourTwo.z << endl;
            normVekt = neighbourOne.times(neighbourTwo);

            return {coordinates.x+normVekt.x, coordinates.y+normVekt.y, coordinates.z+normVekt.z};
        }

        void reverseNormVect(){
            normVekt.x *= -1; normVekt.y *= -1; normVekt.z *= -1;
        }
       

};



