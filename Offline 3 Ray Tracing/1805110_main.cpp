#include <GL/glut.h>

#include <cmath>
#include <math.h>
#include <iostream>
#include <fstream>
#include <vector>

#include "1805110_classes/1805110_Vector.h"
#include "1805110_classes/1805110_Coefficients.h"
#include "1805110_classes/1805110_Sphere.h"
#include "1805110_classes/1805110_Pyramid.h"
#include "1805110_classes/1805110_Checkerboard.h"
#include "1805110_classes/1805110_Light.h"
#include "1805110_classes/1805110_Cube.h"
#include "1805110_classes/1805110_Ray.h"
#define _USE_MATH_DEFINES
#include "bitmap_image.hpp"
using namespace std;

bitmap_image image;

struct point
{
    GLfloat x, y, z;
};




/* Initialize OpenGL Graphics */
void initGL()
{
    // Set "clearing" or background color
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Black and opaque
    glEnable(GL_DEPTH_TEST);              // Enable depth testing for z-culling
}

// Global variables




void capture()
{
	cout<<"Capturing Image"<<endl;

	// initialize bitmap image and set background color to black
	for(int i=0;i<noOfPixels;i++)
		for(int j=0;j<noOfPixels;j++)
			image.set_pixel(i, j, 0, 0, 0);
	
	// image.save_image("black.bmp");
    cout<<"image "<<endl;

	double planeDistance = (screenHeight / 2.0) / tan((M_PI * fieldOfView) / (360.0));

	Vector topLeft = pos + (l * planeDistance) + (u * (screenHeight / 2.0)) - (r * (screenWidth / 2.0));

	double du = screenWidth / (noOfPixels*1.0);
	double dv = screenHeight / (noOfPixels*1.0);

	
	topLeft = topLeft + (r * du / 2.0) - (u * dv / 2.0);

	double objectFound = -1;
	double t,tMin;
    cout<<"entering loop"<<endl;
	for(int i=0;i<noOfPixels;i++)
	{
		for(int j=0;j<noOfPixels;j++)
		{
			// calculate current pixel
			Vector pixel = topLeft + (r * du * i) - (u * dv * j);

			// cast ray from EYE to (curPixel-eye) direction ; eye is the position of the camera
			Ray ray(pos,pixel-pos);
			

			// cout<<"Ray direction "<<ray.dir<<endl;

			// find nearest object
			tMin = -1;
			objectFound = -1;
            Color color;
			for(int o=0;o<objects.size();o++)
			{ 
               // cout<<"object "<<o<<endl;
				t = objects[o]->intersect(ray,color, 0);
				if(t>0 && (objectFound == -1 || t<tMin) )
					tMin = t , objectFound = o;
			}
          //  cout<<"objectFound "<<objectFound<<endl;

			// if nearest object is found, then shade the pixel
			if(objectFound != -1)
			{
				
				color = Color(0,0,0);
				// cout<<"Before Color "<<color.r<<" "<<color.g<<" "<<color.b<<endl;
				double t = objects[objectFound]->intersect(ray,color, 1);

				color.clip();
				
				// cout<<"After Color "<<color.r<<" "<<color.g<<" "<<color.b<<endl;
				image.set_pixel(i, j, 255*color.r, 255*color.g, 255*color.b);
			}
		}
	}

	image.save_image("Output.bmp");
	
	cout<<"Saving Image"<<endl;		
}

void drawAxes()
{
    glLineWidth(3);
    glBegin(GL_LINES);
    glColor3f(1, 0, 0); // Red
    // X axis
    glVertex3f(0, 0, 0);
    glVertex3f(2, 0, 0);

    glColor3f(0, 1, 0); // Green
    // Y axis
    glVertex3f(0, 0, 0);
    glVertex3f(0, 2, 0);

    glColor3f(0, 0, 1); // Blue
    // Z axis
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, 2);
    glEnd();
}

