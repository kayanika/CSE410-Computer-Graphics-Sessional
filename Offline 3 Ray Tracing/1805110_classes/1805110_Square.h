#pragma once

#include "1805110_Vector.h"
#include "1805110_Color.h"
#include "1805110_Coefficients.h"
#include "1805110_Object3d.h"
#include "1805110_Triangle.h"
#include <iostream>
using namespace std;

class Square : public Object3d{
    public:
    Vector p, q, r, w;
    Square (Vector a,Vector b,Vector c,Vector m, Color color, Coefficients co, double s) : Object3d((0,0,0), color, s, co) {
        
        p = a;
        q = b;
        r = c;
        w=m;
       // s = m;
       objects.push_back(new Triangle(p,q,w,color,co,s));
         objects.push_back(new Triangle(q,r,w,color,co,s));

       

      

        
    }
    friend ostream& operator<<(ostream& os, const Square& s){
        os<<"##Square##"<<endl;
        os<<"position "<<s.p.x<<" "<<s.p.y<<" "<<s.p.z<<endl;
        os<<"position "<<s.q.x<<" "<<s.q.y<<" "<<s.q.z<<endl;
        os<<"position "<<s.r.x<<" "<<s.r.y<<" "<<s.r.z<<endl;
        os<<"position "<<s.w.x<<" "<<s.w.y<<" "<<s.w.z<<endl;
        os<<"coefficients "<<s.co.ambient<<" "<<s.co.diffuse<<" "<<s.co.specular<<" "<<s.co.reflection<<endl;
        os<<"shininess "<<s.shininess<<endl;
        return os;
    }
    

    void draw() {
        
       
    }

    

    void print() {
       
    }

    ~Square(){
        cout<<"square destructor"<<endl;
    }
    void intersect(){

    }

};