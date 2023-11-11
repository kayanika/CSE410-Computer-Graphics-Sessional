#pragma once


#include "1805110_Vector.h"
#include "1805110_Coefficients.h"
#include "1805110_Color.h"
#include "1805110_Light.h"
#include "1805110_Ray.h"
#include "1805110_Object3d.h"
#include <iostream>
#include <bits/stdc++.h>

class Triangle : public Object3d
{
public:
    Vector p, q, r;
    
    Triangle(Vector a, Vector b, Vector c, Color color, Coefficients co, double s) : Object3d((0, 0, 0), color, s, co)
    {
        p = a;
        q = b;
        r = c;
    }
    void draw(){
        glBegin(GL_TRIANGLES);{
            glColor3f(color.r,color.g,color.b);
            glVertex3f(p.x,p.y,p.z);
            glVertex3f(q.x,q.y,q.z);
            glVertex3f(r.x,r.y,r.z);
        }
        glEnd();
    }
    void print()
    {
        cout << "Triangle" << endl;
        cout << "p " << p.x << " " << p.y << " " << p.z << endl;
        cout << "q " << q.x << " " << q.y << " " << q.z << endl;
        cout << "r " << r.x << " " << r.y << " " << r.z << endl;
    }
    double intersect(Ray ray, Color &color, int level)
    {
        double determinantA, determinantBeta, determinantGamma, determinantT;
        double beta, gamma, t;

        Vector a, b, c; // three points of the triangle
        Vector normal = (q - p).cross(r - p);

        a = p;
        b = q;
        c = r;

        // calculate the determinant of the matrix A
        determinantA = (a.x - b.x) * ((a.y - c.y) * ray.dir.z - (a.z - c.z) * ray.dir.y);
        determinantA += (a.y - b.y) * ((a.z - c.z) * ray.dir.x - (a.x - c.x) * ray.dir.z);
        determinantA += (a.z - b.z) * ((a.x - c.x) * ray.dir.y - (a.y - c.y) * ray.dir.x);

        // calculate the determinant of the matrix Beta
        determinantBeta = (a.x - ray.origin.x) * ((a.y - c.y) * ray.dir.z - (a.z - c.z) * ray.dir.y);
        determinantBeta += (a.y - ray.origin.y) * ((a.z - c.z) * ray.dir.x - (a.x - c.x) * ray.dir.z);
        determinantBeta += (a.z - ray.origin.z) * ((a.x - c.x) * ray.dir.y - (a.y - c.y) * ray.dir.x);

        // calculate the determinant of the matrix Gamma
        determinantGamma = (a.x - b.x) * ((a.y - ray.origin.y) * ray.dir.z - (a.z - ray.origin.z) * ray.dir.y);
        determinantGamma += (a.y - b.y) * ((a.z - ray.origin.z) * ray.dir.x - (a.x - ray.origin.x) * ray.dir.z);
        determinantGamma += (a.z - b.z) * ((a.x - ray.origin.x) * ray.dir.y - (a.y - ray.origin.y) * ray.dir.x);

        // calculate the determinant of the matrix T
        determinantT = (a.x - b.x) * ((a.y - c.y) * (a.z - ray.origin.z) - (a.z - c.z) * (a.y - ray.origin.y));
        determinantT += (a.y - b.y) * ((a.z - c.z) * (a.x - ray.origin.x) - (a.x - c.x) * (a.z - ray.origin.z));
        determinantT += (a.z - b.z) * ((a.x - c.x) * (a.y - ray.origin.y) - (a.y - c.y) * (a.x - ray.origin.x));

        // if the determinant of A is zero, then the ray is parallel to the triangle. So, no intersection. Return INF.
        if (determinantA == 0.0)
        {
            t = -1;
        }
        else
        {
            // calculate beta, gamma and t
            beta = determinantBeta / determinantA;
            gamma = determinantGamma / determinantA;
            // check if there is an intersection
            if (beta < 0 || gamma < 0 || beta + gamma > 1)
            {
                t = -1; // no intersection
            }
            else
            {
                t = determinantT / determinantA; // intersection
            }
        }
        if(t<0) return -1;
        if (level == 0)
            return t;

        if (normal.dot(ray.dir) > 0)
            normal = normal * -1.0;
        normal.normalize();

      
        Vector intersectionPoint = ray.origin + ray.dir * t;
        Color colorAtIntersection = this->color;

     
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

           

            if (!notVisible(lightRay, t2))
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

        
        color = Color(1, 1, 1) * co.specular * phong + this->color * lambert * co.diffuse + this->color * co.ambient;

        color.clip();

        if (level < levelOfRecursion)
        {
            
            Ray reflectionRay = Ray(intersectionPoint, ray.dir - normal * 2 * (ray.dir.dot(normal)));
            

         
            reflectionRay.origin = reflectionRay.origin + reflectionRay.dir * 0.0000001;

            

            int closestObject = findClosestObject(reflectionRay, color);
            
            
            if (closestObject != -1)
            {
               

                Color temp(0, 0, 0); // refelction color
               
                double t = objects[closestObject]->intersect(reflectionRay, temp, level + 1);

                
                color=color+temp*co.reflection;
            }
        }


        color.clip();

        return t;
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
};