void drawSphere(double radius, int stacks, int slices) {
    struct point points[stacks+1][slices+1];
    for (int j = 0; j <= stacks; j++) {
        double phi = -M_PI / 2.0 + j * M_PI / stacks;
        double r = radius * cos(phi);
        double h = radius * sin(phi);
        for (int i = 0; i < slices+1; i++) {
            double theta = i * 2.0 * M_PI / slices;
            points[j][i].x = r * cos(theta);
            points[j][i].y = r * sin(theta);
            points[j][i].z = h;
        }
    }

    glBegin(GL_QUADS);
        for (int j = 0; j < stacks; j++) {
            for (int i = 0; i < slices; i++) {
                GLfloat c = (2+cos((i+j) * 2.0 * M_PI / slices)) / 3;
                glColor3f(c,c,c);
                glVertex3f(points[j][i].x, points[j][i].y, points[j][i].z);
                glVertex3f(points[j][i+1].x, points[j][i+1].y, points[j][i+1].z);

                glVertex3f(points[j+1][i+1].x, points[j+1][i+1].y, points[j+1][i+1].z);
                glVertex3f(points[j+1][i].x, points[j+1][i].y, points[j+1][i].z);
            }
        }
    glEnd();
}

/* Draw a pyramid centered at the origin */
void drawPyramid() {
    glBegin(GL_TRIANGLES);           // Begin drawing the pyramid with 4 triangles
        // Front
        glColor3f(1.0f, 0.0f, 0.0f);     // Red
        glVertex3f( 0.0f, 1.0f, 0.0f);
        glVertex3f(-1.0f, -1.0f, 1.0f);
        glVertex3f(1.0f, -1.0f, 1.0f);

        // Right
        glColor3f(1.0f, 1.0f, 0.0f);     // Yellow
        glVertex3f(0.0f, 1.0f, 0.0f);
        glVertex3f(1.0f, -1.0f, 1.0f);
        glVertex3f(1.0f, -1.0f, -1.0f);

        // Back
        glColor3f(0.0f, 1.0f, 0.0f);     // Green
        glVertex3f(0.0f, 1.0f, 0.0f);
        glVertex3f(1.0f, -1.0f, -1.0f);
        glVertex3f(-1.0f, -1.0f, -1.0f);

        // Left
        glColor3f(0.0f,1.0f,1.0f);       // Cyan
        glVertex3f( 0.0f, 1.0f, 0.0f);
        glVertex3f(-1.0f,-1.0f,-1.0f);
        glVertex3f(-1.0f,-1.0f, 1.0f);
    glEnd();   // Done drawing the pyramid
}


void drawCylinder(double height, double radius)
{
    double tempx = radius, tempy = 0;
    double currx, curry;
    glColor3f(.8, .7, .9);
    glBegin(GL_QUADS);
    // cout << "cylinder being drawn R " << radius << " Height " << height << endl;
    for (double thetaD = -35; thetaD <= 35; thetaD++)
    { //(double theta=-35;theta<=35;theta++)
        double theta = (M_PI * thetaD * 1.0) / 180.0;
        // cout << theta << "current theta " << endl;
        currx = radius * cos(theta);
        curry = radius * sin(theta);

        glVertex3f(currx, curry, height / 2);
        glVertex3f(currx, curry, -height / 2);

        glVertex3f(tempx, tempy, -height / 2);
        glVertex3f(tempx, tempy, height / 2);

        tempx = currx;
        tempy = curry;
    }
    glEnd();
}
void drawCheckerBoard(){
 
}



/*  Handler for window-repaint event. Call back when the window first appears and
    whenever the window needs to be re-painted. */
void display()
{
    // glClear(GL_COLOR_BUFFER_BIT);            // Clear the color buffer (background)
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW); // To operate on Model-View matrix
    glLoadIdentity();           // Reset the model-view matrix

    // default arguments of gluLookAt
    // gluLookAt(0,0,0, 0,0,-100, 0,1,0);

    // control viewing (or camera)
    r.x = (l.y * u.z - l.z * u.y);
    r.y = -(l.x * u.z - l.z * u.x);
    r.z = (l.x * u.y - l.y * u.x);
    gluLookAt(pos.x, pos.y, pos.z,
              pos.x + l.x, pos.y + l.y, pos.z + l.z,
              u.x, u.y, u.z);
    // glScalef(2,2,2);
     drawAxes();
    //drawobjects
    //drawlight
    //drawspotlight
   for (auto object : objects) {
        object->draw();
    }

  
    glutSwapBuffers(); // Render now
}

/* Handler for window re-size event. Called back when the window first appears and
   whenever the window is re-sized with its new width and height */
void reshape(GLsizei width, GLsizei height){
    if (height == 0)
        height = 1; // To prevent divide by 0
    GLfloat aspect = (GLfloat)width / (GLfloat)height;

    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION); // To operate on the Projection matrix
    glLoadIdentity();            // Reset the projection matrix
    gluPerspective(fieldOfView, aspectRatio, distanceNear, distanceFar);
}

