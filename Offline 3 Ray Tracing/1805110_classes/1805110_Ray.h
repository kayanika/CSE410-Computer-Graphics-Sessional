#pragma once

#include "1805110_Vector.h"
#include "1805110_Color.h"
class Ray{
    public:
    Vector origin, dir;
    
    Ray(Vector origin, Vector dir){
        this->origin = origin;
        dir.normalize();
        this->dir = dir;
    }

    // stream
    friend ostream& operator<<(ostream &out, Ray r)
    {
        out << "Origin : " << r.origin << ", Direction : " << r.dir;
        return out;
    }
};