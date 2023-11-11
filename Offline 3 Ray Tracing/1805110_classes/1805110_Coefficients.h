#pragma once

#include <iostream>

using namespace std;
class Coefficients{
    public :
    double ambient ;
    double diffuse ;
    double specular ;
    double reflection ;
    

    Coefficients(double a = 0,double d = 0,double s = 0,double r = 0){
        ambient=a;
        diffuse=d;
        specular=s;
        reflection=r;
    }

    friend ostream& operator<<(ostream& os, const Coefficients& c){
        os<<c.ambient<<" "<<c.diffuse<<" "<<c.specular<<" "<<c.reflection<<endl;
        return os;
    }
    friend istream& operator>>(istream& is, Coefficients& c){
        is>>c.ambient>>c.diffuse>>c.specular>>c.reflection;
        return is;
    }
};
