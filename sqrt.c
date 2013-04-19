#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define EPS 0.000000001

double mabs(double x) 
{
    return x < 0 ? -x : x;
}

double msqrt(double x)
{
    double l = 0;
    double r = x;
    double n, k;
    do {
        n = (l + r) / 2;
        k = n*n;
        if (k - x > EPS) {
            r = n;
        }
        else {
            l = n;
        }
    } while (mabs(k - x) > EPS);
    return n;
}

int main(int argc, char ** argv) {
    msqrt(atof(argv[1]));
}

