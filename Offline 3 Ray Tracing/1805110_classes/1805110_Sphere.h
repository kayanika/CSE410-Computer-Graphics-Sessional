#pragma once

#include "1805110_Vector.h"
#include "1805110_Color.h"
#include "1805110_Coefficients.h"
#include "1805110_Object3d.h"
#include "1805110_Ray.h"
#include <iostream>
using namespace std;

class Sphere : public Object3d{
    public:
    double radius;
    
    Sphere (Vector position, double r, Color color, Coefficients co, double s) : Object3d(position, color, s, co) {
        radius = r;
    }
    friend ostream& operator<<(ostream& os, const Sphere& s){
        os<<"##Sphere##"<<endl;
        os<<"radius "<<s.radius<<endl;
        os<<"position "<<s.position.x<<" "<<s.position.y<<" "<<s.position.z<<endl;
        os<<"coefficients "<<s.co.ambient<<" "<<s.co.diffuse<<" "<<s.co.specular<<" "<<s.co.reflection<<endl;
        os<<"shininess "<<s.shininess<<endl;
        return os;
    }


    void draw() {
        int stacks = 100, slices = 100;
        Vector points[stacks+1][slices+1];
        for (int j = 0; j <= stacks; j++) {
            double phi = -M_PI / 2.0 + j * M_PI / stacks;
            double r = radius * cos(phi);
            double h = radius * sin(phi);
            for (int i = 0; i < slices+1; i++) {
                double theta = i * 2.0 * M_PI / slices;
                points[j][i].x = position.x + r * cos(theta);
                points[j][i].y = position.y + r * sin(theta);
                points[j][i].z = position.z + h;
            }
        }

        glBegin(GL_QUADS);
            for (int j = 0; j < stacks; j++) {
                for (int i = 0; i < slices; i++) {
                    glColor3f(color.r, color.g, color.b);
                    glVertex3f(points[j][i].x, points[j][i].y, points[j][i].z);
                    glVertex3f(points[j][i+1].x, points[j][i+1].y, points[j][i+1].z);

                    glVertex3f(points[j+1][i+1].x, points[j+1][i+1].y, points[j+1][i+1].z);
                    glVertex3f(points[j+1][i].x, points[j+1][i].y, points[j+1][i].z);
                }
            }
        glEnd();
    }
    double intersect(Ray ray, Color &color, int level){
        Vector v=ray.origin-position;
  
            
            double a = 1;
            double b =  (ray.dir.dot(v)* 2.0);
            double c = (v.dot(v)) - (radius*radius);

            

            double discriminant = pow(b, 2) - 4 * a * c;
            double t = -1;
            if (discriminant < 0){
                t = -1;
            }
            else{
                
                if(fabs(a) < 1e-5)
                {
                    t = -c/b;
                    return t;
                }

                double t1 = (-b - sqrt(discriminant)) / (2 * a);
                double t2 = (-b + sqrt(discriminant)) / (2 * a);

                if(t2<t1) swap(t1, t2);

                if (t1 > 0){
                    t = t1;
                }
                else if (t2 > 0){
                    t = t2;
                }
                else{
                    t = -1;
                }
            }
            
           if(t<0) return -1;
           if(level==0) return t;
      
        Vector intersectionPoint = ray.origin + ray.dir * t;
       
        Vector normal=intersectionPoint-position;

        
        double distance=pos.getDistanceBetweenPoints(intersectionPoint);
        if(distance>radius){
            normal=normal*-1;
        }
        normal.normalize();

        
       
        double phong = 0;
        double lambert = 0;
     for (int i = 0; i < lights.size(); i++)
        {

            Vector lightPosition = lights[i]->position;
            Vector lightDirection = intersectionPoint - lightPosition;
            lightDirection.normalize();

           
            Ray lightRay = Ray(lightPosition, lightDirection);

            

            double t2 = (intersectionPoint - lightPosition).length();
            if (t2 < 1e-5)
                continue;

            

            if (!notVisible(lightRay, t2) )
            {
                Vector toSource = lightPosition - intersectionPoint;
                toSource.normalize();
                double distance = intersectionPoint.getDistanceBetweenPoints(lightPosition);
                double scalingFactor = exp(-lights[i]->falloffParameter * distance * distance);
                // cout<<"Scaling Factor "<<scalingFactor<<endl;
                lambert += max(0.0, toSource.dot(normal)) * scalingFactor;
                Vector reflection = lightRay.dir - normal * 2.0 * (lightRay.dir.dot(normal));
                reflection.normalize();
                phong += pow(max(0.0, reflection.dot(toSource)), shininess) * scalingFactor;

                // lambert value
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
            // Vector normal = getNormal(intersectionPoint, lightRay).dir;

            double t2 = (intersectionPoint - lightPosition).length();
            if (t2 < 1e-5)
                continue;

           

            if (!notVisible(lightRay, t2) )
            {
                Vector toSource = lightPosition - intersectionPoint;
                toSource.normalize();
                double distance = intersectionPoint.getDistanceBetweenPoints(lightPosition);
                double scalingFactor = exp(-spotlights[i]->fallOffParameter * distance * distance);
                // cout<<"Scaling Factor "<<scalingFactor<<endl;
                lambert += max(0.0, toSource.dot(normal)) * scalingFactor;
                Vector reflection = lightRay.dir - normal * 2.0 * (lightRay.dir.dot(normal));
                reflection.normalize();
                phong += pow(max(0.0, reflection.dot(toSource)), shininess) * scalingFactor;
            }
        }

        /**
         * @brief RECURSIVE REFLECTION
         *
         */
        color = Color(1, 1, 1) * co.specular * phong + this->color * lambert * co.diffuse + this->color * co.ambient;

        color.clip();

        if (level < levelOfRecursion)
        {
            
            Ray reflectionRay = Ray(intersectionPoint, ray.dir - normal * 2 * (ray.dir.dot(normal)));

            /**
             * @brief slightly forward from the point
             * (by moving the start a little bit towards the reflection direction)
             * to avoid self intersection
             *
             */
            reflectionRay.origin = reflectionRay.origin + reflectionRay.dir * 1e-5;

            // find nearest intersection object and do recursive call

            int nearestObjectIndex = findClosestObject(reflectionRay, color);

            if (nearestObjectIndex != -1)
            {
              

                Color reflectionColor(0, 0, 0); // refelction color
                
                double t = objects[nearestObjectIndex]->intersect(reflectionRay, reflectionColor, level + 1);

               
                color = color + reflectionColor * co.reflection;
            }
        }


        color.clip();
 // cout<<this->color.r<<" "<<this->color.g<<" "<<this->color.b<<endl;
  //cout<<color.r<<" "<<color.g<<" "<<color.b<<endl;
        return t;


    }

    void print() {
        cout<<"##Sphere##"<<endl;
        cout<<"radius "<<radius<<endl;
        cout<<"position "<<position.x<<" "<<position.y<<" "<<position.z<<endl;
        cout<<"coefficients "<<co.ambient<<" "<<co.diffuse<<" "<<co.specular<<" "<<co.reflection<<endl;
        cout<<"shininess "<<shininess<<endl<<endl;
    }
     

    ~Sphere() {
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