#include <bits/stdc++.h>
#include <iostream>
#include "point.cpp"

using namespace std;

class matrix
{
public:
    double mat[4][4];

    matrix()
    {
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                mat[i][j] = 0;
            }
        }
    }
    // copy constructor
    matrix(const matrix &m)
    {
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                this->mat[i][j] = m.mat[i][j];
            }
        }
    }
    matrix getIdentityMatrix()
    {
        matrix m;
        for (int i = 0; i < 4; i++)
        {
            m.mat[i][i] = 1;
        }
        return m;
    }

    matrix operator*(const matrix &m)
    {
        matrix res;
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                for (int k = 0; k < 4; k++)
                {
                    res.mat[i][j] += this->mat[i][k] * m.mat[k][j];
                }
            }
        }
        return res;
    }
    point operator*(const point &p)
    {
        point res;
          double sum = 0;
        for(int i = 0; i < 4; i++){
            sum =    (this->mat[i][0] * p.x) + 
                            (this->mat[i][1] * p.y) + 
                            (this->mat[i][2] * p.z) + 
                            (this->mat[i][3] * 1);

            if(i == 0) res.x = sum;
            else if(i == 1) res.y = sum;
            else if(i == 2) res.z = sum; 
            else {
                res.x /= sum;
                res.y /= sum;
                res.z /= sum;
            }
                
        }
        
        return res;
    }
    matrix getTranslationMatrix(double tx, double ty, double tz)
    {
        matrix m = this->getIdentityMatrix();
        m.mat[0][3] = tx;
        m.mat[1][3] = ty;
        m.mat[2][3] = tz;
        return m;
    }
    matrix getScalingMatrix(double sx, double sy, double sz)
    {
        matrix m = this->getIdentityMatrix();
        m.mat[0][0] = sx;
        m.mat[1][1] = sy;
        m.mat[2][2] = sz;
        return m;
    }
    matrix getRotationMatrix(double angle, point p)
    {
        matrix m = this->getIdentityMatrix();
        p.normalize();
        cout<<"after notmalize "<<p<<endl;
        cout<<"c1 "<<endl;
        cout<<applyRodriguesFormula(point(1, 0, 0), p, angle).x<< "  "<<applyRodriguesFormula(point(1, 0, 0), p, angle).y<< "  "<<applyRodriguesFormula(point(1, 0, 0), p, angle).z<<endl;
        m.mat[0][0] = applyRodriguesFormula(point(1, 0, 0), p, angle).x;
        m.mat[1][0] = applyRodriguesFormula(point(1, 0, 0), p, angle).y;
        m.mat[2][0] = applyRodriguesFormula(point(1, 0, 0), p, angle).z;
        cout<<"c2 "<<endl;
        cout<<applyRodriguesFormula(point(0, 1, 0), p, angle).x<< "  "<<applyRodriguesFormula(point(0, 1, 0), p, angle).y<< "  "<<applyRodriguesFormula(point(0, 1, 0), p, angle).z<<endl;
        m.mat[0][1] = applyRodriguesFormula(point(0, 1, 0), p, angle).x;
        m.mat[1][1] = applyRodriguesFormula(point(0, 1, 0), p, angle).y;
        m.mat[2][1] = applyRodriguesFormula(point(0, 1, 0), p, angle).z;
       cout<<"c3 "<<endl;
        cout<<applyRodriguesFormula(point(0, 0, 1), p, angle).x<< "  "<<applyRodriguesFormula(point(0, 0, 1), p, angle).y<< "  "<<applyRodriguesFormula(point(0, 0, 1), p, angle).z<<endl;  
        // m.mat[0][2] = applyRodriguesFormula(point(0, 0, 1), p, angle).x;
        m.mat[1][2] = applyRodriguesFormula(point(0, 0, 1), p, angle).y;
        m.mat[2][2] = applyRodriguesFormula(point(0, 0, 1), p, angle).z;

        return m;
    }
    
    matrix getViewMatrix(point eye, point look, point up)
    {
        matrix m = this->getIdentityMatrix();
        point l = look - eye;
     
        l.normalize();
       
        point r = l ^ up;
       
        r.normalize();
     
        point u = r ^ l;
        cout << "u " << u << endl;
        m.mat[0][0] = r.x;
        m.mat[0][1] = r.y;
        m.mat[0][2] = r.z;
        m.mat[0][3] = -r.x * eye.x - r.y * eye.y - r.z * eye.z;

        m.mat[1][0] = u.x;
        m.mat[1][1] = u.y;
        m.mat[1][2] = u.z;
        m.mat[1][3] = -u.x * eye.x - u.y * eye.y - u.z * eye.z;

        m.mat[2][0] = -l.x;
        m.mat[2][1] = -l.y;
        m.mat[2][2] = -l.z;
        m.mat[2][3] = l.x * eye.x + l.y * eye.y + l.z * eye.z;

        m.mat[3][0] = 0;
        m.mat[3][1] = 0;
        m.mat[3][2] = 0;
        m.mat[3][3] = 1;

        return m;
    }

    void printMatrix()
    {
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                cout << mat[i][j] << " ";
            }
            cout << endl;
        }
    }

    matrix getProjectionMatrix(double fovY, double aspectRatio, double near, double far)
    {
        matrix m = this->getIdentityMatrix();
        double fovX = fovY * aspectRatio;
        double t = tan(fovY / 2 * (M_PI / 180)) * near;
        double r = tan(fovX / 2 * (M_PI / 180)) * near;
        m.mat[0][0] = near / r;
        m.mat[1][1] = near / t;
        m.mat[2][2] = -(far + near) / (far - near);
        m.mat[2][3] = -(2 * far * near) / (far - near);
        m.mat[3][2] = -1;
        m.mat[3][3] = 0;
        return m;
    }
};
