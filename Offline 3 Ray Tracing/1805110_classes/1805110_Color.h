#pragma once
#include <iostream>


using namespace std;
class Color {
    public:
    double r, g, b;

    Color(double p = 0, double q = 0, double w = 0)
    {
        r = p;
        g = q;
        b = w;
    }
    void clip(){
        if(r>1){
            r=1;
        }
        if(g>1){
            g=1;
        }
        if(b>1){
            b=1;
        }
        if(r<0){
            r=0;
        }
        if(g<0){
            g=0;
        }
        if(b<0){
            b=0;
        }
    }

    friend ostream& operator<<(ostream& os, const Color& c)
    {
        os << c.r << " " << c.g << " " << c.b;
        return os;
    }

    friend istream& operator>>(istream& is, Color& c)
    {
        is >> c.r >> c.g >> c.b;
        return is;
    }
    // to add two colors
        Color operator+(const Color &c){
            Color c1;
            c1.r = r + c.r;
            c1.g = g + c.g;
            c1.b = b + c.b;
            return c1;
        }
        // to add two colors
        Color operator+(const double val){
            Color c1;
            c1.r = r + val;
            c1.g = g + val;
            c1.b = b + val;
            return c1;
        }

        // copy constructor
        Color operator=(const Color &c){
            r = c.r;
            g = c.g;
            b = c.b;
            return *this;
        }

        // to multiply a color with a scalar
        Color operator*(const double &d){
            Color c;
            c.r = r * d * 1.0;
            c.g = g * d * 1.0;
            c.b = b * d * 1.0;
            return c;
        }

        // to multiply two colors
        Color operator*(const Color &c){
            Color c1;
            c1.r = r * c.r * 1.0;
            c1.g = g * c.g * 1.0;
            c1.b = b * c.b * 1.0;
            return c1;
        }
        Color getRandomColor(){
            return Color((rand()%100)/100.0,(rand()%100)/100.0,(rand()%100)/100.0);
        }

};