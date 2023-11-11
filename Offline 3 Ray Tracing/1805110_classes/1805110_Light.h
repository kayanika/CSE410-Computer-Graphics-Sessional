#pragma once

#include "1805110_Vector.h"
#include "1805110_Color.h"

#include "1805110_Coefficients.h"
#include <iostream>
using namespace std;

class Light{
    public:
    Vector position;
    
    double falloffParameter;

    Light(Vector pos,double f){
        position=pos;
        falloffParameter=f;
    }
       
    void print(){
        cout<<"position "<<position.x<<" "<<position.y<<" "<<position.z<<endl;
     
        cout<<"falloff "<<falloffParameter<<endl;
    }
    friend ostream& operator<<(ostream& os, const Light& l){
        os<<"position "<<l.position.x<<" "<<l.position.y<<" "<<l.position.z<<endl;
      
        os<<"falloff "<<l.falloffParameter<<endl;
        return os;
    }
    friend istream& operator>>(istream& is, Light& l){
        is>>l.position.x>>l.position.y>>l.position.z;
     
        is>>l.falloffParameter;
        return is;
    }

    void draw(){
 glPointSize(5);
        glBegin(GL_POINTS);
        glColor3f(1.0, 1.0, 1.0);
       glVertex3f(position.x,position.y,position.z);
        glEnd();
    }
    
};



class SpotLightSource{
    public:
    Vector position;
    Color color;
    double fallOffParameter;

    Vector lookDirection;
    double cutOffAngle;
    SpotLightSource(){
       position=Vector(0,0,0);
        fallOffParameter=0;
        lookDirection=Vector(0,0,0);
        cutOffAngle=0;
    }
    
    SpotLightSource( Vector pos, double fallOff, Vector lookDir, double cutOff){
        position=pos;
        fallOffParameter=fallOff;
        lookDirection=lookDir;
        cutOffAngle=cutOff;
    }
    void draw(){
  glPointSize(15);
        glBegin(GL_POINTS);
        glColor3f(1.0, 1.0, 1.0);
       glVertex3f(position.x,position.y,position.z);
        glEnd();
    }
    friend ostream& operator<<(ostream& os, const SpotLightSource& l){
        os<<"position "<<l.position.x<<" "<<l.position.y<<" "<<l.position.z<<endl;
     
        os<<"falloff "<<l.fallOffParameter<<endl;
        os<<"lookDirection "<<l.lookDirection.x<<" "<<l.lookDirection.y<<" "<<l.lookDirection.z<<endl;
        os<<"cutOffAngle "<<l.cutOffAngle<<endl;
        return os;
    }
    friend istream& operator>>(istream& is, SpotLightSource& l){
        is>>l.position.x>>l.position.y>>l.position.z;
     
        is>>l.fallOffParameter;
        is>>l.lookDirection.x>>l.lookDirection.y>>l.lookDirection.z;
        is>>l.cutOffAngle;
        return is;
    }
        
    
};
