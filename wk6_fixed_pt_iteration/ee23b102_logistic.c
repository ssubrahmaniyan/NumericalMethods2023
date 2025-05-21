/***************************************************************************
 * SANJEEV SUBRAHMANIYAN S B
 * EE1103 NUMERICAL METHODS ASSIGNMENT 
 * FIXED POINT ITERATION
 * Last edited on 8 September 2023
 * PURPOSE: To accept user values from the command line for Niter(number of max iterations), rstep(step value for increment in r) and error tolerance(relative) and to plot the logistics map for the recurrence relation x(i+1)=r*x(i)*(1-x(i)) as a function of r.
 * INPUT: Takes arguments on the command line.
 * OUTPUT: Creates a text file "data.txt" which contains two columns of data (x,r) which is then plot on gnuplot.
 ****************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<math.h>

float iter(float x, float r){ //definition of the function to use the recurrence relation
	float z = r * x * (1 - x);
	return z;
}

int main(int argc, char** argv){//no of iter, step size in r, tolerance in float taken in as command line inputs
	FILE *fptr = fopen("data.txt", "w");//opens and creates a file pointer
	float r;
	float x0 = 0.5;
	float cc =atof(argv[3]); // max approximate error tolerance
	int itermax = atoi(argv[1]);//fixes the max number of iterations as taken from the user
	for(r = 0; r <= 4;r = r + atof(argv[2])){
		float x = x0;
		float ea = 1.0;
		int itercount = 0;
		while(itercount <= 20){//initial series of 20 iterations are skipped to make the plot uniform
			float xi = x;
			x = iter(x,r);
			itercount++;
		}
		while(ea >= cc && itercount <= itermax){//condition for further executions
			float xi = x;
			x = iter(x,r);
			if(x != 0)
				ea = fabs((x-xi)/x);
			fprintf(fptr,"%f %f \n", r, x);//writing the data to the file
			itercount++;
		}
	}

	fclose(fptr);

}
