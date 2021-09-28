#include <math.h>
class Point{
    private:
        bool active = 1;
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

};

class Tripple{
    public:
        int first, second, third;
        Tripple(int a, int b, int c){
            first = a; second = b; third = c;
        }
        int minimum(){
            return std::min(std::min(first, second), third);
        }
};

class Foursome{
    public:
        int one, two, three, four;
        Foursome(int a, int b, int c, int d){
            one = a; two = b; three = c; four = d;
        }
};