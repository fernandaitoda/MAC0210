/********************************** 
		EP3 - MAC0210 
	
	    Fernanda Itoda
	       10740825
***********************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>

#define SEED 37490234
#define SIN 1
#define X3 2
#define EX 3
#define PI 4


void newtonDividedDiff (float x[], float y[][10], int n)
{
    for (int i = 1; i < n; i++)
        for (int j = 0; j < n-1; j++)
            y[j][i] = (y[j][i-1] - y[j+1] [i-1]) / (x[j] - x[i+j]); 
}

float evalP (float x[], float y[][10], float x0, int n)
{
    float product, res;

    // first term in sum f0
    res = y[0][0];

    for (int i = 1; i < n; i++){
        product = 1;
        for (int j = 0; j < i; j++)
            product *= (x0-x[j]);
        res += (product * y[0][i]);
    }
    return res;
}

float compositeTrap (float x[], float y[][10], int n)
{
    float res, h, x0;
    int r = 30;
    h = (x[n-1] - x[0]) / r; 

    res = 0;
    x0 = x[0];
    for (int i = 0; i < r; i++){
        res += evalP (x, y, x0, n) + evalP (x, y, x0+h, n);
        x0 += h;
    }
    res *= h / 2;
    return res;
}

float compositeSimpson (float x[], float y[][10], int n)
{
    float res, h, x0;
    int r = 30;
    h = (x[n-1] - x[0]) / r;

    res = 0;
    x0 = x[0];
    for (int i = 0; i < r; i+=2){
        res += evalP (x, y, x0, n) + 4 * evalP (x, y, x0+h, n) + evalP (x, y, x0+2*h, n);
        x0 = x0+2*h;
    }

    res *= h / 3;
    return res;
}

double monteCarlo (int n, int id)
{
    // SIN-> sin(x); a = 0, b = 1
    // X3-> x^3; a = 3, b = 7
    // EX-> e^(-x); a = 0, b = inf
    // PI-> aprox pi

    double x, y, z;
    srand (SEED);
    if (id == SIN){
        y = 0;
        for (int i = 0; i < n; i++){
            x = (double) rand()/RAND_MAX;
            y += sin (x) / n;
        }
        return y * (1 - 0);
    }

    else if (id == X3){
        y = 0;
        for (int i = 0; i < n; i++){
            x = (double)rand()/RAND_MAX * (7-3) + 3;
            y += pow (x, 3) / n;
        }
        return y * (7 - 3);
    }

    else if (id == EX){
        y = 0;
        for (int i = 0; i < n; i++){
            x = (double)rand()/RAND_MAX;
            y += (exp (1-(1/x)) / pow (x, 2)) / n;
        }
        return y * (1 - 0);
    }

    else if (id == PI){
        int cnt = 0;
        for (int i = 0; i < n; i++){
            x = (double) rand()/RAND_MAX;
            y = (double) rand()/RAND_MAX;
            z = x*x + y*y;
            if (z <= 1) 
                cnt++;
        }
        return (double)cnt / n * 4;
    }

}

int main()
{
    int nPoints = 7;
    float x[] = {0, 5, 10, 15, 20, 25, 30};
    float fxcos[10][10];

    float aux[] = {0.0000, 1.5297, 9.5120, 8.7025, 2.8087, 1.0881, 0.3537};
    for (int i = 0; i < nPoints; i++)
        fxcos[i][0] =  aux[i];
     
    // NEWTON INTERPOLATION
    newtonDividedDiff (x, fxcos, nPoints);

    // COMPOSITE TRAPEZOIDAL METHOD
    float Itc = compositeTrap (x, fxcos, nPoints);

    // COMPOSITE SIMPSON METHOD
    float Isc = compositeSimpson (x, fxcos, nPoints);

    printf ("********** PART 1 - RESULTS ********** \n");
    printf ("Composite Trapezoidal: %f \n", Itc);
    printf ("Composite Simpson: %f \n", Isc);

    // MONTE CARLO METHODS
    int n;
    printf ("\nEnter the value of n: ");
    scanf ("%d", &n);

    // sin(x)
    double r1 = monteCarlo (n, SIN);
    
    // x^3
    double r2 = monteCarlo (n, X3);
    
    // e^(-x)
    double r3 = monteCarlo (n, EX);
    
    // value of pi
    double r4 = monteCarlo (n, PI);

    printf ("********** PART 2 - RESULTS *********** \n");
    printf ("Value of trials: %d\n", n);
    printf ("Integral of sin(x), from 0 to 1: %lf\n", r1);
    printf ("Integral of x^3, from 3 to 7: %lf\n", r2);
    printf ("Integral of e^(-x), from 0 to infinit : %lf\n", r3);
    printf ("Estimate of pi: %lf\n", r4);
    
    return 0;
}