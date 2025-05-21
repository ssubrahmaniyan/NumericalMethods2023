/***************************************************************************
 * SANJEEV SUBRAHMANIYAN S B
 * EE1103 NUMERICAL METHODS ASSIGNMENT 
 * GAUSSIAN ELIMINATION FOR MATRIX SOLVING	
 * Last edited on 13 OCTOBER 2023
 * PURPOSE: To read coefficients of a set of equations and solve them using gaussian elimination method
 * INPUT: Takes arguments from the command line, the datafile in which the data is stored.
 * OUTPUT: Prints the solution for the set of equations, in the same order of the variables given in the question.
 * CONSTRAINTS : The code is optimized for most sets, throws an error if the set has more than one solution or is consistent dependent. The file is also assumed to be appropriately typed and saved.
 ****************************************************************************/
#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#define TOL 1e-4

void normalize(float **datarr, int n){//function that normalizes the coefficients in the file to prevent errors
	float max = 0;
	for(int i = 0; i < n; i++){
		for(int j = 0; j < n + 1; j++){
			if(fabs(datarr[i][j]) >= max){//finding the maximum value of the coefficients and divides the set by the max value
				max = datarr[i][j];
				}
		}
		for(int j = 0; j < n + 1; j++){
			datarr[i][j] /= max;
		}
	}
}
	
int findpivot(int start, int n, int ind, float **datarr){//function used to find the pivot for one operation and returns the location where the pivot was swapped
	float abs = 0, dummy = 0;

	for(int i = start; i < n; i++){
		if(fabs(datarr[i][ind]) >= abs){
			abs = fabs(datarr[i][ind]);
			dummy = i;
		}
	}

	return dummy;
}

void swaprows(int start, int dum, int n, float **datarr){ //start is the row index with which the pivot row(dum) is swapped with
	float temp;
	for(int i = 0; i < n + 1; i++){
		temp = datarr[dum][i];
		datarr[dum][i] = datarr[start][i];
		datarr[start][i] = temp;
	}
}

void eliminate(int index, int rowno, int start, int n, float **datarr){ //index is the position of the element in the array, that is index = 2 is the coefficient of the third variable and rowno is the number of the row (0 to  n-1)
	if(fabs(datarr[start][index]) > TOL){
		float factor = datarr[rowno][index] / datarr[start][index];
		for(int i = 0; i < n + 1; i++){
			datarr[rowno][i] -= datarr[start][i] * factor;
		}
	}
}

void final(int n, float **datarr){//the function that solves the set of equations and updates the final matrix
	int p, q, dum;
	for(int p = 0; p < n - 1; p++){
		dum = findpivot(p, n, p, datarr);
		swaprows(p, dum, n, datarr);
		for(q = p + 1; q < n; q++){
			eliminate(p, q, p, n, datarr);
		}
	}
}

void solve(float *datarr, float *solution, int solind, int n){//solves the set and updates the solution array
	float sum = 0;
	for(int i = n; i > solind; i--){
		sum += datarr[i] * solution[i];
	}
	if(fabs(datarr[solind]) < TOL){
		printf("Consistent Dependent\n");
		exit(2);
	}
	solution[solind] = (datarr[n] - sum) / datarr[solind];
}

void reversesub(int n, float** datarr, float *solution){
	for(int j = 0; j < n; j++){
		solution[j] = 0;
	}
	for(int i = n - 1; i >= 0; i--){
		solve(datarr[i], solution, i, n);
	}
}

int main(int argc, char** argv){//data is taken in the form of number of rows(n), the name of the datafile(name)
	int n = 0;
	char* file = argv[1];
	FILE *f = fopen(file, "r");
    	char c;

    	while ((c = fgetc(f)) != EOF) {// Count the number of lines
        	if (c == '\n') {
            		n++;
        	}
    	}
    	fclose(f);

	FILE* fread = fopen(file, "r");
	int i, j;
	float solution[n];
	float **datarr = (float** )malloc(n * sizeof(float *)); //memory is allocated to create an array of pointers to array which signify the rows of the matrix that we are handling. For example, dataarr[i] represents the the pointer to the array that contains the values of the ith row(note that here i runs from 0 to  n-1)
	for(i = 0; i < n; i++){//allocation of memory to contain the n+1 elements that correspond to the coefficients of the row
		datarr[i] = (float*)malloc((n+1) * sizeof(float)); 
	}

	for(i = 0; i < n; i++){
		for(j = 0; j < n + 1; j++){
			fscanf(fread, "%f", &datarr[i][j]);
		}
	}
	normalize(datarr, n);
	final(n, datarr);
	reversesub(n, datarr, solution);

	for(int p = 0; p < n; p++){
		printf("%f ", solution[p]);
	}
	printf("\n");
	
	free(datarr);
	fclose(fread);

}






