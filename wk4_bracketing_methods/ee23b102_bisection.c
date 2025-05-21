/***************************************************************************
 * SANJEEV SUBRAHMANIYAN S B
 * EE1103 NUMERICAL METHODS ASSIGNMENT 4
 * BRACKETING METHODS
 * Last edited on 1 September 2023
 * PURPOSE: To use bracketing methods bisection and false position to find the root(s) of a defined polynomial
 * INPUT: Takes arguments from the command line (1) and (2) to perform the bisection and false position methods respectively
 * OUTPUT: Prints the root of the function as calculated by the function(method).
 ****************************************************************************/
#include<stdio.h>
#include<math.h>
#include<stdlib.h>

float bisection(float xl, float xu, float xr, float tol); // the bisection function is initialized
float false_position(float,float,float,float); // the false position function is initialized
float f(float x); // a function which returns the value of the polynomial given in the question is written

int main(int argc, char** argv){
	int y;
	y = atoi(argv[1]);
	float xl = 0.5, xu = 1.0, tol = 0.1, tolf = 0.002; //tol and tolf are the respective tolerance values for bisection and falseposition methods
	float xr = (xl + xu) / 2;
	float prev = xl;
	switch(y){ //checking the user input and calling the function for the specified method
		case 1:
			printf("%f", bisection(xl, xu, xr, tol));
			break;
		case 2:
			printf("%f", false_position(xl, xu, tolf, prev));
			break;
	}
}

float f(float x){ // definition of the f(x) as a function
	float z = (-25 + 82 * x - 90 * powf(x, 2) + 44 * powf(x, 3) - 8 * powf(x, 4) + 0.7 * powf(x, 5));
	return z;
}

float bisection(float xl, float xu, float xr,float tol){ //defintion of the bisection function
	float xrn = (xl + xu) / 2, xln, xun; //standard definitions in the bisection method
	float fl = f(xl), fu = f(xu), fr = f(xr);
	float ea = fabsf(((xl - xu) / (xl + xu)));
	if(ea <= tol){
		return xrn;}
       	if ((fl * fr) > 0){
		xln = xr;
		xun = xu;
	}
	else if ((fl * fr) < 0){
		xun = xr;
		xln = xl;
	}
	else if ((fl * fr) == 0){
		return xrn;
	}
	return bisection(xun, xln, xrn, tol); // implementation of recursion until the required accuracy is achieved
			
}

float false_position(float xl, float xu, float tolf,float prev){ // definition of the method for the false position operations
	float xrn = xu -(((f(xu)) * (xl - xu)) / (f(xl) - f(xu))), xln, xun;
	float fl = f(xl), fu = f(xu), fr = f(xrn);
	float ea = fabsf((xrn - prev) / xrn);
	if(ea <= tolf){
		return xrn;}
       	if ((fl * fr) > 0){
		xln = xrn;
		xun = xu;
	}
	else if ((fl * fr) < 0){
		xun = xrn;
		xln = xl;
	}
	else if ((fl * fr) == 0){
		return xrn;
	}
	return false_position(xun, xln, tolf, xrn); // recursion implementation
			
}
