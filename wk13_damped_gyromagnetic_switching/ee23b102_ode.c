/***************************************************************************
 * SANJEEV SUBRAHMANIYAN S B
 * EE1103 NUMERICAL METHODS ASSIGNMENT 
 * DAMPED GYROMAGNETIC SWITCHING
 * Last edited on 3 NOVEMBER 2023
 * PURPOSE: To simulate a damped gyromagnetic switching by solving first order ordinary differential equations and comparing the different methods used: Euler, Heun and Runge-Kutta
 * INPUT: Takes as argument theta start, theta stop, alpha, delta_t
 * OUTPUT: Prints parameters of the different solutions in the order : alpha, delta_t, r2euler, r2heun
 * Comments : The code can be appropriately modified by choosing the commented out lines in the end to obtain the switching time plots and so on.
 ****************************************************************************/
#include<stdio.h>
#include<math.h>
#include<stdlib.h>

//definition of the constants that are used in the implementation and may be changed for testing if needed.
#define GAMMA 17.6e10
#define H 1e-10
#define Hk 0
#define RKINTERVAL 0.001

//function initializers for the functions used in the implementation
double Euler(double theta_start, double theta_stop, double alpha, double delta_t);
double Heun(double theta_start, double theta_stop, double alpha, double delta_t);
double RK(double theta_start, double theta_stop, double alpha, double delta_t);
double dTheta(double t, double a);
double dPhi(double theta, double a);
double thetaEuler(double theta, double t, double a, double delta_t);
double phiEuler(double phi, double theta, double a, double delta_t, double t);
double thetaHeun(double theta, double t, double a, double delta_t);
double phiHeun(double phi, double theta, double a, double delta_t, double t);
double thetaRK(double theta, double t, double a, double delta_t);
double phiRK(double phi, double theta, double a, double delta_t, double t);

int main(int argc, char** argv){
    double theta_start = atof(argv[1]), theta_stop = atof(argv[2]), alpha = atof(argv[3]), delta_t = atof(argv[4]);//reading of command line arguments
    int parameter = (int)(delta_t/0.001);
	//calling the functions that create the data files for analysis for the respective methods of solving the equations and which can also be used to plot the data on polar coordinates.    
    double tE = Euler(theta_start, theta_stop, alpha, delta_t);
    double tH = Heun(theta_start, theta_stop, alpha, delta_t);
    double tRK = RK(theta_start, theta_stop, alpha, RKINTERVAL);
    
    //R2 analysis for the euler method is performed
    FILE* f = fopen("RKdata.txt", "r");
    FILE* f2 = fopen("Eulerdata.txt", "r");
    long double sumx = 0, sumy = 0, sumx2 = 0, sumy2 = 0, sumxy = 0;
    long long n = 0;
    char c;
    
    while ((c = getc(f2)) != EOF) {//counting the number of lines in the file to find the number of data points on which the analysis is done
        if (c == '\n') {
            n++;
        }
    }
    fclose(f2);
    
    //calculation of r2 using the standard definition by computing
    FILE* f1 = fopen("Eulerdata.txt", "r");
    double temp1, temp2;
    double dumm;
    for(long long i = 0; i < n; i++){
        fscanf(f1,"%lf %*f %*f %*d", &temp1);
        sumx+=temp1;
        sumx2+=temp1*temp1;
        fscanf(f, "%lf %*f %*f %*d", &temp2);
        for(int l = 1; l < parameter; l++){
        fscanf(f, "%lf %*f %*f %*d", &dumm);
        }
        sumy+=temp2;
        sumy2+=temp2*temp2;
        sumxy+=temp1*temp2;
    }
    double r2euler = pow(((n * sumxy) - (sumx * sumy))/(sqrt((n*sumx2-(sumx * sumx)))*sqrt((n*sumy2-(sumy * sumy)))),2);
    fclose(f1);
    fclose(f);
    
    //R2 analysis for Heun follows
    FILE* f0 = fopen("RKdata.txt", "r");
    FILE* f4 = fopen("Heundata.txt", "r");
    sumx = 0, sumy = 0, sumx2 = 0, sumy2 = 0; sumxy = 0;
    n = 0;
    while ((c = getc(f4)) != EOF) {//counting the number of data lines to find the number of data points
        if (c == '\n') {
            n++;
        }
    }
    fclose(f4);
    FILE* f3 = fopen("Heundata.txt", "r");
    
    for(long long i = 0; i < n; i++){
        fscanf(f3,"%lf %*f %*f %*d", &temp1);
        sumx+=temp1;
        sumx2+=temp1*temp1;
        fscanf(f0, "%lf %*f %*f %*d", &temp2);
        for(int l = 1; l < parameter; l++){
        fscanf(f0, "%lf %*f %*f %*d", &dumm);
        }
        sumy+=temp2;
        sumy2+=temp2*temp2;
        sumxy+=temp1*temp2;
    }
    double r2heun = pow(((n * sumxy) - (sumx * sumy))/(sqrt((n*sumx2-(sumx * sumx)))*sqrt((n*sumy2-(sumy * sumy)))),2);
    fclose(f3);
    fclose(f0);
    double theot = ((alpha * alpha + 1)/(alpha*GAMMA))*(2/H)*log(1/tan(theta_start/2));
    
    printf("%lf %lf %lf %lf\n", alpha, delta_t, r2euler, r2heun);
    
    //the following commands can be used to obtain the switching times and the true switching time and so on.
    
    //printf("%lf %lf %lf %lf\n", tE, tH, tRK, theot);
    //printf("%lf %lf\n", delta_t, r2euler);
}

