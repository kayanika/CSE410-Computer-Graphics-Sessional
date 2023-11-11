//  Assignment 2 (Jan 2023) Raster Based Graphics Pipeline
#include <bits/stdc++.h>
#define _USE_MATH_DEFINES
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include "matrix.cpp"
#include "bitmap_image.hpp"

using namespace std;

ifstream input;
int line=0;
int com=0;
ofstream output;
stack<matrix> matrixStack;
int totalTriangles = 0;
double eyeX, eyeY, eyeZ, lookX, lookY, lookZ, upX, upY, upZ, fovy, aspectRatio, near, far;
int screen_width , screen_height;
double viewBox_leftWidth=-1, veiwBox_rightWidth=1, viewBox_bottomHeight=-1, viewBox_topHeight=1;
double zMin=-1, zMax=1;
double Top_Y, Bottom_Y, Left_X, Right_X;

// stage 1 : Modeling Transformation
bool modelingTransformation()
{
    string line;

    input >> line;
    com++;
    
    if (line == "triangle")
    {  
        point p1, p2, p3;
        input >> p1 >> p2 >> p3;
        p1 = matrixStack.top() * (p1);
        p2 = matrixStack.top() * (p2);
        p3 = matrixStack.top() * (p3);

        output << p1 << endl;
        output << p2 << endl;
        output << p3 << endl;
        output << endl;

        totalTriangles++;
    }
    else if (line == "translate")
    {
        double tx, ty, tz;
        input >> tx >> ty >> tz;
        matrix m = matrix().getTranslationMatrix(tx, ty, tz);
        matrix m1 = matrixStack.top() * m;
        matrixStack.pop();
        matrixStack.push(m1);
    }
    else if (line == "scale")
    {
        double sx, sy, sz;
        input >> sx >> sy >> sz;
        matrix m = matrix().getScalingMatrix(sx, sy, sz);
        matrix m1 = matrixStack.top() * m;
        matrixStack.pop();
        matrixStack.push(m1);
    }
    else if (line == "rotate")
    {
        double angle, rx, ry, rz;
        input >> angle >> rx >> ry >> rz;
        matrix m = matrix().getRotationMatrix(angle, point(rx, ry, rz));
        
        matrix m1 = matrixStack.top() * m;
        
        matrixStack.pop();
        matrixStack.push(m1);
        
    }
    else if (line == "push")
    {
        matrix m = matrixStack.top();
        matrixStack.push(m);
    }
    else if (line == "pop")
    {
        if (!matrixStack.empty())
            matrixStack.pop();
        else
        {
            cout << "Stack is empty" << endl;
            exit(0);
        }
    }
    else if (line == "end")
    {
        return false;
    }
    return true;
}
// stage 2 : Viewing Transformation
void viewingTransformation()
{
  
    input.open("stage1.txt");
    output.open("stage2.txt");

    matrix m = matrix().getViewMatrix(point(eyeX, eyeY, eyeZ), point(lookX, lookY, lookZ), point(upX, upY, upZ));
    // m.printMatrix();
    for (int i = 0; i < totalTriangles; i++)
    {
        point p1, p2, p3;
        input >> p1 >> p2 >> p3;
        p1 = m * p1;
        p2 = m * p2;
        p3 = m * p3;
        // cout<<"Points"<<endl;
        output << p1 << endl;
        //  cout<<p1<<endl;
        output << p2 << endl;
        // cout<<p2<<endl;
        output << p3 << endl;
        // cout<<p3<<endl;
        output << endl;
    }
    input.close();
    output.close();
}
// stage 3 : Projection Transformation
void projectionTransformation()
{
    input.open("stage2.txt");
    output.open("stage3.txt");
    matrix m = matrix().getProjectionMatrix(fovy, aspectRatio, near, far);
    for (int i = 0; i < totalTriangles; i++)
    {
        point p1, p2, p3;
        input >> p1 >> p2 >> p3;
        p1 = m * p1;
        p2 = m * p2;
        p3 = m * p3;

        output << p1 << endl;
        output << p2 << endl;
        output << p3 << endl;
        output << endl;
    }
    input.close();
    output.close();
}
static unsigned long int g_seed = 1;

int randomGen()
{
g_seed = (214013 * g_seed + 2531011);
return (g_seed >> 16) & 0x7FFF;
}

