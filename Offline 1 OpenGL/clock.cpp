
#define _USE_MATH_DEFINES
#include <windows.h> // for MS Windows
#include <GL/glut.h> // GLUT, include glu.h and gl.h
#include <cmath>
#include <iostream>
#include <time.h>

using namespace std;

struct point
{
    GLfloat x, y;
};

GLdouble hourHandLength = 0.2;
GLdouble MinuteHandLength = 0.25;
GLdouble SecondHandLength = 0.3;
GLdouble hourAngle = 30;
GLdouble minuteAngle = 60;
GLdouble secondAngle = 120;
GLdouble clockHourAngle = 30;
GLdouble clockMinuteAngle = 60;
GLdouble clockSecondAngle = 120;
GLdouble millis = 0;
GLdouble prevMillis = 0;
int flag = 1;

time_t rawTime;
struct tm *timeInfo;
struct timeb tb;

/* Initialize OpenGL Graphics */
void initGL()
{
    // Set "clearing" or background color
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Black and opaque
    glEnable(GL_DEPTH_TEST);              // Enable depth testing for z-culling
}
void drawCircle(double cx, double cy, double r)
{
    glBegin(GL_LINE_LOOP); // All vertices form a single loop of single pixel width
    // Light-blue

    for (float theta = 0; theta < 360; theta += 10)
    {
        float x = cx + r * cos(theta / 180 * M_PI);
        float y = cy + r * sin(theta / 180 * M_PI);
        // cout << "normal theta" << theta << endl;
        // cout << "x y " << x << " " << y << endl;
        glVertex2f(x, y);
    }

    glEnd();
}

void drawPendulam()
{

    double pendulamAngle = 16 * cos((millis / 1000) * M_PI);
    if (prevMillis > 900 && millis < 100)
    {
        flag = -flag;
    }
    pendulamAngle = flag * pendulamAngle;
    glPushMatrix();
    glTranslatef(0.0, 0.1, 0);
    glRotatef(pendulamAngle, 0, 0, 1);
    glColor3f(.8, .9, .9);
    glBegin(GL_LINES);
    glVertex2d(0.0, 0.0);
    glVertex2d(0.0, -0.3);
    glEnd();
    drawCircle(0.0, -0.4, 0.1);

    glPopMatrix();
}

void display()
{
    double cx, cy, r;
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT); // Clear the color buffer (background)
                                  // glMatrixMode(GL_MODELVIEW);   // To operate on Model-View matrix

    glColor3f(0.5f, 0.5f, 1.0f); // glLoadIdentity();
    drawCircle(0.0, 0.5, 0.4);
    cx = 0.0;
    cy = 0.5;
    r = .4;
    glColor3f(0.5f, 0.8f, 1.0f);
    for (float theta = 0; theta < 360; theta += 30)
    {
        //
        // cout << "for theta" << theta << endl;
        float x = cx + r * cos(theta / 180 * M_PI);
        float y = cy + r * sin(theta / 180 * M_PI);
        float x1 = cx + (r - .05) * cos(theta / 180 * M_PI);
        float y1 = cy + (r - .05) * sin(theta / 180 * M_PI);
        // cout << "x y " << x << " " << y << "x1 y1 " << x1 << " " << y1 << endl;
        glLineWidth(2);    // Make next lines wider
        glBegin(GL_LINES); // Each set of 2 vertices form a line of single pixel width
                           //  glColor3f(1.0f, 1.0f, 1.0f); // White
        glVertex2f(x, y);
        glVertex2f(x1, y1);

        glEnd();
    }

    // write the timer function here

    // draw hour hand
    float x1 = cx + hourHandLength * cos(clockHourAngle / 180 * M_PI);
    float y1 = cy + hourHandLength * sin(clockHourAngle / 180 * M_PI);
    glLineWidth(4);
    glColor3f(0.2f, 0.8f, .9f);
    glBegin(GL_LINES);
    glVertex2f(cx, cy);
    glVertex2f(x1, y1);

    glEnd();
    // draw minuteHand
    float x2 = cx + MinuteHandLength * cos(clockMinuteAngle / 180 * M_PI);
    float y2 = cy + MinuteHandLength * sin(clockMinuteAngle / 180 * M_PI);
    glLineWidth(4);
    glColor3f(0.8f, 0.7f, .6f);
    glBegin(GL_LINES);
    glVertex2f(cx, cy);
    glVertex2f(x2, y2);

    glEnd();
    // draw secondHand
    float x3 = cx + SecondHandLength * cos(clockSecondAngle / 180 * M_PI);
    float y3 = cy + SecondHandLength * sin(clockSecondAngle / 180 * M_PI);
    glLineWidth(4);
    glColor3f(0.4f, 0.8f, .3f);
    glBegin(GL_LINES);

    glVertex2f(cx, cy);
    glVertex2f(x3, y3);

    glEnd();

    // drawPendulamBox
    float x4 = cx - r * sin(39.0 / 180.0 * M_PI);
    float y4 = cy - r * cos(39.0 / 180.0 * M_PI);
    float x5 = cx + r * sin(39.0 / 180.0 * M_PI);
    float y5 = cy - r * cos(39.0 / 180.0 * M_PI);
    glColor3f(0.9f, 0.8f, .9f);
    glBegin(GL_LINES);
    glVertex2f(x4, y4);
    glVertex2f(x4, y4 - .7);
    glEnd();
    glBegin(GL_LINES);
    glVertex2f(x5, y5);
    glVertex2f(x5, y5 - .7);
    glEnd();
    glBegin(GL_LINES);
    glVertex2f(x4, y4 - .7);
    glVertex2f(x5, y5 - .7);
    glEnd();

    drawPendulam();

    glFlush();
}
/* Main function: GLUT runs as a console application starting at main()  */
void timer(int value)
{
    time(&rawTime);
    ftime(&tb);
    timeInfo = localtime(&rawTime);
    int hour = timeInfo->tm_hour;
    int minute = timeInfo->tm_min;
    int second = timeInfo->tm_sec;
    prevMillis = millis;
    millis = tb.millitm;
    // cout << "time in millisecond " << millis << endl;
    hourAngle = (hour % 12) * 30 + minute * 0.5 + second * (1.0 / 120);
    clockHourAngle = -hourAngle + 90;
    minuteAngle = minute * 6 + second * (1.0 / 10);
    clockMinuteAngle = -minuteAngle + 90;
    secondAngle = second * 6;
    clockSecondAngle = -secondAngle + 90;
    glutPostRedisplay();
    glutTimerFunc(15, timer, 1);
}

int main(int argc, char **argv)
{

    glutInit(&argc, argv);                   // Initialize GLUT
    glutInitWindowSize(640, 640);            // Set the window's initial width & height
    glutInitWindowPosition(50, 50);          // Position the window's initial top-left corner
                                             // Depth, Double buffer, RGB color
    glutCreateWindow("OpenGL 3D Drawing 2"); // Create a window with the given title
    glutDisplayFunc(display);
    glutTimerFunc(15, timer, 1); // Register display callback handler for window re-paint
                                 // glutReshapeFunc(reshape);                                 // Register callback handler for window re-shape

    // glutKeyboardFunc(keyboardListener);
    // glutSpecialFunc(specialKeyListener);

    // Our own OpenGL initialization
    glutMainLoop(); // Enter the event-processing loop
    return 0;
}
