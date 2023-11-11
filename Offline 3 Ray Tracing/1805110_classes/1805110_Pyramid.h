#pragma once

#include "1805110_Vector.h"
#include "1805110_Color.h"
#include "1805110_Coefficients.h"
#include "1805110_Object3d.h"
#include "1805110_Triangle.h"
#include <iostream>
using namespace std;

class Pyramid : public Object3d{
    public:
    double width;
    double height;
    
    Pyramid (Vector position, double widthP,double heightP, Color color, Coefficients co, double s) : Object3d(position, color, s, co) {
     width=widthP;
        height=heightP;
         Vector v1 = Vector(position.x,position.y,position.z);
        Vector v2 = Vector(position.x+width,position.y,position.z);
        Vector v3 = Vector(position.x+width,position.y+width,position.z);
        Vector v4 = Vector(position.x,position.y+width,position.z);
        Vector v5 = Vector(position.x+width/2,position.y+width/2,position.z+height);
        //the square made up with two traingles
        innerObjects.push_back(new Triangle(v1,v2,v3,color,co,s)); //square 1
        innerObjects.push_back(new Triangle(v1,v3,v4,color,co,s)); //square 2
        innerObjects.push_back(new Triangle(v1,v2,v5,color,co,s)); //square 3
        innerObjects.push_back(new Triangle(v2,v3,v5,color,co,s)); //square 4
        innerObjects.push_back(new Triangle(v3,v4,v5,color,co,s)); //square 5
        innerObjects.push_back(new Triangle(v4,v1,v5,color,co,s)); //square 6
        

        
    }
    friend ostream& operator<<(ostream& os, const Pyramid& s){
       os<<"##Pyramid##"<<endl;
        
        os<<"position "<<s.position.x<<" "<<s.position.y<<" "<<s.position.z<<endl;
        os<<"width "<<s.width<<endl;
        os<<"height "<<s.height<<endl;
        os<<"coefficients "<<s.co.ambient<<" "<<s.co.diffuse<<" "<<s.co.specular<<" "<<s.co.reflection<<endl;
        os<<"shininess "<<s.shininess<<endl;
        return os;
    }
    friend istream& operator>>(istream& is, Pyramid& s){
      
        is>>s.position.x>>s.position.y>>s.position.z;
         is>>s.width>>s.height;
         is>>s.color.r>>s.color.g>>s.color.b;
        is>>s.co.ambient>>s.co.diffuse>>s.co.specular>>s.co.reflection;
        is>>s.shininess;
        return is;
    }


    void draw() {
        //write the code for drawing a pyramid of width and height and the position as bottom left corner
       for(Object3d* o: innerObjects){
        o->draw();
       }
       
    }
    

    void print() {
       
    }

    ~Pyramid(){
        cout<<"Pyramid destructor"<<endl;
    }
    double intersect(Ray ray, Color& color,int level) {
        //write the code damn it
       double t=INF;
         for(Object3d* o: innerObjects){
              double t1=o->intersect(ray,color,level);
              if(t1>0 && t1<t){
                
                t=t1;
              }
         }
return t;
    }

};