int main()
{
  //  read from scene.txt file
    input.open("scene.txt");
    output.open("stage1.txt");
    input >> eyeX >> eyeY >> eyeZ;
    input >> lookX >> lookY >> lookZ;
    input >> upX >> upY >> upZ;
    input >> fovy >> aspectRatio >> near >> far;
    matrixStack.push(matrix().getIdentityMatrix());
    while (modelingTransformation())
        ;
    input.close();
    output.close();

    // // cout << eyeY << "   " << upX << " " << aspectRatio << " " << fovy << endl;
    viewingTransformation();

     projectionTransformation();

     //stage 4 begins
    input.open("config.txt");
    output.open("z_buffer.txt");
    
    input>>screen_width>>screen_height;
    input.close();
    input.open("stage3.txt");
    double dx= (veiwBox_rightWidth-viewBox_leftWidth)/screen_width;
    double dy= (viewBox_topHeight-viewBox_bottomHeight)/screen_height;

    Top_Y= viewBox_topHeight-dy/2;
    Bottom_Y= viewBox_bottomHeight+dy/2;
    Left_X= viewBox_leftWidth+dx/2;
    Right_X= veiwBox_rightWidth-dx/2;
    // cout<<Top_Y<<" "<<Bottom_Y<<" "<<Left_X<<" "<<Right_X<<endl;

    vector<vector<double>> zBuffer(screen_height,vector<double>(screen_width,zMax));
    bitmap_image img(screen_width,screen_height);

    for(int i=0;i<screen_width;i++){
        for(int j=0;j<screen_height;j++){
            img.set_pixel(i,j,0,0,0);
        }
    }
    //assign traingle colors

    for(int triangleNo=0;triangleNo<totalTriangles;triangleNo++){
        point trianglePoints[3];
        input>>trianglePoints[0]>>trianglePoints[1]>>trianglePoints[2];
        
        int rgb[3];


        for(int i=0;i<3;i++){
            rgb[i]=randomGen()%256;
        }
        double minX=max(min(min(trianglePoints[0].x,trianglePoints[1].x),trianglePoints[2].x),viewBox_leftWidth);
       
        double maxX=min(max(max(trianglePoints[0].x,trianglePoints[1].x),trianglePoints[2].x),veiwBox_rightWidth);
        double minY=max(min(min(trianglePoints[0].y,trianglePoints[1].y),trianglePoints[2].y),viewBox_bottomHeight);
        double maxY=min(max(max(trianglePoints[0].y,trianglePoints[1].y),trianglePoints[2].y),viewBox_topHeight);
        int scanlineYend= ceil((Top_Y-maxY)/dy);
        int scanlineYstart= ceil((Top_Y-minY)/dy);

        for(int y=scanlineYend;y<=scanlineYstart;y++){
            double scanlineY=Top_Y-y*dy;
            int intersectionCount=0;
           double xvals[2],zvals[2];
           //find the three edges between the points of the triangles

              for( int i=0,j=1;i<3;i++,j=(j+1)%3){
             
                if(trianglePoints[i].y!=trianglePoints[j].y){
                    
                    if(scanlineY>=min(trianglePoints[i].y,trianglePoints[j].y) && scanlineY<=max(trianglePoints[i].y,trianglePoints[j].y)){
                        double parameterT= (scanlineY-trianglePoints[i].y)/(trianglePoints[j].y-trianglePoints[i].y);
                        xvals[intersectionCount]= trianglePoints[i].x+parameterT*(trianglePoints[j].x-trianglePoints[i].x);
                        zvals[intersectionCount]= trianglePoints[i].z+parameterT*(trianglePoints[j].z-trianglePoints[i].z);
                        intersectionCount++;
                    }

                }
                
              }
             
              //readjust if the intersection points are outside the range
              double oldvals[2];
              oldvals[0]=xvals[0];
                oldvals[1]=xvals[1];
             for(int i=0;i<2;i++){
                      if(xvals[i]<minX){
                        xvals[i]=minX;
                  double lineParamterT= (xvals[i]-oldvals[1])/(oldvals[1]-oldvals[0]);
                        zvals[i]=zvals[1]+lineParamterT*(zvals[1]-zvals[0]); 
                          
                      }  
                      else if(xvals[i]>maxX){
                        xvals[i]=maxX;
                          double lineParamterT= (xvals[i]-oldvals[1])/(oldvals[1]-oldvals[0]);
                         zvals[i]=zvals[1]+lineParamterT*(zvals[1]-zvals[0]);
                          
                      }
           
              }
              if (xvals[0] >= xvals[1]) {
    double tempX = xvals[0];
    double tempZ = zvals[0];

    xvals[0] = xvals[1];
    zvals[0] = zvals[1];

    xvals[1] = tempX;
    zvals[1] = tempZ;
}

              int startPixel= ceil((xvals[0]-Left_X)/dx);
              
                int endPixel= ceil((xvals[1]-Left_X)/dx);
             

                for(int x=startPixel;x<=endPixel;x++){
                    double scanlineX=Left_X+x*dx;
                    double lineParamterT= (scanlineX-xvals[1])/(xvals[1]-xvals[0]);
                    double z= zvals[1]+lineParamterT*(zvals[1]-zvals[0]);
                 
                    if(z>=zMin && z<zBuffer[y][x]){
                        zBuffer[y][x]=z;
                        
                        img.set_pixel(x,y,rgb[0],rgb[1],rgb[2]);
                        
                    }
                }
        }
    }

                for(int i=0;i<screen_height;i++){
                    for(int j=0;j<screen_width;j++){
                        if(zBuffer[i][j]<zMax){
                            output<<setprecision(6)<<fixed<<zBuffer[i][j]<<"  ";
                 
                          //  cout<<setprecision(6)<<fixed<<zBuffer[i][j]<<" ";
                        }
                       
                    }  output<<endl;
                }
                
                //cleanup
                
                img.save_image("out.bmp");
                
    for (int i=0; i < screen_height; i++) {
        zBuffer[i].clear();
        zBuffer[i].shrink_to_fit();
    }
    zBuffer.clear();
    zBuffer.shrink_to_fit();
                
output.close();
        





    }


   
