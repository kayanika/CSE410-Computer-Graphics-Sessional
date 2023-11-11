#include <GL/glut.h>
#include <windows.h>
#include <cmath>
#include <math.h>
#include <iostream>
#include <vector>
#define _USE_MATH_DEFINES
using namespace std;

struct point
{
    GLfloat x, y, z;
};
struct Vertex
{
    double x, y, z;
    Vertex()
    {
    }
    Vertex(double a, double b, double c)
    {
        x = a;
        y = b;
        z = c;
    }
};
struct Triangle
{
    Vertex p, q, r;
    Triangle(Vertex l, Vertex m, Vertex n)
    {
        p = Vertex(l.x, l.y, l.z);
        q = Vertex(m.x, m.y, m.z);
        r = Vertex(n.x, n.y, n.z);
    }
};
Vertex P1 = Vertex(1.0, 0.0, 0.0);
Vertex P2 = Vertex(0.0, 1.0, 0.0);
Vertex P3 = Vertex(0.0, 0.0, 1.0);

Triangle t = Triangle(P1, P2, P3);
GLdouble triangleLength;
GLdouble CylinderRadius = 0;
GLdouble CylinderHeight = 0;
GLdouble TriangleDelX;
double AngleR = 0;
float CylinderDelL = 0;
float CylinderDelR = 0;
float CylinderCenterX;
float sphereRadius;
float a;
bool firstTriangle;
int stacks, slices;

/* Initialize OpenGL Graphics */
void initGL()
{
    // Set "clearing" or background color
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Black and opaque
    glEnable(GL_DEPTH_TEST);              // Enable depth testing for z-culling
}

// Global variables
struct point pos; // position of the eye
struct point l;   // look/forward direction
struct point r;   // right direction
struct point u;   // up direction
int pointsPerRow = 12;
struct point pointsS[12][12];
struct Vertex topVertices[4];
bool topVertice = false;

