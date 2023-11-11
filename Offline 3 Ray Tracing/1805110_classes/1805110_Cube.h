#pragma once

#include "1805110_Vector.h"
#include "1805110_Color.h"
#include "1805110_Coefficients.h"
#include "1805110_Object3d.h"
#include <iostream>
using namespace std;

class Cube : public Object3d{
    public:
    double side;
    
    Cube (Vector position, double width, Color color, Coefficients co, double s) : Object3d(position, color, s, co) {
      side=width;
       Vector v1 = Vector(position.x,position.y,position.z);
        Vector v2 = Vector(position.x+side,position.y,position.z);
        Vector v3 = Vector(position.x+side,position.y+side,position.z);
        Vector v4 = Vector(position.x,position.y+side,position.z);
        Vector v5 = Vector(position.x,position.y,position.z+side);
        Vector v6 = Vector(position.x+side,position.y,position.z+side);
        Vector v7 = Vector(position.x+side,position.y+side,position.z+side);
        Vector v8 = Vector(position.x,position.y+side,position.z+side);
        
        //push back each square object with two triangle object
        //assign random colors to every traingle
       
        innerObjects.push_back(new Triangle(v1,v2,v3,color,co,s)); //1
        innerObjects.push_back(new Triangle(v1,v3,v4,color,co,s)); //2
        innerObjects.push_back(new Triangle(v1,v2,v6,color,co,s)); //3
        innerObjects.push_back(new Triangle(v1,v6,v5,color,co,s)); //4
        innerObjects.push_back(new Triangle(v2,v3,v7,color,co,s)); //5
        innerObjects.push_back(new Triangle(v2,v7,v6,color,co,s)); //6
        innerObjects.push_back(new Triangle(v3,v4,v8,color,co,s)); //7
        innerObjects.push_back(new Triangle(v3,v8,v7,color,co,s)); //8
        innerObjects.push_back(new Triangle(v4,v1,v5,color,co,s)); //9
        innerObjects.push_back(new Triangle(v4,v5,v8,color,co,s)); //10
        innerObjects.push_back(new Triangle(v5,v6,v7,color,co,s)); //11
        innerObjects.push_back(new Triangle(v5,v7,v8,color,co,s)); //12




    }
    friend ostream& operator<<(ostream& os, const Cube& s){
        os<<"##Cube##"<<endl;
        os<<"width "<<s.side<<endl;
        os<<"position "<<s.position.x<<" "<<s.position.y<<" "<<s.position.z<<endl;
        os<<"coefficients "<<s.co.ambient<<" "<<s.co.diffuse<<" "<<s.co.specular<<" "<<s.co.reflection<<endl;
        os<<"shininess "<<s.shininess<<endl;
        
        return os;
    }


    void draw() {
        //write the code for drawing a cube of side length side and the position as bottom left corner
      for(Object3d* o: innerObjects){
           o->draw();
      }
        

    }
    

    void print() {
        cout<<"##Cube##"<<endl;
        cout<<"width "<<side<<endl;
        cout<<"position "<<position.x<<" "<<position.y<<" "<<position.z<<endl;
        cout<<"coefficients "<<co.ambient<<" "<<co.diffuse<<" "<<co.specular<<" "<<co.reflection<<endl;
        cout<<"shininess "<<shininess<<endl;
        cout<<"ineeObjects "<<innerObjects.size()<<endl;
    }
 double intersect(Ray ray, Color& color,int level)  {
 double t=INF;
         for(Object3d* o: innerObjects){
              double t1=o->intersect(ray,color,level);
              if(t1>0 && t1<t){
                t=t1;
              }
         }
         return t;
         

    
 }
    ~Cube(){
        cout<<"Cube destructor"<<endl;
    }

};