/***************************************************************************
 * SANJEEV SUBRAHMANIYAN S B
 * EE1103 NUMERICAL METHODS ASSIGNMENT 
 * INTERPOLATION 
 * Last edited on 20 October 2023
 * PURPOSE: To create a lagrange interpolation for a given set of inputs for the function 1 / (1 + 25*x^2). A newton interpolation is made with chosen samples from the lagrange interpole, which is then used to estimate the value of the the interpole at the given input.
 * INPUT: Takes arguments from the command line, the values for the order of the polynomial, the start point of the sampling ranges, the end point of the sampling range and the point at which the interpole is to be estimated.
 * OUTPUT: Prints the interpolated value for the given input, done in the way described above.
 * CONSTRAINTS : The code is optimized for polynomials of orders less than 10, and note that the value at which the interpole is to be estimated must lie between the start and end of the sampling points.
 ****************************************************************************/
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>

float rungePoly(float x){
	return (1/(float)(1 + 25 * pow(x, 2)));
}

float lagrange(float* x, float* y, int n, float x0);

float newton(float* samplesX, float* samplesY, int polyOrder, float x0);

int main(int argc, char** argv){
	int polyOrder = atoi(argv[1]);
	float xstart = atof(argv[2]), xend = atof(argv[3]), x0 = atof(argv[4]);
	
	float* xValues = (float*)malloc((polyOrder + 1) * sizeof(float));
	float* yValues = (float*)malloc((polyOrder + 1) * sizeof(float));

	for(int i = 0; i < polyOrder + 1; i++){ //creation of the array which hold the data points and their corresponding values
		xValues[i] = xstart + (((xend - xstart) / polyOrder) * i);
		yValues[i] = rungePoly(xValues[i]);
	}

	float* samplesX = (float*)malloc((polyOrder + 1) * sizeof(float));
	float* samplesY = (float*)malloc((polyOrder + 1) * sizeof(float));

	srand(time(NULL));

	for(int i = 0; i < polyOrder + 1; i++){//creating a new set of samples that are interpolated using the lagrange interpolation
		samplesX[i] = xstart + (xend - xstart) * (rand()/(float)RAND_MAX);
		samplesY[i] = lagrange(xValues, yValues, polyOrder + 1, samplesX[i]);
	}
	
	printf("%f\n", newton(samplesX, samplesY, polyOrder + 1, x0));

	//freeing the pointers allocated
	free(xValues);
	free(yValues);
	free(samplesX);
	free(samplesY);
}

float lagrange(float* x, float* y, int n, float x0){//evaluation of the point using the lagrange interpolation method
	float sum = 0;
	
	for(int i = 0; i < n; i++){
		float product = y[i];
		for(int j = 0; j < n; j++){
			if(i != j)
				product = product * (x0 - x[j]) / (x[i] - x[j]);
		}
		sum += product;
	}
	
	return sum;
}

float newton(float* samplesX, float* samplesY, int polyOrder, float x0){//evaluation of the value at the point using the newton interpolation method
	float fdd[polyOrder][polyOrder];
	
	for(int i = 0; i < polyOrder; i++){
		fdd[i][0] = samplesY[i];
	}
	
	for(int j = 1; j < polyOrder; j++){
		for(int i = 0; i < polyOrder - j; i++){
			fdd[i][j] = (fdd[i +  1][j - 1] - fdd[i][j - 1])/(samplesX[i + j] - samplesX[i]);
		}
	}
	
	float xterm = 1;
	float yint[polyOrder];
	yint[0] = fdd[0][0];
	float yint2;
	
	for(int ord = 1; ord < polyOrder; ord++){
		xterm = xterm * (x0 - samplesX[ord - 1]);
		yint2 = yint[ord - 1] + fdd[0][ord] * xterm;
		yint[ord] = yint2;
	}
	return yint[polyOrder - 1];
}



	