/* Draw axes: X in Red, Y in Green and Z in Blue */
void buildUnitPositiveX()
{
    const float DEG2RAD = acos(-1) / 180.0f;

    float n1[3]; // normal of longitudinal plane rotating along Y-axis
    float n2[3]; // normal of latitudinal plane rotating along Z-axis
    float v[3];  // direction vector intersecting 2 planes, n1 x n2
    float a1;    // longitudinal angle along Y-axis
    float a2;    // latitudinal angle along Z-axis

    // compute the number of vertices per row, 2^n + 1
    // pointsPerRow = (int)pow(2, subdivision) + 1;

    // rotate latitudinal plane from 45 to -45 degrees along Z-axis (top-to-bottom)
    for (unsigned int i = 0; i < pointsPerRow; ++i)
    {
        // normal for latitudinal plane
        // if latitude angle is 0, then normal vector of latitude plane is n2=(0,1,0)
        // therefore, it is rotating (0,1,0) vector by latitude angle a2
        a2 = DEG2RAD * (45.0f - 90.0f * i / (pointsPerRow - 1));
        n2[0] = -sin(a2);
        n2[1] = cos(a2);
        n2[2] = 0;

        // rotate longitudinal plane from -45 to 45 along Y-axis (left-to-right)
        for (unsigned int j = 0; j < pointsPerRow; ++j)
        {
            // normal for longitudinal plane
            // if longitude angle is 0, then normal vector of longitude is n1=(0,0,-1)
            // therefore, it is rotating (0,0,-1) vector by longitude angle a1
            a1 = DEG2RAD * (-45.0f + 90.0f * j / (pointsPerRow - 1));
            n1[0] = -sin(a1);
            n1[1] = 0;
            n1[2] = -cos(a1);

            // find direction vector of intersected line, n1 x n2
            v[0] = n1[1] * n2[2] - n1[2] * n2[1];
            v[1] = n1[2] * n2[0] - n1[0] * n2[2];
            v[2] = n1[0] * n2[1] - n1[1] * n2[0];

            // normalize direction vector
            float scale = sphereRadius / sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
            v[0] *= scale;
            v[1] *= scale;
            v[2] *= scale;

            // add a vertex into array

            pointsS[i][j].x = v[0];
            pointsS[i][j].y = v[1];
            pointsS[i][j].z = v[2];
        }
    }
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
void updateTriangle()
{
    double px = 1, py = 0, pz = 0, qx = 0, qy = 1, qz = 0, rx = 0, ry = 0, rz = 1;
    t.p.x = ((t.p.x * 15) + (1.0 / 3.0)) / 16.0;
    // cout << t.p.x << " t.p.x" << endl;
    t.p.y = ((t.p.y * 15) + (1.0 / 3.0)) / 16.0;
    // cout << t.p.y << " t.p.y" << endl;
    t.p.z = ((t.p.z * 15) + (1.0 / 3.0)) / 16.0;

    t.q.x = ((t.q.x * 15) + (1.0 / 3.0)) / 16.0;
    t.q.y = ((t.q.y * 15) + (1.0 / 3.0)) / 16.0;
    t.q.z = ((t.q.z * 15) + (1.0 / 3.0)) / 16.0;

    t.r.x = ((t.r.x * 15) + (1.0 / 3.0)) / 16.0;
    t.r.y = ((t.r.y * 15) + (1.0 / 3.0)) / 16.0;
    t.r.z = ((t.r.z * 15) + (1.0 / 3.0)) / 16.0;

    TriangleDelX = sqrt(pow((0.5 * (px + qx)) - (0.5 * (t.p.x + t.q.x)), 2) + pow((0.5 * (py + qy)) - (0.5 * (t.p.y + t.q.y)), 2) + pow((0.5 * (pz + qz)) - (0.5 * (t.p.z + t.q.z)), 2));
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
void updateTriangle2()
{
    if (t.p.x <= 1 && t.p.y <= 1 && t.p.z <= 1)
    {
        double px = 1, py = 0, pz = 0, qx = 0, qy = 1, qz = 0, rx = 0, ry = 0, rz = 1;
        t.p.x = ((t.p.x * 16) - (1.0 / 3.0)) / 15.0;
        cout << t.p.x << " t.p.x" << endl;
        t.p.y = ((t.p.y * 16) - (1.0 / 3.0)) / 15.0;
        cout << t.p.y << " t.p.y" << endl;
        t.p.z = ((t.p.z * 16) - (1.0 / 3.0)) / 15.0;
        cout << t.p.z << " t.p.z" << endl;

        t.q.x = ((t.q.x * 16) - (1.0 / 3.0)) / 15.0;
        t.q.y = ((t.q.y * 16) - (1.0 / 3.0)) / 15.0;
        t.q.z = ((t.q.z * 16) - (1.0 / 3.0)) / 15.0;

        t.r.x = ((t.r.x * 16) - (1.0 / 3.0)) / 15.0;
        t.r.y = ((t.r.y * 16) - (1.0 / 3.0)) / 15.0;
        t.r.z = ((t.r.z * 16) - (1.0 / 3.0)) / 15.0;

        TriangleDelX = sqrt(pow((0.5 * (px + qx)) - (0.5 * (t.p.x + t.q.x)), 2) + pow((0.5 * (py + qy)) - (0.5 * (t.p.y + t.q.y)), 2) + pow((0.5 * (pz + qz)) - (0.5 * (t.p.z + t.q.z)), 2));
    }
}

void drawTraingle()
{
    glBegin(GL_TRIANGLES);
    glVertex3f(t.p.x, t.p.y, t.p.z);
    //  cout << "t.p.x, t.p.y, t.p.z " << t.p.x << " " << t.p.y << " " << t.p.z << endl;
    glVertex3f(t.q.x, t.q.y, t.q.z);
    //  cout << "t.q.x, t.q.y, t.q.z " << t.q.x << " " << t.q.y << " " << t.q.z << endl;
    glVertex3f(t.r.x, t.r.y, t.r.z);
    // cout << "t.r.x, t.r.y, t.r.z " << t.r.x << " " << t.r.y << " " << t.r.z << endl;
    glEnd();
}
void updateCylinder()
{
    CylinderDelR = TriangleDelX * tan(0.955305966);
    // cout << "traingleDelX " << TriangleDelX << endl;
    CylinderDelL = sqrt(pow(CylinderDelR, 2) + pow(TriangleDelX, 2));

    CylinderRadius = CylinderDelR;
    CylinderCenterX = (sqrt(2) / 2.0) - CylinderDelL;
    CylinderHeight = sqrt(pow((t.p.x - t.q.x), 2) + pow((t.p.y - t.q.y), 2) + pow((t.p.z - t.q.z), 2));
    // cout << "cylinder Height " << CylinderHeight << endl;
    //  cout << "CylinderCenterX " << CylinderCenterX << endl;
}

void drawSphere()
{
    buildUnitPositiveX();

    glBegin(GL_QUADS);
    for (int j = 0; j < pointsPerRow; j++)
    {
        for (int i = 0; i < pointsPerRow; i++)
        {

            glVertex3f(pointsS[j][i].x, pointsS[j][i].y, pointsS[j][i].z);
            glVertex3f(pointsS[j][i + 1].x, pointsS[j][i + 1].y, pointsS[j][i + 1].z);

            glVertex3f(pointsS[j + 1][i + 1].x, pointsS[j + 1][i + 1].y, pointsS[j + 1][i + 1].z);
            glVertex3f(pointsS[j + 1][i].x, pointsS[j + 1][i].y, pointsS[j + 1][i].z);
        }
    }
    glEnd();
}
void updateSphere()
{
    a = 2 * t.q.x;
    sphereRadius = (sqrt(3) / 2.0) * a;
}
void transformToSphere()
{
    updateTriangle();
    updateCylinder();
    updateSphere();
    glutPostRedisplay();
}
void drawOneEight()
{
    cout << "drawing octa" << endl;

    drawTraingle(); // 1
    glPushMatrix();

    glRotatef(90, 1, 0, 0);
    glRotatef(45, 0, 1, 0);
    glTranslatef((t.p.x - (a / 2.0)) / sqrt(2), 0, 0);
    drawCylinder(CylinderHeight, CylinderRadius);
    glPopMatrix();

    glPushMatrix();
    glRotatef(-45, 0, 1, 0);
    glTranslatef((t.p.x - (a / 2.0)) / sqrt(2), 0, 0);
    drawCylinder(CylinderHeight, CylinderRadius);
    glPopMatrix();

    // lower 1 fourth

    //  glTranslatef(-CylinderCenterX, 0.0, 0.0);
    // glRotatef(-45, 0.0, 0.0, 1);
    // glRotatef(90, 0.0, 0.0, 1.0);
    // glRotatef(45, 0.0, 1.0, 0.0);
    // glTranslatef(CylinderCenterX, 0.0, 0.0);
    // drawCylinder(CylinderHeight, CylinderRadius);
    // glTranslatef(-CylinderCenterX, 0.0, 0.0);
    // glRotatef(-45, 0.0, 1.0, 0.0);
    //  glRotatef(-90, 0.0, 0.0, 1.0);
    // cout << "first triangle drawn";

    // glPushMatrix();

    // glTranslatef(t.p.x - (a / 2.0), 0, 0);

    // drawSphere();
    // glPopMatrix();
}
void drawSpheres()
{
    glPushMatrix();
    glTranslatef(t.p.x - (a / 2.0), 0, 0);
    glColor3f(.1, .8, .6);
    drawSphere(); // 1
    glPopMatrix();
    glRotatef(90, 0, 0, 1);
    glPushMatrix();
    glTranslatef(t.p.x - (a / 2.0), 0, 0);
    glColor3f(.7, .8, .3);
    drawSphere(); // 2
    glPopMatrix();

    glRotatef(90, 0, 0, 1);
    glPushMatrix();
    glTranslatef(t.p.x - (a / 2.0), 0, 0);
    glColor3f(.5, .2, .3);
    drawSphere(); // 3
    glPopMatrix();

    glRotatef(90, 0, 0, 1);
    glPushMatrix();
    glTranslatef(t.p.x - (a / 2.0), 0, 0);
    glColor3f(.8, .1, .3);
    drawSphere(); // 4
    glPopMatrix();

    glRotatef(90, 0, 0, 1);
    glRotatef(90, 0, 1, 0);
    glPushMatrix();
    glTranslatef(t.p.x - (a / 2.0), 0, 0);
    glColor3f(.9, .1, .1);
    drawSphere(); // 5
    glPopMatrix();

    glRotatef(180, 0, 1, 0);
    glPushMatrix();
    glTranslatef(t.p.x - (a / 2.0), 0, 0);
    glColor3f(.4, .7, .1);
    drawSphere(); // 6
    glPopMatrix();
    // glTranslatef(t.p.x - (a / 2.0), 0, 0);
    // drawSphere();
    // glRotatef(90, 0, 0, 1);
    // drawSphere();
    // glRotatef(90, 0, 0, 1);
    // drawSphere();
    // glRotatef(90, 0, 0, 1);
    // glRotatef(90, 0, 1, 0);
    // drawSphere();
    // glRotatef(180, 0, 1, 0);
    // drawSphere();
    // glPopMatrix();
}
void DrawHalf()
{
    glColor3f(.2, .7, .8);
    drawOneEight(); // 1
    glRotatef(-90, 0, 0, 1);
    glColor3f(.8, .2, .8);
    drawOneEight(); // 2
    glRotatef(-90, 0, 0, 1);
    glColor3f(.8, .7, .2);
    drawOneEight(); // 3
    glRotatef(-90, 0, 0, 1);
    glColor3f(.8, .6, .6);
    drawOneEight(); // 4
}
void draw_oct()
{ // upper half
    glPushMatrix();
    DrawHalf();

    glRotatef(180, 0, 1, 0);
    DrawHalf();
    glPopMatrix();
    drawSpheres();

    // glPushMatrix();

    // glRotatef(90, 0.0, 1.0, 0.0);
    // glColor3f(0.0, 1.0, 0.0);
    // drawTraingle(); // 2
    // // now for cylinder
    // glRotatef(45, 0.0, 1.0, 0.0);
    // glTranslatef(CylinderCenterX, 0.0, 0.0);
    // drawCylinder(CylinderHeight, CylinderRadius);
    // glTranslatef(-CylinderCenterX, 0.0, 0.0);
    // glRotatef(-45, 0.0, 1.0, 0.0);
    // glRotatef(90, 0.0, 0.0, 1.0);
    // glRotatef(45, 0.0, 1.0, 0.0);
    // glTranslatef(CylinderCenterX, 0.0, 0.0);
    // drawCylinder(CylinderHeight, CylinderRadius);
    // glTranslatef(-CylinderCenterX, 0.0, 0.0);
    // glRotatef(-45, 0.0, 1.0, 0.0);
    // glRotatef(-90, 0.0, 0.0, 1.0);

    // glRotatef(90, 0.0, 1.0, 0.0);
    // glColor3f(0.0, 0.0, 1.0);
    // drawTraingle(); // 3
    // glRotatef(45, 0.0, 1.0, 0.0);
    // glTranslatef(CylinderCenterX, 0.0, 0.0);
    // drawCylinder(CylinderHeight, CylinderRadius);
    // glTranslatef(-CylinderCenterX, 0.0, 0.0);
    // glRotatef(-45, 0.0, 1.0, 0.0);
    // glRotatef(90, 0.0, 0.0, 1.0);
    // glRotatef(45, 0.0, 1.0, 0.0);
    // glTranslatef(CylinderCenterX, 0.0, 0.0);draw_oct()
    // drawCylinder(CylinderHeight, CylinderRadius);
    // glTranslatef(-CylinderCenterX, 0.0, 0.0);
    // glRotatef(-45, 0.0, 1.0, 0.0);
    // glRotatef(-90, 0.0, 0.0, 1.0);

    // glRotatef(90, 0.0, 1.0, 0.0);
    // glColor3f(1.0, 0.0, 1.0);
    // drawTraingle(); // 4
    // glRotatef(45, 0.0, 1.0, 0.0);
    // glTranslatef(CylinderCenterX, 0.0, 0.0);
    // drawCylinder(CylinderHeight, CylinderRadius);
    // glTranslatef(-CylinderCenterX, 0.0, 0.0);
    // glRotatef(-45, 0.0, 1.0, 0.0);
    // glRotatef(90, 0.0, 0.0, 1.0);
    // glRotatef(45, 0.0, 1.0, 0.0);
    // glTranslatef(CylinderCenterX, 0.0, 0.0);
    // drawCylinder(CylinderHeight, CylinderRadius);
    // glTranslatef(-CylinderCenterX, 0.0, 0.0);
    // glRotatef(-45, 0.0, 1.0, 0.0);
    // glRotatef(-90, 0.0, 0.0, 1.0);
    // glPopMatrix();

    // // lower half
    // glPushMatrix();
    // // rotation for lower half
    // glRotatef(90, 1.0, 0.0, 0.0);
    // glColor3f(0.0, 0.0, 1.0); // blue
    // drawTraingle();           // 5

    // glRotatef(90, 0.0, 0.0, 1.0); // green
    // glColor3f(0.0, 1.0, 0.0);
    // drawTraingle(); // 6

    // glRotatef(90, 0.0, 0.0, 1.0);
    // glColor3f(1.0, 0.0, 0.0);
    // drawTraingle(); // draw

    // glRotatef(90, 0.0, 0.0, 1.0);
    // glColor3f(1.0, 0.0, 0.0);
    // drawTraingle(); // 8
    // glPopMatrix();
}

void transFormToOcta()
{
    updateTriangle2();
    updateCylinder();
    updateSphere();
    glutPostRedisplay();
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
    // drawAxes();
    glPushMatrix();
    glRotatef(AngleR, 0, 1, 0);
    draw_oct();
    // drawSphere();
    // glScalef(0.5,0.5,0.5);
    glPopMatrix();
    glutSwapBuffers(); // Render now
}

/* Handler for window re-size event. Called back when the window first appears and
   whenever the window is re-sized with its new width and height */
void reshape(GLsizei width, GLsizei height)
{ // GLsizei for non-negative integer
    // Compute aspect ratio of the new window
    if (height == 0)
        height = 1; // To prevent divide by 0
    GLfloat aspect = (GLfloat)width / (GLfloat)height;

    // Set the viewport to cover the new window
    glViewport(0, 0, width, height);

    // Set the aspect ratio of the clipping area to match the viewport
    glMatrixMode(GL_PROJECTION); // To operate on the Projection matrix
    glLoadIdentity();            // Reset the projection matrix
    /*if (width >= height) {
        // aspect >= 1, set the height from -1 to 1, with larger width
        gluOrtho2D(-1.0 * aspect, 1.0 * aspect, -1.0, 1.0);
    } else {
        // aspect < 1, set the width to -1 to 1, with larger height
        gluOrtho2D(-1.0, 1.0, -1.0 / aspect, 1.0 / aspect);
    }*/
    // Enable perspective projection with fovy, aspect, zNear and zFar
    gluPerspective(45.0f, aspect, 0.1f, 100.0f);
}

void keyboardListener(unsigned char key, int xx, int yy)
{
    double v = 0.25;
    double s;
    double rate = 0.01;
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
    case ',':
        transformToSphere();
        break;
    case '.':
        transFormToOcta();
        break;
    case 'a':
        AngleR = AngleR - 10;
        break;
    case 'd':
        AngleR = AngleR + 10;
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
        pos.x += l.x * .02; // l forward vector
        pos.y += l.y * .02;
        pos.z += l.z * .02;
        break;
    case GLUT_KEY_DOWN: // up arrow key
        pos.x -= l.x * .02;
        pos.y -= l.y * .02;
        pos.z -= l.z * .02;
        break;

    case GLUT_KEY_RIGHT:
        pos.x += r.x * .02; // r right vector
        pos.y += r.y * .02;

        pos.z += r.z * .02;

        break;
    case GLUT_KEY_LEFT:
        pos.x -= r.x * .02;
        ;
        pos.y -= r.y * .02;
        ;
        pos.z -= r.z * .02;
        ;
        break;

    case GLUT_KEY_PAGE_UP:
        pos.x += u.x * .02;
        pos.y += u.y * .02;
        pos.z += u.z * .02;
        break;
    case GLUT_KEY_PAGE_DOWN:
        pos.x -= u.x * .02;
        pos.y -= u.y * .02;
        pos.z -= u.z * .02;
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

/* Main function: GLUT runs as a console application starting at main()  */
int main(int argc, char **argv)
{
    pos.x = 4;
    pos.y = 4;
    pos.z = 4;

    l.x = -4;
    l.y = -4;
    l.z = -4;

    u.x = 0;
    u.y = 1;
    u.z = 0;

    r.x = 1;
    r.y = 0;
    r.z = 0;

    glutInit(&argc, argv);                                    // Initialize GLUT
    glutInitWindowSize(640, 640);                             // Set the window's initial width & height
    glutInitWindowPosition(50, 50);                           // Position the window's initial top-left corner
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB); // Depth, Double buffer, RGB color
    glutCreateWindow("OpenGL 3D Drawing 2");                  // Create a window with the given title
    glutDisplayFunc(display);                                 // Register display callback handler for window re-paint
    glutReshapeFunc(reshape);                                 // Register callback handler for window re-shape

    glutKeyboardFunc(keyboardListener);
    glutSpecialFunc(specialKeyListener);

    initGL();       // Our own OpenGL initialization
    glutMainLoop(); // Enter the event-processing loop
    return 0;
}
