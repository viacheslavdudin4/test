// g++ main.c -frtti
// https://github.com/viacheslavdudin4/test.git
#include <iostream>
#include <math.h>
#include <map>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <typeinfo>
using namespace std;
/* Base class for any 3d curve or line

*/
typedef long double Real;
class Point3D {
    public:
        Point3D() {x=0;y=0;z=0;}
        Point3D(Real x_, Real y_, Real z_):x(x_),y(y_),z(z_){}
        Point3D operator=(const Point3D & Sample){x=Sample.x;y=Sample.y;z=Sample.z;return *this;}
        Real x;Real y;Real z;
};
std::ostream& operator << (std::ostream &os, const Point3D &p) {
    return os << "{" << p.x << ","<< p.y << "," << p.z << "}";
}
// ..............................................................
class Curve3D:public Point3D {
    public:
        Curve3D() {}
        Curve3D(Real x_, Real y_, Real z_):Point3D(x_,y_,z_){}
        virtual Point3D GetPoint3D(Real t) const {
            return Point3D(x,y,z);
        }
        virtual Point3D GetDerivative3D(Real t) const = 0;
        virtual int GetType() = 0;
};
// ..............................................................
class Circle3D: public Curve3D {
    public:
        Circle3D():Curve3D(){}
        Circle3D(Real x_, Real y_, Real z_, Real R_):Curve3D(x_,y_,z_),R(R_) {}
        virtual Point3D GetPoint3D(Real t) const {
            Point3D Result(sin(t)*R,cos(t)*R,z);
            return Result;
        }
        virtual Point3D GetDerivative3D(Real t) const {
            Point3D Result(cos(t)*R,-sin(t)*R,0);
            return Result;
        }
        virtual int GetType() {return 0;}
    public:
        Real R;
};
// ..............................................................
class Ellipse3D: public Circle3D {
    public:
        Ellipse3D():Circle3D(){}
        Ellipse3D(Real x_, Real y_, Real z_, Real R_, Real r_):Circle3D(x_,y_,z_,R_),r(r_){};
        virtual Point3D GetPoint3D(Real t) const {
            Point3D Result(sin(t)*R,cos(t)*r,z);
            return Result;
        }
        virtual Point3D GetDerivative3D(Real t) const {
            Point3D Result(cos(t)*R,-sin(t)*r,0);
            return Result;
        }
        virtual int GetType() {return 1;}
    public:
        Real r;
};
// ..............................................................
class Helix3D: public Circle3D {
    public:
        Helix3D():Circle3D() {}
        Helix3D(Real x_, Real y_, Real z_, Real R_, Real Step_):Circle3D(x_,y_,z_,R_),Step(Step_) {}
        virtual Point3D GetPoint3D(Real t) const {
            Point3D Result(sin(t)*R,cos(t)*R,z+t*Step);
            return Result;
        }
        virtual Point3D GetDerivative3D(Real t) const {
            Point3D Result(cos(t)*R,-sin(t)*R,Step);
            return Result;
        }
        virtual int GetType() {return 2;}
    public:
        Real Step;
};
// ..............................................................
vector<Curve3D*>RandomObjects;
int Count;
//multimap<Real,Circle3D*> Index;
multimap<Real,Circle3D*,std::less<>> Index;
void SortCircles(Curve3D * Obj) {
    if (Obj->GetType()==0) {
        Index.insert({reinterpret_cast<Circle3D*>(Obj)->R,reinterpret_cast<Circle3D*>(Obj)});
    }
}
// ..............................................................
int main() {
    Count = 0;
    //multiset<Circle3D> Circles;
    //std::map<Curve3D*> Index;
    // Create an objects in random manner
    for (int i=0;i<30;++i) {
        Curve3D* Obj;
        int ObjectType = rand(); //RAND_MAX
        if (ObjectType>RAND_MAX/3*2) {
            Obj = new Helix3D(rand(),rand(),rand(),rand(),rand());
        }else {
            if (ObjectType>RAND_MAX/3) {
                Obj = new Ellipse3D(rand(),rand(),rand(),rand(),rand());
            }else {
                Obj = new Circle3D(rand(),rand(),rand(),rand());
            }
        }
        RandomObjects.push_back(Obj);
    }
    // Print coordinates of points and derivatives of all curves in the container at t=PI/4
    vector<Curve3D*>::iterator c; vector<Curve3D*>::iterator ce;
    c = RandomObjects.begin(); ce = RandomObjects.end();
    Point3D p;
    while(c!=ce) {
        Curve3D* obj = *c;
        p = obj->GetPoint3D(M_PI/4); cout << p << "\t";
        p = obj->GetDerivative3D(M_PI/4); cout << p << "\r\n";
        ++c;
    }
    // Print out all the circles
    // Sort circles
    for_each(RandomObjects.begin(),RandomObjects.end(),SortCircles);
    multimap<Real,Circle3D*,std::greater<>>::iterator iterator;
    multimap<Real,Circle3D*,std::greater<>>::iterator iteratorEnd;
    iterator = Index.begin();
    iteratorEnd = Index.end();
    Real Sum = 0;
    while(iterator!=iteratorEnd) {
        cout<<"Radius of circle:"<<iterator->first<<endl;
        Sum+=iterator->first;
        ++iterator;
    }
    cout<<"The sum of radii "<<Sum<<endl;
    return 0;
}
