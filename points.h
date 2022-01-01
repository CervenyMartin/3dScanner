#include <math.h>
#include <tuple>
#define SMOOTH_COEFICIENT 100
using namespace std;

struct position{ float x,y,z;};



class Point{
    private:
        bool active = 1;
        position coordinates;
        position smoothVector = {0,0,0};
        
    public:
        position color = {135,95,15};
        Point(float x, float y, float z){
            coordinates = {x,y,z};
        }

        position getPosition () {return coordinates;}
        

        const pair<int,int> project(position pc, position cam, float angleX, float angleY){
            angleX *= M_PI/180;
            angleY *= M_PI/180;
            position trnsfrm;
            // around y 
            trnsfrm.x = (coordinates.x-pc.x)*cos(angleY) + (coordinates.z-pc.z)*sin(angleY) + pc.x;
            trnsfrm.z = (pc.x - coordinates.x)*sin(angleY) + (coordinates.z-pc.z)*cos(angleY) + pc.z;
                        
            // around x
            trnsfrm.y = (coordinates.y-pc.y)*cos(angleX) + (trnsfrm.z-pc.z)*sin(angleX) + pc.y;
            trnsfrm.z = (pc.y-coordinates.y)*sin(angleX) + (trnsfrm.z-pc.z) * cos(angleX) + pc.z;
            
            trnsfrm.x = cam.x -(cam.z/(cam.z-trnsfrm.z)*(cam.x-trnsfrm.x));
            trnsfrm.y = cam.y -(cam.z/(cam.z-trnsfrm.z)*(cam.y-trnsfrm.y));



            return {trnsfrm.x, trnsfrm.y};
        }

        void addSmoothVector(position v){ /// v neni vektor, ale koncovy bod vektoru, pocatecni je volany bod!!!
            smoothVector.x += v.x-coordinates.x;
            smoothVector.y += v.y-coordinates.y;
            smoothVector.z += v.z-coordinates.z;
        }

        void moveBySmoothVector(){
            coordinates.x += smoothVector.x/SMOOTH_COEFICIENT;
            coordinates.y += smoothVector.y/SMOOTH_COEFICIENT;
            coordinates.z += smoothVector.z/SMOOTH_COEFICIENT;
        }
        void resetSmoothVector(){
            smoothVector = {0,0,0};
        }

};



