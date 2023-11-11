
#include <bits/stdc++.h>
using namespace std;

struct point
{
public:
    double x, y, z;
    point()
    {
        x = 0;
        y = 0;
        z = 0;
    }

    point(double a, double b, double c)
    {
        x = a;
        y = b;
        z = c;
    }
    // copy constructor
    point(const point &p)
    {
        this->x = p.x;
        this->y = p.y;
        this->z = p.z;
    }

    void normalize()
    {
        double len = sqrt(this->x * this->x + this->y * this->y + this->z * this->z);
        if (len != 0)
        {
            this->x = this->x / len;
            this->y = this->y / len;
            this->z = this->z / len;
        }
        else
        {
            cout << "Error- Division by 0" << endl;
        }
    }
    point operator+(const point &p)
    {
        point res;
        res.x = this->x + p.x;
        res.y = this->y + p.y;
        res.z = this->z + p.z;
        return res;
    }

    point operator-(const point &p)
    {
        point res;
        res.x = this->x - p.x;
        res.y = this->y - p.y;
        res.z = this->z - p.z;
        return res;
    }
    point operator*(const double &d)
    {
        point res;
        res.x = this->x * d;
        res.y = this->y * d;
        res.z = this->z * d;
        return res;
    }
    point operator^(const point &p)
    {
        point res;
        res.x = this->y * p.z - this->z * p.y;
        res.y = this->z * p.x - this->x * p.z;
        res.z = this->x * p.y - this->y * p.x;
        return res;
    }
    double operator*(const point &p)
    {
        return this->x * p.x + this->y * p.y + this->z * p.z;
    }

    friend point applyRodriguesFormula(point p, point a, double theta)
    {
        point res;
       
        res = p * cos(theta / 180.0 * M_PI) + a * (a * p) * (1 - cos(theta / 180.0 * M_PI)) + (a ^ p) * sin(theta / 180.0 * M_PI);
        return res;
    }

    friend ostream &operator<<(ostream &out, point &p)
    {
        out << fixed << setprecision(7) << p.x << " " << p.y << " " << p.z;
        return out;
    }

    friend ifstream &operator>>(ifstream &in, point &p)
    {
        in >> p.x >> p.y >> p.z;
        return in;
    }

    
};
