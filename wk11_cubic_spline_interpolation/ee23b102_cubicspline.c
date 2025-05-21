/***************************************************************************
 * SANJEEV SUBRAHMANIYAN S B
 * EE1103 NUMERICAL METHODS ASSIGNMENT 
 * CUBIC SPLINE INTERPOLATION	
 * Last edited on 24 OCTOBER 2023
 * PURPOSE: To read the polynomial order(n), used to create a cubic spline interpolation of n+1 values and estimate the value of x0 using it
 * INPUT: Takes arguments from the command line, the order of polynomial(npoints-1), the point to be estimated, and the start and ending points as optional arguments. By default the start and end are assumed to be -1 and 1.
 * OUTPUT: Prints the value interpolated at the given point
 * CONSTRAINTS : The point to be estimated must lie between -1 and 1 by default and between the given points if specified.
 ****************************************************************************/
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define TOL 1e-6

double runge(double x) {
    return 1 / (1 + 25 * x * x);
}

void set0(double* xArr, double* yArr, int p, int q, double datarr[], int n) {//here p is the index of the interval, and q is the index of the point that is being used to evaluate. note that datarr[j] must be passed to set the coefficients for the (j+1)th row
    datarr[4 * p + 0] = pow(xArr[q], 3);
    datarr[4 * p + 1] = pow(xArr[q], 2);
    datarr[4 * p + 2] = pow(xArr[q], 1);
    datarr[4 * p + 3] = 1;
    datarr[4 * n] = yArr[q];
}

void set1(double* xArr, int p, int q, int r, double* datarr, int n) {//here p is the index of the interval, and q is the index of the point that is being used to evaluate note that datarr[j] must be passed to set the coefficients for the (j+1)th row
    datarr[4 * p + 0] = 3 * pow(xArr[q], 2);
    datarr[4 * p + 1] = 2 * pow(xArr[q], 1);
    datarr[4 * p + 2] = 1;
    datarr[4 * p + 3] = 0;
    datarr[4 * r + 0] = -3 * pow(xArr[q], 2);
    datarr[4 * r + 1] = -2 * pow(xArr[q], 1);
    datarr[4 * r + 2] = -1;
    datarr[4 * r + 3] = 0;
    datarr[4 * n] = 0;
}

void set2(double* xArr, int p, int q, int r, double* datarr, int n) {//here p is the index of the interval, and q is the index of the point that is being used to evaluate note that datarr[j] must be passed to set the coefficients for the (j+1)th row
    datarr[4 * p + 0] = 6 * xArr[q];
    datarr[4 * p + 1] = 2;
    datarr[4 * p + 2] = 0;
    datarr[4 * p + 3] = 0;
    datarr[4 * r + 0] = -6 * xArr[q];
    datarr[4 * r + 1] = -2;
    datarr[4 * r + 2] = 0;
    datarr[4 * r + 3] = 0;
    datarr[4 * n] = 0;
}

void set3(double* xArr, int p, int q, double* datarr, int n) {
    datarr[4 * p + 0] = 6 * xArr[q];
    datarr[4 * p + 1] = 2;
    datarr[4 * p + 2] = 0;
    datarr[4 * p + 3] = 0;
    datarr[4 * n] = 0;
}

void normalize(double** datarr, int n) {//function that normalizes the coefficients in the file to prevent errors
    double max = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n + 1; j++) {
            if (fabs(datarr[i][j]) >= max) {//finding the maximum value of the coefficients and divides the set by the max value
                max = fabs(datarr[i][j]);
            }
        }
        for (int j = 0; j < n + 1; j++) {
            datarr[i][j] /= max;
        }
    }
}

int findpivot(int start, int n, int ind, double** datarr) {//function used to find the pivot for one operation and returns the location where the pivot was swapped
    double abs = 0;
    int dummy = 0;

    for (int i = start; i < n; i++) {
        if (fabs(datarr[i][ind]) >= abs) {
            abs = fabs(datarr[i][ind]);
            dummy = i;
        }
    }
    return dummy;
}

void swaprows(int start, int dum, int n, double** datarr) {//start is the row index with which the pivot row(dum) is swapped with
    double temp;
    for (int i = 0; i < n + 1; i++) {
        temp = datarr[dum][i];
        datarr[dum][i] = datarr[start][i];
        datarr[start][i] = temp;
    }
}

