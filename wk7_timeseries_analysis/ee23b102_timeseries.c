/***************************************************************************
 * SANJEEV SUBRAHMANIYAN S B
 * EE1103 NUMERICAL METHODS ASSIGNMENT 
 * TIME SERIES ANALYSIS
 * Last edited on 5 October 2023
 * PURPOSE: To create lorentzian and gaussian time series and analyse them
 * INPUT: Takes arguments from the command line, the values of M = number of peaks, T = time between two peaks, a = width and "Gaussian" or "Lorentzian" depending on which is used for analysis
 * OUTPUT: Prints the average of T, average of a, standard deviation of T and standard deviation of a.
 * CONSTRAINTS : The code is optimized for M<=1000, T<=100, a <=20. Please avoid testing it at further values.
 ****************************************************************************/
 
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>

// Constants for error amplitudes and data points per peak
#define ERRORAMP_T 0.01666
#define ERRORAMP_a 0.01666
#define ERRORAMP_n 0.03333
#define PPL 100

// Struct to represent a peak
struct peak {
    double loc;  // Location
    double amp;  // Amplitude
    double fwhm; // Full Width at Half Maximum
};

// Lorentzian function
double lorentzian(double a, double mT, double t) {
    double p = a * a / ((t - mT) * (t - mT) + a * a);
    return p;
}

// Gaussian function
double gaussian(double a, double mT, double t) {
    double p = exp(-0.5 * ((t - mT) * (t - mT)) / (a * a));
    return p;
}

// Function to generate error for T parameter
double eps_T(double t) {
    double x = (double)rand() / (double)RAND_MAX;
    double y = (double)rand() / (double)RAND_MAX;
    double z0 = sqrt(-2 * log(x)) * cos(2 * M_PI * y);
    return z0 * ERRORAMP_T * t;
}

// Function to generate error for a parameter
double eps_a(double a) {
    double x = (double)rand() / (double)RAND_MAX;
    double y = (double)rand() / (double)RAND_MAX;
    double z0 = sqrt(-2 * log(x)) * cos(2 * M_PI * y);
    return z0 * ERRORAMP_a * a;
}

// Function to generate error for n parameter
double eps_n(double n) {
    double x = (double)rand() / (double)RAND_MAX;
    double y = (double)rand() / (double)RAND_MAX;
    double z0 = sqrt(-2 * log(x)) * cos(2 * M_PI * y);
    return z0 * ERRORAMP_n * n;
}