//the following functions are the standard definitions for the solving of the equations
double dTheta(double theta, double a){
	return ((GAMMA * a)/(a * a + 1))*(H * sin(theta) - Hk * sin(theta) * cos(theta));
}

double dPhi(double theta, double a){
	return (-1/(a*sin(theta)));
}

double thetaEuler(double theta, double t, double a, double delta_t){
	return theta + (dTheta(theta, a) * delta_t);
}

double phiEuler(double phi, double theta, double a, double delta_t, double t){
	double h = thetaEuler(theta, t, a, delta_t) - theta;
	return phi + (dPhi(theta, a)*h);
}

double thetaHeun(double theta, double t, double a, double delta_t){
    double k1 = dTheta(theta, a);
    double k2 = dTheta(theta + delta_t, a);
    return theta + ((k1 + k2) * delta_t) / 2;
}

double phiHeun(double phi, double theta, double a, double delta_t, double t){
    double h = thetaHeun(theta, t, a, delta_t) - theta;
    double k1 = dPhi(theta, a);
    double k2 = dPhi(theta + h, a);
    return phi + ((k1 + k2) * h) / 2;
}

double thetaRK(double theta, double t, double a, double delta_t){
	double k1 = dTheta(theta, a);
	double k2 = dTheta(theta + k1 * (delta_t/2), a);
	double k3 = dTheta(theta + k2 * (delta_t/2), a);
	double k4 = dTheta(theta + k3 * delta_t, a);
	return theta + ((k1 + 2*k2 + 2*k3 + k4) * (delta_t/6));
}

double phiRK(double phi, double theta, double a, double delta_t, double t){
	double h = thetaRK(theta, t, a, delta_t) - theta;
	double k1 = dPhi(theta, a);
	double k2 = dPhi(theta + k1 * (h/2), a);
	double k3 = dPhi(theta + k2 * (h/2), a);
	double k4 = dPhi(theta + k3 * h, a);
	return phi + ((k1 + 2*k2 + 2*k3 + k4) * (h/6));
}

//the following are the individual implementations for the different methods that are used.
double Euler(double theta_start, double theta_stop, double alpha, double delta_t)	{
	double t = 0;
	double phi = 0;
	FILE* fp = fopen("Eulerdata.txt", "w");
	fprintf(fp, "%lf %lf %lf %d\n", theta_start, phi, t, 1);
	double theta = thetaEuler(theta_start, t, alpha, delta_t);
	phi = phiEuler(phi, theta, alpha, delta_t, t);
	fprintf(fp, "%lf %lf %lf %d\n", theta, phi, t, 1);
	while(theta < theta_stop){
		t += delta_t;
		theta = thetaEuler(theta, t, alpha, delta_t);
		phi = phiEuler(phi, theta, alpha, delta_t, t);
		fprintf(fp, "%lf %lf %lf %d\n", theta, phi, t, 1);
	}
	fclose(fp);
	return t;

}

double Heun(double theta_start, double theta_stop, double alpha, double delta_t)	{
	double t = 0;
	double phi = 0;
	FILE* fp = fopen("Heundata.txt", "w");
	fprintf(fp, "%lf %lf %lf %d\n", theta_start, phi, t, 1);
	double theta = thetaHeun(theta_start, t, alpha, delta_t);
	phi = phiHeun(phi, theta, alpha, delta_t, t);
	fprintf(fp, "%lf %lf %lf %d\n", theta, phi, t, 1);
	while(theta < theta_stop){
		t += delta_t;
		theta = thetaHeun(theta, t, alpha, delta_t);
		phi = phiHeun(phi, theta, alpha, delta_t, t);
		fprintf(fp, "%lf %lf %lf %d\n", theta, phi, t, 1);
	}
	fclose(fp);
	return t;

}

double RK(double theta_start, double theta_stop, double alpha, double delta_t)	{
	double t = 0;
	double phi = 0;
	FILE* fp = fopen("RKdata.txt", "w");
	fprintf(fp, "%lf %lf %lf %d\n", theta_start, phi, t, 1);
	double theta = thetaRK(theta_start, t, alpha, delta_t);
	phi = phiRK(phi, theta, alpha, delta_t, t);
	fprintf(fp, "%lf %lf %lf %d\n", theta, phi, t, 1);
	while(theta < theta_stop){
		t += delta_t;
		theta = thetaRK(theta, t, alpha, delta_t);
		phi = phiRK(phi, theta, alpha, delta_t, t);
		fprintf(fp, "%lf %lf %lf %d\n", theta, phi, t, 1);
	}
	fclose(fp);
	return t;

}
