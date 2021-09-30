#include <math.h>
#include <tuple>
using namespace std;
class Point{
    private:
        bool active = 1;
        tuple<float, float, float> smoothVector = {0,0,0};
    public:
        float x, y, z;
        Point(float a, float b, float c){
            x = a; y = b; z = c;
        }
        bool getPointValue(){
            return active;
        }
        void setPointValue(bool val){
            active = val;
        }
        Point rotatedY(float alpha, float axisX, float axisZ){
            float newX, newZ;
            alpha /= 180; alpha *= M_PI;
            newX = (x-axisX) *  cos(alpha) + (z-axisZ) * sin(alpha);
            newZ = -(x-axisX) * sin(alpha) + (z-axisZ) * cos(alpha);
            return Point(newX+axisX, y, newZ+axisZ);
        }
        Point rotatedX(float alpha, float axisY, float axisZ){
            float newY, newZ;
            alpha /= 180; alpha *= M_PI;
            newY = (y-axisY) *  cos(alpha) + (z-axisZ) * sin(alpha);
            newZ = -(y-axisY) * sin(alpha) + (z-axisZ) * cos(alpha);
            return Point(x, newY+axisY, newZ+axisZ);
        }
        void addSmoothVector(tuple<float,float,float> v){ /// v neni vektor, ale koncovy bod vektoru, pocatecni je volany bod!!!
            get<0>(smoothVector) += get<0>(v)-x;
            get<1>(smoothVector) += get<1>(v)-y;
            get<2>(smoothVector) += get<2>(v)-z;
        }

        void moveBySmoothVector(){
            x += get<0>(smoothVector)/100;
            y += get<1>(smoothVector)/100;
            z += get<2>(smoothVector)/100;
        }
        void inicializeSmoothVector(){
            smoothVector = {0,0,0};
        }

};