void keyboardListener(unsigned char key, int xx, int yy)
{
    double v = 0.25;
    double s;
    double rate = 0.1;
    switch (key)
    {

    case '1':
        r.x = r.x * cos(rate) + l.x * sin(rate);
        r.y = r.y * cos(rate) + l.y * sin(rate);
        r.z = r.z * cos(rate) + l.z * sin(rate);

        l.x = l.x * cos(rate) - r.x * sin(rate);
        l.y = l.y * cos(rate) - r.y * sin(rate);
        l.z = l.z * cos(rate) - r.z * sin(rate);
        break;

    case '2':
        r.x = r.x * cos(-rate) + l.x * sin(-rate);
        r.y = r.y * cos(-rate) + l.y * sin(-rate);
        r.z = r.z * cos(-rate) + l.z * sin(-rate);

        l.x = l.x * cos(-rate) - r.x * sin(-rate);
        l.y = l.y * cos(-rate) - r.y * sin(-rate);
        l.z = l.z * cos(-rate) - r.z * sin(-rate);
        break;

    case '3':
        l.x = l.x * cos(rate) + u.x * sin(rate);
        l.y = l.y * cos(rate) + u.y * sin(rate);
        l.z = l.z * cos(rate) + u.z * sin(rate);

        u.x = u.x * cos(rate) - l.x * sin(rate);
        u.y = u.y * cos(rate) - l.y * sin(rate);
        u.z = u.z * cos(rate) - l.z * sin(rate);
        break;

    case '4':
        l.x = l.x * cos(-rate) + u.x * sin(-rate);
        l.y = l.y * cos(-rate) + u.y * sin(-rate);
        l.z = l.z * cos(-rate) + u.z * sin(-rate);

        u.x = u.x * cos(-rate) - l.x * sin(-rate);
        u.y = u.y * cos(-rate) - l.y * sin(-rate);
        u.z = u.z * cos(-rate) - l.z * sin(-rate);
        break;

    case '5':
        u.x = u.x * cos(rate) + r.x * sin(rate);
        u.y = u.y * cos(rate) + r.y * sin(rate);
        u.z = u.z * cos(rate) + r.z * sin(rate);

        r.x = r.x * cos(rate) - u.x * sin(rate);
        r.y = r.y * cos(rate) - u.y * sin(rate);
        r.z = r.z * cos(rate) - u.z * sin(rate);
        break;

    case '6':
        u.x = u.x * cos(-rate) + r.x * sin(-rate);
        u.y = u.y * cos(-rate) + r.y * sin(-rate);
        u.z = u.z * cos(-rate) + r.z * sin(-rate);

        r.x = r.x * cos(-rate) - u.x * sin(-rate);
        r.y = r.y * cos(-rate) - u.y * sin(-rate);
        r.z = r.z * cos(-rate) - u.z * sin(-rate);
        break;

    case '0':
        capture();
        break;
    case 'z':
     pos.x += u.x * 5;
        pos.y += u.y * 5;
        pos.z += u.z * 5;
        break;
    case 'y':  
     pos.x -= u.x * 5;
        pos.y -= u.y * 5;
        pos.z -= u.z * 5;
        break;
      
   
    default:
        break;
    }
    glutPostRedisplay();
}

