//ee23b102_series.c		SANJEEV SUBRAHMANIYAN S B - EE23B102		August 18 2023
//Program to compute the value of sin(x) upto a specific number of terms using the Taylor expansion and determine the error with respect to the inbuilt sin function.
//The input takes an integer and a float from the command line for the number of terms in the Taylor expression and argument of the sine function and prints the computed value and the error against the true value of the sine.
//**********************************************************************************************************************************************
#include<stdio.h>
#include<math.h>
#include<stdlib.h>

float mysin(int x, float y); // declaration of the mysin function
long long factorial(int x);  // declaration of the factorial function

int main(int argc, char *argv[]){ 

	int x;
	float y;
	
	x = atoi(argv[1]);
	y = atof(argv[2]);

	
	float mysin(int x, float y) { // definition of the mysin function
		int z = 1, i, w = 0;
		float si = 0;
		float *ret[2];

		for (i = 0; i < x; i++) {
			si += pow(-1, w) * pow(y, z) / factorial(z); // expression to compute each term in the Taylor expression for the sin(x)
			z += 2;
			w += 1;
		}
		
		return si;
	}
	
	float s = sin(y);
	float sineval = mysin(x, y);
	float err = s - sineval; // computing the error between the true and truncated values of sin
	
	printf("%.3f,%.3f\n", sineval, err);
}
	
long long factorial(int p){  //definition of the factorial function
	int m = 1;
	long long val = 1;

	for (m = 1; m <= p; m++)
		val = val * m;
	return val; 
}
		