int main(int argc, char **argv) {
    // Command line arguments
    long M = atol(argv[1]);      // Number of peaks
    double T = atof(argv[2]);    // T parameter
    double a = atof(argv[3]);    // a parameter
    int v = strcmp("Gaussian", argv[4]); // Peak type (Gaussian or Lorentzian)

    long m;
    double t;
    time_t t0;
    time_t t1;
    time_t t2;
    float INCREMENT = T / PPL;
    struct peak *p = (struct peak *)malloc(M * sizeof(struct peak));

    // Generation of the noise (both as a text file for plotting and an array for access in program)
    long x;
    double *e_a = (double *)malloc(M * sizeof(double));
    double *e_T = (double *)malloc(M * sizeof(double));
    double *e_n = (double *)malloc((M * PPL / INCREMENT) * sizeof(double));

    srand(t0);

    for (x = 0; x < M * PPL / INCREMENT; x++) {
        e_n[x] = eps_n(1);
    }

    srand(t1);
    for (x = 0; x < M; x++) {
        e_T[x] = eps_T(T);
    }

    srand(t2);
    for (x = 0; x < M; x++) {
        e_a[x] = eps_a(a);
    }

    // Generation of Lorentzians
    time_t tt;
    srand(tt);
    double y, y1;
    double *dsy = (double *)malloc((M * PPL / INCREMENT) * sizeof(double));
    double *dst = (double *)malloc((M * PPL / INCREMENT) * sizeof(double));

    long long n = 0;
    if (v == 0) {
        for (t = 0; t < M * PPL; t += INCREMENT) {
            y = 0;
            y1 = 0;
            for (m = 1; m < M; m++) {
                y1 = gaussian(a + e_a[m], m * T + e_T[m], t);
                y += y1;
            }
            dst[n] = t;
            dsy[n] = y;
            n++;
        }
    } else {
        for (t = 0; t < M * PPL; t += INCREMENT) {
            y = 0;
            y1 = 0;
            for (m = 1; m < M; m++) {
                y1 = lorentzian(a + e_a[m], m * T + e_T[m], t);
                y += y1;
            }
            dst[n] = t;
            dsy[n] = y;
            n++;
        }
    }

    long k;
    for (k = 0; k < M * PPL / INCREMENT; k++) {
        dsy[k] += e_n[k]; // Addition of normal noise
    }

    // Window average filter application with width = 3;
    long i, j;
    double sumelements = 0;
    for (i = 1; i < M * PPL / INCREMENT - 1; i++) {
        sumelements = 0;
        for (j = -1; j <= 1; j++) {
            sumelements += dsy[i + j];
        }
        dsy[i] = sumelements / 3;
    }

    double threshold = 10.0 * (0.1 / 3.0); // Threshold is defined as 6 times the standard deviation of the noise
    double min_t, max_t, max_y;
    int val1 = 1, val2 = 1;
    for (i = 0; i < M; i++) {
        max_y = 0;
        min_t = 0;
        max_t = 0;
        val1 = 1;
        val2 = 1;
        for (j = 0; j < PPL; j++) {
            if (dsy[i * PPL + j] > max_y)
                max_y = dsy[i * PPL + j];
            if (dsy[i * PPL + j] < threshold && dsy[i * PPL + j + 1] > threshold && val1) {
                min_t = dst[i * PPL + j + 1];
                val1 = 0;
            }
            if (dsy[i * PPL + j] > threshold && dsy[i * PPL + j + 1] < threshold && val2) {
                max_t = dst[i * PPL + j];
                val2 = 0;
            }
        }
        p[i].loc = (max_t + min_t) / 2.0;
        p[i].amp = max_y;
    }

    double *min = (double *)malloc(M * sizeof(double));
    double *max = (double *)malloc(M * sizeof(double));
    for (i = 0; i < M; i++) {
        val1 = 1;
        for (j = 0; j < PPL; j++) {
            if (dsy[i * PPL + j] < p[i].amp / 2.0 && dsy[i * PPL + j + 1] > p[i].amp / 2.0 && val1) {
                min[i] = dst[i * PPL + j];
                val1 = 0;
            }
        }
    }
    for (i = 0; i < M; i++) {
        val2 = 1;
        for (j = 0; j < PPL; j++) {
            if (dsy[i * PPL + j] > p[i].amp / 2.0 && dsy[i * PPL + j + 1] < p[i].amp / 2.0 && val2) {
                max[i] = dst[i * PPL + j];
                val2 = 0;
            }
        }
    }
    for (i = 0; i < M - 1; i++) {
        p[i].fwhm = max[i + 1] - min[i];
        if (p[i].fwhm > 20 || p[i].fwhm < 0)
            p[i].fwhm = 5;
    }

    double sumt = 0, suma = 0, sumsqt = 0, sumsqa = 0;
    for (i = 0; i < M - 2; i++) {
        sumt += p[i + 1].loc - p[i].loc;
    }
    if (v == 0) {
        for (i = 0; i < M - 1; i++) {
            suma += (p[i].fwhm / 2.0) * sqrt(2 * log(2));
        }
    } else {
        for (i = 0; i < M - 1; i++) {
            suma += p[i].fwhm / 2.0;
        }
    }
    double avga, avgt;
    avga = suma / (double)(M - 1);
    avgt = sumt / (double)(M - 1);
    for (i = 0; i < M - 2; i++) {
        sumsqt += pow(avgt - p[i + 1].loc + p[i].loc, 2);
    }
    for (i = 0; i < M - 1; i++) {
        sumsqa += pow(avga - (p[i].fwhm / 2), 2);
    }
    double stdeva = sqrt(sumsqa / (double)(M - 2));
    double stdevt = sqrt(sumsqt / (double)(M - 2));
    printf("%lf %lf %lf %lf \n", avgt, avga, stdevt, stdeva);

    free(min);
    free(max);
    free(dst);
    free(dsy);
    free(p);

    return 0;
}