void eliminate(int index, int rowno, int start, int n, double** datarr) {//index is the position of the element in the array, that is index = 2 is the coefficient of the third variable and rowno is the number of the row (0 to  n-1)
    if (fabs(datarr[start][index]) > TOL) {
        double factor = datarr[rowno][index] / datarr[start][index];
        for (int i = 0; i < n + 1; i++) {
            datarr[rowno][i] -= datarr[start][i] * factor;
        }
    }
}

void final(int n, double** datarr) {//the function that solves the set of equations and updates the final matrix
    for (int p = 0; p < n - 1; p++) {
        int dum = findpivot(p, n, p, datarr);
        swaprows(p, dum, n, datarr);
        for (int q = p + 1; q < n; q++) {
            eliminate(p, q, p, n, datarr);
        }
    }
}

void solve(double* datarr, double* solution, int solind, int n) {//solves the set and updates the solution array
    double sum = 0;
    for (int i = n; i > solind; i--) {
        sum += datarr[i] * solution[i];
    }
    if (fabs(datarr[solind]) > TOL) {
        solution[solind] = (datarr[n] - sum) / datarr[solind];
    }
    else {
        solution[solind] = 0;
    }
}

void reversesub(int n, double** datarr, double* solution) {
    for (int j = 0; j < n; j++) {
        solution[j] = 0;
    }
    for (int i = n - 1; i >= 0; i--) {
        solve(datarr[i], solution, i, n);
    }
}


int main(int argc, char** argv) {
    int n = atoi(argv[1]) + 1;
    double xstart = -1, xend = 1;
    double x0 = atof(argv[2]);
    //check if the start and end are given as arguments in the line and accordingly modify the default for those
    if (argc != 3) {
        xstart = atof(argv[3]);
        xend = atof(argv[4]);
    }

    double xArr[n], yArr[n];
    double step = (xend - xstart) / (n - 1);
    //initialization of the data arrays which contain the points through which the interpolation is performed
    for (int i = 0; i < n; i++) {
        xArr[i] = xstart + step * i;
        yArr[i] = runge(xArr[i]);
        
    }
    n--;

    double** datarr = (double**)malloc(4 * n * sizeof(double*));
    for (int i = 0; i < 4 * n; i++) {
        datarr[i] = (double*)malloc((4 * n + 1) * sizeof(double));
    }
    for(int i = 0; i < 4 * n; i++){
    	for(int j = 0; j < 4 * n + 1; j++){
    		datarr[i][j] = 0;
    	}
	}
    int cnt = 0;
    //implementation of the constraints that use the fact that the function is continuous and passes through the given points and the derivatives are alos continuous
    for (int i = 0; i < n - 1; i++) {
        set0(xArr, yArr, i, i + 1, datarr[cnt], n);
        cnt++;
        set0(xArr, yArr, i + 1, i + 1, datarr[cnt], n);
        cnt++;
    }

    for (int i = 0; i < n - 1; i++) {
        set1(xArr, i, i + 1, i + 1, datarr[cnt], n);
        cnt++;
    }

    for (int i = 0; i < n - 1; i++) {
        set2(xArr, i, i + 1, i + 1, datarr[cnt], n);
        cnt++;
    }

    set3(xArr, 0, 0, datarr[cnt], n);
    cnt++;
    set3(xArr, n - 1, n, datarr[cnt], n);
    cnt++;
    set0(xArr, yArr, 0, 0, datarr[cnt], n);
    cnt++;
    set0(xArr, yArr, n - 1, n, datarr[cnt], n);
    
    double solution[4 * n];
    
    normalize(datarr, 4 * n);
    final(4 * n, datarr);
    reversesub(4 * n, datarr, solution);
	
    double v;
    int l = (x0 - xstart) / step;
    /*if(floor((x0 - xstart) / step) == (x0 - xstart)/step){
    	v = yArr[l];
    }*/
    //else{
    	v = solution[4 * l] * pow(x0,3) + solution[4 * l + 1] * pow(x0,2) + solution[4 * l + 2] * (x0) + solution[4 * l + 3];
    //}
    printf("%lf\n", v);
    
    for (int i = 0; i < 4 * n; i++) {
        free(datarr[i]);
    }
    free(datarr);

    return 0;
}


