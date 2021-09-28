# include<iostream>
# include"mesh.h"
# include<vector>
# include <map>
# include <tuple>
using namespace std;

int main(){

    vector<tuple<int,int, int> > src = {{1,2,3},{2,3,3}, {4,5,6},{1,2,3},{4,5,6}};
    Mesh m(src);

    for(Point* p : m.vertex){
        cout << p->x << " " << p -> y << " " << p->z << endl;
    }
    for(int i : m.faces)
        cout << i << endl;

/*    vector<Point*> v = {&a, &b, &c, &a};
    Mesh m(v);
    for(int i : m.faces)
        cout << i << endl;
*/
    return 0;
}