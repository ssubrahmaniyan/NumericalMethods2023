/***************************************************************************
 * SANJEEV SUBRAHMANIYAN S B
 * EE1103 NUMERICAL METHODS ASSIGNMENT 
 * LORENZIAN VS GAUSSIAN	
 * Last edited on 27 OCTOBER 2023
 * PURPOSE: To read N and standard deviation of the noise and to print out the parameters of the curve fit gaussian for the lorenzian 
 * INPUT: Takes as argument N and standard deviation of the noise
 * OUTPUT: Prints parameters of the fit curve, sigmaN, a, sigmaG, r^2.
 ****************************************************************************/
#include<stdio.h>
#include<math.h>
#include<stdlib.h>

#define POINTSPERUNIT 100
#define START -2.0
#define END 2.0
#define GNUPLOT "gnuplot -persist > /dev/null 2>&1"

double eps_N(double t, float sigmaN) {
    double x = (double)rand() / (double)RAND_MAX;
    double y = (double)rand() / (double)RAND_MAX;
    double z0 = sqrt(-2 * log(x)) * cos(2 * M_PI * y);
    return z0 * sigmaN;
}

double lorenzian(double t){
	return 1/(1 + 25 * t * t);
}
//the program takes as arguments the number of points in the noise timeseries, and the standard deviation of the noise
int main(int argc, char** argv){

	int N = atoi(argv[1]);
	float sigmaN = atof(argv[2]);
	FILE* fp = fopen("noise.txt", "w");
	float t = 0;
	double eps = 0;
	float inc = (END - START)/N; //the increment for the timeseries
	for(float i = START; i <= END; i+=inc){
		eps = eps_N(i, sigmaN) + lorenzian(i);
		fprintf(fp, "%f %lf\n", i, eps);
	}

	fclose(fp);
	
	double A, sigmaG, R2;
	FILE* gpp = popen(GNUPLOT, "w");//creates a pipe to read the parameters from the gnuplot window
	//the following commands run on the gnuplot terminal and return the parameters into a temp file
	
	fprintf(gpp, "f(x) = a * exp(-1*(x**2)/2*(g**2))\n");
        fprintf(gpp, "mean(x) = m\n");
        fprintf(gpp, "fit mean(x) \"noise.txt\" using 1:2 via m\n");
        fprintf(gpp, "SST = FIT_WSSR/(FIT_NDF + 1)\n");
        fprintf(gpp, "fit f(x) \"noise.txt\" using 1:2 via a, g\n");
        fprintf(gpp, "SSE = FIT_WSSR/(FIT_NDF)\n");
        fprintf(gpp, "SSR = SST - SSE\n");
        fprintf(gpp, "R2 = SSR/SST\n");
        fprintf(gpp, "set print \"temp.txt\" \n");
	fprintf(gpp, "print  a, g, R2\n"); 
	fprintf(gpp, "exit\n");
	pclose(gpp);
	
	FILE* fpp = fopen("temp.txt","r");
	fscanf(fpp,"%lf %lf %lf", &A, &sigmaG, &R2);//reading from temp the parameters that were retrieved
	
	fclose(fpp);
	printf("%f %lf %lf %lf\n", sigmaN, A, sigmaG, R2);
}


