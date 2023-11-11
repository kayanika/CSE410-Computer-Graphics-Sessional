#pragma once

#include "1805110_Vector.h"
#include "1805110_Coefficients.h"
#include "1805110_Object3d.h"
#include "1805110_Color.h"
#include "1805110_Light.h"
#include "1805110_Ray.h"
#include <iostream>
#include <bits/stdc++.h>

using namespace std;
Vector pos; // position of the eye
Vector l;   // look/forward direction
Vector r;   // right direction
Vector u;   // up direction

double distanceNear,distanceFar,fieldOfView,aspectRatio,noOfObjects,screenWidth,screenHeight,noOfPixels,levelOfRecursion;



class Object3d {
    public:
    Vector position;
    Color color;
    double shininess;
    Coefficients co;
    vector<Object3d *> innerObjects;

    Object3d(Vector p, Color c, double s, Coefficients co) {
        position = p;
        color = c;
        shininess = s;
        this->co = co;
    }

    virtual void draw() = 0;
    virtual void print() = 0;
    virtual double intersect(Ray ray, Color& color,int level) = 0;
    
    // gimme << operator overload but virtual
    virtual ~Object3d() {
    }
    
    
};
vector<Object3d *> objects;
vector<Light *> lights;
vector<SpotLightSource *> spotlights;