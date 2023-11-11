#pragma once

#include <cmath>
#include <iostream>
#define INF 1e9

using namespace std;
class Vector
{
    public:
    double x, y, z;
    Vector(double a = 0, double b = 0, double c = 0)
    {
        x = a;
        y = b;
        z = c;
    }
 //COPY CONSTRUCTOR
    Vector(const Vector& v)
    {
        x = v.x;
        y = v.y;
        z = v.z;
    }
    Vector operator+(const Vector& v) const
    {
        return Vector(x + v.x, y + v.y, z + v.z);
    }

    Vector operator-(const Vector& v) const
    {
        return Vector(x - v.x, y - v.y, z - v.z);
    }

    Vector operator*(double m) const
    {
        return Vector(x * m, y * m, z * m);
    }

    Vector operator/(double m) const
    {
        return Vector(x / m, y / m, z / m);
    }

    double length() const
    {
        return sqrt(x * x + y * y + z * z);
    }
    double getDistanceBetweenPoints(Vector v){
        return sqrt((x-v.x)*(x-v.x)+(y-v.y)*(y-v.y)+(z-v.z)*(z-v.z));
    }

    

    void normalize() 
    {
       //thikmoto code lekh
        double l=length();
        x=x/l;
        y=y/l;
        z=z/l;
    }

    Vector cross(const Vector& v) 
    {
        return Vector(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
    }

    double dot(const Vector& v) 
    {
        return x * v.x + y * v.y + z * v.z;
    }

    friend ostream& operator<<(ostream& os, const Vector& v)
    {
        os << v.x << " " << v.y << " " << v.z;
        return os;
    }

    friend istream& operator>>(istream& is, Vector& v)
    {
        is >> v.x >> v.y >> v.z;
        return is;
    }
};