void specialKeyListener(int key, int x, int y)
{
    switch (key)
    {
    case GLUT_KEY_UP:       // down arrow key
        pos.x += l.x * 5; // l forward vector
        pos.y += l.y * 5;
        pos.z += l.z * 5;
        break;
    case GLUT_KEY_DOWN: // up arrow key
        pos.x -= l.x * 5;
        pos.y -= l.y * 5;
        pos.z -= l.z * 5;
        break;

    case GLUT_KEY_RIGHT:
        pos.x += r.x * 5; // r right vector
        pos.y += r.y * 5;

        pos.z += r.z * 5;

        break;
    case GLUT_KEY_LEFT:
        pos.x -= r.x * 5;
        ;
        pos.y -= r.y * 5;
        ;
        pos.z -= r.z * 5;
        ;
        break;

    case GLUT_KEY_PAGE_UP:
        pos.x += u.x * 5;
        pos.y += u.y * 5;
        pos.z += u.z * 5;
        break;
    case GLUT_KEY_PAGE_DOWN:
        pos.x -= u.x * 5;
        pos.y -= u.y * 5;
        pos.z -= u.z * 5;
        break;

    case GLUT_KEY_INSERT:
        break;

    case GLUT_KEY_HOME:
        break;
    case GLUT_KEY_END:
        break;

    default:
        break;
    }
    glutPostRedisplay();
}
void readObjectData(string filename){

    ifstream in(filename);
    
    in>>distanceNear>>distanceFar;
    in>>fieldOfView;
    in>>aspectRatio;
    in>>levelOfRecursion;
    in>>noOfPixels;
    int tileWidth;
    Coefficients checkerboardCo;
    
    in>>tileWidth;
    in>>checkerboardCo.ambient>>checkerboardCo.diffuse>>checkerboardCo.reflection;
    Checkerboard *checkerboard=new Checkerboard(tileWidth,checkerboardCo);
     objects.push_back(checkerboard);
    cout<<"checkerboard"<<endl;
    
    in>>noOfObjects;
    cout<<"noOfObjects "<<noOfObjects<<endl;
    for(int i=0;i<noOfObjects;i++){
        string type;
        in>>type;
        if(type=="sphere"){
            cout<<"sphere"<<endl;
            Vector position;
            double radius;
            Color color;
            Coefficients co;
            double shininess;

            in >> position >> radius >> color >> co >> shininess;
            Sphere *s = new Sphere(position,radius,color,co,shininess);
            objects.push_back(s);
        }
        else if(type=="cube"){
            cout<<"cube"<<endl;
            Vector position;
            double width;
            Color color;
            Coefficients co;
            double shininess;
            in>>position>>width>>color>>co>>shininess;
            Cube *c = new Cube(position,width,color,co,shininess);
            objects.push_back(c);
        }
        else if(type=="pyramid"){
            cout<<"pyramid"<<endl;
            Vector position;
            double width,height;
            Color color;
            Coefficients co;
            double shininess;
            in>>position>>width>>height>>color>>co>>shininess;
            Pyramid *p = new Pyramid(position,width,height,color,co,shininess);
            objects.push_back(p);

        }
    }
    int noOfLights;
    in>>noOfLights;
    cout<<"noOfLights "<<noOfLights<<endl;
    for(int i=0;i<noOfLights;i++){
        Vector position;
        double falloff;
        in>>position>>falloff;
        Light *l = new Light(position,falloff);
        lights.push_back(l);
        
    }
    int noOfSpotLightSources;
    in>>noOfSpotLightSources;
    cout<<"noOfSpotLightSources "<<noOfSpotLightSources<<endl;
    for(int i=0;i<noOfSpotLightSources;i++){
        Vector position;
        double falloff;
        Vector direction;
        double angle;
        in>>position>>falloff>>direction>>angle;
        SpotLightSource *s = new SpotLightSource(position,falloff,direction,angle);
        spotlights.push_back(s);
    }

}

/* Main function: GLUT runs as a console application starting at main()  */
int main(int argc, char **argv)
{
    pos.x = 0;
    pos.y = -150;
    pos.z = 50;
//0 -150 50
    l.x = 0;
    l.y = 1;
    l.z = 0;
//0 1 0
    u.x = 0;
    u.y = 0;
    u.z = 1;
//0 0 1
    r.x = 1;
    r.y = 0;
    r.z = 0;
  //  1 0 0
   
    readObjectData("in.txt");
    for (auto object : objects) {
        object->print();
    }
    screenHeight=500;
    screenWidth=500;
    cout<<objects.size()<<endl;
    image = bitmap_image(noOfPixels, noOfPixels);
    glutInit(&argc, argv);                                    // Initialize GLUT
    glutInitWindowSize(screenWidth, screenHeight);                             // Set the window's initial width & height
    glutInitWindowPosition(0, 0);                           // Position the window's initial top-left corner
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB); // Depth, Double buffer, RGB color
    glutCreateWindow("offline 3");                  // Create a window with the given title
    glutDisplayFunc(display);                                 // Register display callback handler for window re-paint
    glutReshapeFunc(reshape);                                 // Register callback handler for window re-shape

    glutKeyboardFunc(keyboardListener);
    glutSpecialFunc(specialKeyListener);

    initGL();       // Our own OpenGL initialization
    glutMainLoop(); // Enter the event-processing loop
    return 0;
}
