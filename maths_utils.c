#include "maths_utils.h"
#include <avr/io.h>
double m_atan(double x)
{
    double res = 0;
    double exp = x;
    for(int n = 0; n < TaylorInf;++n)      
    {
        double minus = n%2 == 0? 1: -1; 
        res += minus * exp / (2*n+1);
        exp *= x*x;
    }

    return res;
}

double get_angle_of(double x, double y)
{
    if(ABS(x) <= TOLERANCE) 
        return PI/2 * (1+2*(y<0));
    if(ABS(y) <= TOLERANCE) 
        return x > 0? 0: PI;
    if(ABS(x-y) <= TOLERANCE)
    {
        if(x > 0 && y > 0) return PI/4;
        if(x < 0 && y > 0) return 3*PI/4;
        if(x < 0 && y < 0) return 5*PI/4;
        else return 7*PI/4;
    }

    double reduced_theta = 0;
    if(ABS(y) <= ABS(x)) reduced_theta = m_atan(y/x);
    else
    {
        double theta_p = m_atan(x/y);
        reduced_theta = theta_p < 0? -PI/2 -theta_p : PI/2-theta_p;
    }
    if (x > 0)
        return y > 0? reduced_theta : 2*PI+reduced_theta;
    
    return PI+reduced_theta ;
}