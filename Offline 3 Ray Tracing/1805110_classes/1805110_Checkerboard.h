#pragma once

#include "1805110_Vector.h"
#include "1805110_Coefficients.h"
#include "1805110_Color.h"
#include "1805110_Object3d.h"
#include <iostream>
using namespace std;

class Checkerboard : public Object3d{
    public:
    double width;
    Checkerboard( double width, Coefficients co) : Object3d(Vector(0,0,0), Color(0,0,0), 0, co) {
        this->width = width;
    }

    friend ostream& operator<<(ostream& os, const Checkerboard& c){
        os<<"width "<<c.width<<endl;
        os<<"coefficients "<<c.co.ambient<<" "<<c.co.diffuse<<" "<<c.co.specular<<" "<<c.co.reflection<<endl;
        return os;
    }
    friend istream& operator>>(istream& is, Checkerboard& c){
        is>>c.width;
        is>>c.co.ambient>>c.co.diffuse>>c.co.reflection;
        return is;
    }

    void draw(){
        cout<<"Checkerboard drawinggggg"<<endl;
       
       int tiles= 1000/width;
       Vector v1 = Vector(-tiles*width/2,-tiles*width/2,0);
       for(int i=0;i<tiles;i++){
        for(int j=0;j<tiles;j++){
            if((i+j)%2==0){
                glColor3f(0,0,0);
            }
            else{
                glColor3f(1,1,1);
            }
            glBegin(GL_QUADS);{
                glVertex3f(v1.x+i*width,v1.y+j*width,v1.z);
                glVertex3f(v1.x+i*width,v1.y+(j+1)*width,v1.z);
                glVertex3f(v1.x+(i+1)*width,v1.y+(j+1)*width,v1.z);
                glVertex3f(v1.x+(i+1)*width,v1.y+j*width,v1.z); 
            }
            glEnd();
        }
       }
    }
    void print(){
        cout<<"Checkerboard"<<endl;
        cout<<"width "<<width<<endl;
        cout<<"coefficients "<<co.ambient<<" "<<co.diffuse<<" "<<co.reflection<<endl;
    }
        Color colorAtPoint(Vector pt){
             int tiles= 1000/width;
       Vector v1 = Vector(-tiles*width/2,-tiles*width/2,0);

        int tileX = (pt.x - v1.x) / width;
		int tileY = (pt.y - v1.y) / width;

        if(tileX<0 || tileX>=tiles || tileY<0 || tileY>=tiles){
            return Color(0,0,0);
        }

		if (((tileX + tileY) % 2) == 0)
		{  
			return Color(1,1,1);
		}
		else
		{
          
			return Color(0,0,0);
		}
    }
    double intersect(Ray ray, Color& color,int level){
          Vector normal = Vector(0, 0, 1);
          int tiles= 1000/width;
          Vector v1 = Vector(-tiles*width/2.0,-tiles*width/2.0,0);
        double dotP = normal .dot( ray.dir);
        
        if (round(dotP * 100) == 0)
			return -1;

        double t = -(normal.dot( ray.origin)) / dotP;

        Vector p = ray.origin + ray.dir * t;

        if(p.x <= v1.x || p.x >= abs(v1.x) && p.y <= v1.y && p.y >= abs(v1.y)){
            return -1;
        }
        
        if (normal.dot(ray.dir) > 0)
            normal = normal * -1;
        normal.normalize();

       
        Vector intersectionPoint = ray.origin + ray.dir * t;
        Color colorAtIntersection = this->color;

       
        double phong = 0;
        double lambert = 0;
        if(level==0) return t;
        for (int i = 0; i < lights.size(); i++)
        {

            Vector lightPosition = lights[i]->position;
            Vector lightDirection = intersectionPoint - lightPosition;
            lightDirection.normalize();

            Ray lightRay = Ray(lightPosition, lightDirection);

           

            double t2 = INF;
            double lightDistance=lightPosition.getDistanceBetweenPoints(intersectionPoint);
          

            bool obscured = false;

            for (Object3d *obj : objects)
            { Color c;
                double t3 = obj->intersect(lightRay,c, 0);
                if (t3 > 0.0 && t3  < t2)
                {
                   
                   t2=t3;
                }
            }

            if (t2<lightDistance-1e-5)
            continue;
            
            {
                Vector toSource = lightPosition - intersectionPoint;
                toSource.normalize();
                double distance = intersectionPoint.getDistanceBetweenPoints(lightPosition);
                double scalingFactor = exp(-lights[i]->falloffParameter * distance * distance);
           
                lambert += max(0.0, toSource.dot(normal)) * scalingFactor;
                Vector reflection = lightRay.dir - normal * 2.0 * (lightRay.dir.dot(normal));
                reflection.normalize();
                phong += pow(max(0.0, reflection.dot(ray.dir)), shininess) * scalingFactor;

               
            }
        }

       
        for (int i = 0; i < spotlights.size(); i++)
        {
 
            Vector lightPosition = spotlights[i]->position;
            Vector lightDirection = intersectionPoint - lightPosition;
            lightDirection.normalize();
            Vector v1 = intersectionPoint - lightPosition;
            Vector v2 = spotlights[i]->lookDirection;
            v1.normalize();
            v2.normalize();
            double angle = acos(-v1.dot(v2)) * (180.0 / M_PI);
            if (angle > spotlights[i]->cutOffAngle)
                continue;

           

            Ray lightRay = Ray(lightPosition, lightDirection);
         

            

            double t2 = (intersectionPoint - lightPosition).length();
            if (t2 < 1e-5)
                continue;

            bool obscured = false;

            for (Object3d *obj : objects)
            { Color c;
                double t3 = obj->intersect(lightRay, c, 0);
                if (t3 > 0.0 && t3  < t2)
                { t2=t3;
                    
                }
            }
             double lightDistance=lightPosition.getDistanceBetweenPoints(intersectionPoint);

            if (t2<lightDistance-1e-5)
            continue;
            {
                Vector toSource = lightPosition - intersectionPoint;
                toSource.normalize();
                double distance = intersectionPoint.getDistanceBetweenPoints(lightPosition);
                double scalingFactor = exp(-spotlights[i]->fallOffParameter * distance * distance);
              
                lambert += max(0.0, toSource.dot(normal)) * scalingFactor;
                Vector reflection = lightRay.dir - normal * 2.0 * (lightRay.dir.dot(normal));
                reflection.normalize();
                phong += pow(max(0.0, reflection.dot(ray.dir)), shininess) * scalingFactor;
            }
        }
       
        color = Color(1, 1, 1) * co.specular * phong + this->colorAtPoint(intersectionPoint) * lambert * co.diffuse + this->colorAtPoint(intersectionPoint)* co.ambient;

        if (level < levelOfRecursion)
        {
            
            Ray reflectionRay = Ray(intersectionPoint, ray.dir - normal * 2 * (ray.dir.dot( normal)));

           
            reflectionRay.origin = reflectionRay.origin + reflectionRay.dir * 1e-5;

           

            int in = findClosestObject(reflectionRay, color);
           
            if (in != -1)
            {
               

                Color temp(0, 0, 0); // refelction color
              
                double t = objects[in]->intersect(reflectionRay, temp, level + 1);

                
                color = color + temp * co.reflection;
            }
          

         
        }
        

        color.clip();
       // cout<<color.r<<" "<<color.g<<" "<<color.b<<endl;


        return t;
    }
     bool notVisible(Ray lightRay, double t2){
    bool obscured = false;
    for (Object3d *obj : objects)
    {
        Color c;
        double t3 = obj->intersect(lightRay, c, 0);
        if (t3 > 0 && t3 + 1e-5 < t2)
        {
            obscured = true;
            break;
        }
    }
    return obscured;
}
int findClosestObject(Ray ray, Color& color){
        double t=INF;
        int index=-1;
        for(int i=0;i<objects.size();i++){
            double t1=objects[i]->intersect(ray,color,0);
            if(t1>0 && t1<t){
                t=t1;
                index=i;
            }
        }
        return index;
    }

};
