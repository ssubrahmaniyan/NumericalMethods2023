/***************************************************************************
 * SANJEEV SUBRAHMANIYAN S B
 * EE1103 NUMERICAL METHODS ASSIGNMENT 
 * REAL TIME SERIES 
 * Last edited on 14 October 2023
 * PURPOSE: To create lorentzian and gaussian time series, apply the exponential filtering on them and to find the average and standard deviation vales of a and T.
 * INPUT: Takes arguments from the command line, the value s of M = number of peaks, T = time between two peaks, a = width and "Gaussian" or "Lorentzian" depending on which is used for analysis(note that Lorenzian is assumed to be default when no corresponding input in provided)
 * OUTPUT: Prints the average of T, average of a, standard deviation of T and standard deviation of a.
 * CONSTRAINTS : The code is optimized for M<=1000, T<=100, a <=20. Please avoid testing it at further values.
 ****************************************************************************/
 
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include<string.h>

#define scale_n 0.0333
#define scale_t 0.0166
#define scale_a 0.0166
#define PPL 100
#define SMOOTHCONST 0.9

//a struct to hold the parameters for every peak is defined intially
struct peak{
	double loc;
	double amp;
	double a;
};

//this function creates the arrays that hold the random variables x, y for the box muller transform. this is seeded before execution. also global variables arr1, arr2, arrN1, arrN2 need to be created to store the values and initialzied as double* arr1 =NULL;

void randarr(long long M, double* arr){ //here M is passed as such to create the random variable arrays for noise in T and a. for the noise in amplitude the passed value of M = no of peaks * ppl
	long long i; 
	
	for(i = 0; i < M; i++){
		*arr = rand() / (double)RAND_MAX;
		arr++;
	}
}

//the noise function which takes arguments a, scaling factor and returns a box muller transformed normally distributed random number that has been scaled to the size of a. this is used to add noise to width and time

double noise(double scale, double a, double* arr1, double* arr2){
	double x = *arr1;
	double y = *arr2;
	double z0 = sqrt(-2 * log(x)) * cos(2 * M_PI * y);
	
	return z0*scale*a;
}

double noiseN(double scale, double a, double* arrN1, double* arrN2){//this is used to create the amplitude noise for N
	double x = *arrN1;
	double y = *arrN2;
	double z0 = sqrt(-2 * log(x)) * cos(2 * M_PI * y);
	
	return z0*scale*a;
}

double timeseries(double a, double mT, double t, int i,double T){
	
	if(i == 0){ //a value of i is given to choose between lorenzian and gaussian series
		double p = pow(a, 2) / (pow(a, 2) + pow(t-mT+(T/2.0), 2));
		
		return p;
	}
	
	else{
		double p = exp(-0.5 * pow(t-mT+(T/2.0), 2) / pow(a, 2));
		
		return p;
	}
}

//this writer function follows a seeding statement that uses time as its random seed

void writer(int M, double a, double T, double ind, double* arr1, double* arr2, double* arrN1, double* arrN2){//ind holds the value of if it is lorenzian or gaussian
	
	FILE* fwriter = fopen("data.txt", "w");
	
	double i = 0; //here the i variable operates as t for the generator functions
	double v = 0;
	int m = 1;
	int n = 1;
	long long p = 1;
	double k = 0;
	double a1 = a, T1 = T;
	
	for(i = 0; i < (double)M*T; i = i + (double)T/PPL){
		v = 0;
		n = 0;
		a1 = a;
		T1 = T;
		
		for(m = 1; m <= M; m++){
			a1 = a + noise(scale_a, a, (arr1+n-1), (arr2+n-1));//addition of corresponding noise to the datapoints
			T1 = m*T + noise(scale_t, T, (arr1+n-1), (arr2+n-1));
			n++;
			k = timeseries(a1, T1, i, ind, T);
			v += k;
		}
		
		fprintf(fwriter, "%lf %lf\n", i, v + noiseN(scale_n, v, (arrN1+p-1), (arrN2+p-1)));//writing the data into a file for reading later
		p++;
	}
	
	fclose(fwriter);
}

double smooth(double avg, double val, double sc){//sc is smoothening coefficient
	avg = (1 - sc) * val + sc * avg;
	
	return avg;
}

int main(int argc, char** argv){
	    // Command line arguments
    int M = atoi(argv[1]);      // Number of peaks
    double T = atof(argv[2]);    // T parameter
    double a = atof(argv[3]);    // a parameter
    int ind;
    time_t t1;
    time_t t2;
    time_t t3;
    time_t t4;
    
    if(argc == 4){
    	ind  = 0;
    }
    
    else{
    ind = strcmp("Lorenzian", argv[4]); // Peak type (Gaussian or Lorentzian)
    }
    
    struct peak *p = (struct peak*)malloc(M * sizeof(struct peak));
    
    double* arr1 = (double*)malloc(M * sizeof(double));
    double* arr2 = (double*)malloc(M * sizeof(double));
    double* arrN1 = (double*)malloc(M * PPL * sizeof(double));
    double* arrN2 = (double*)malloc(M * PPL * sizeof(double));
    
    srand((unsigned)time(&t1));
    randarr(M, arr1);
    srand((unsigned)time(&t2));
    randarr(M, arr2);
    srand((unsigned)time(&t3));
    randarr(M*PPL, arrN1);
    srand((unsigned)time(&t4));
    randarr(M*PPL, arrN2);
    writer(M, a, T, ind, arr1, arr2, arrN1, arrN2);
    
    free(arr1);
    free(arr2);
    free(arrN1);
    free(arrN2);
    //file reading and analysis and smoothening follows
    
    FILE* freader = fopen("data.txt", "r");
    //FILE* fw = fopen("smooth.txt", "w");
    
    double threshold = 10 * (0.1 / 3.0); //threshold is defined to be 5 times the standard deviation
    long long c;
    double r1;
    double r2;
    double avg = 0;
    double ti = 0, tf = 0;
    int val1 = 1, val2 = 0, valc1 = 1, valc2 = 0;//val1 is the condition ot find the upward crossing of the curve and val2 is the condition for the downward crossing of the curve
    int z = 0;//keep track of the structs 
    double check1 = 0, check2 = 0;
    
    double **datarr = (double**) malloc(PPL * sizeof(double*)); //a datarray to hold the values of time vs amplitude at each point dyanmically for every lorenian to find the maximum and fwhm.
    
    for(int g = 0; g < PPL; g++){
    	datarr[g] = (double*) malloc(2 * sizeof(double));
    }
    
    for(c = 0; c < M; c++){
    	
    	for(int u = 0; u < PPL; u++){
	    	fscanf(freader, "%lf %lf", &r1, &r2);
	    	avg = smooth(avg, r2, SMOOTHCONST);
	    	//fprintf(fw, "%lf %lf\n", r1, avg);
		datarr[u][0] = r1;
		datarr[u][1] = r2;
		
	    	//checking if it crosses the threshold
	    	if(r2 > threshold && val1){
	    		ti = (threshold - r2) * (r1 - check1) / (r2 - check2) + r1;//extrapolating the curve to optimally find the location of crossing
	    		val1 = 0;
	    		val2 = 1;
	    	}
	    	//checking for the downward crossing after the upward crossing
	    	if(r2 < threshold && val2){
	    		tf = (threshold - r2) * (r1 - check1) / (r2 - check2) + r1;
	    		val1 = 1;
	    		val2 = 0;
	    		p[z].loc = (tf + ti)/(double)2;
	    		z++;
	    	}
	    	   	
	    	check1 = r1;
	    	check2 = r2;
    	 }
    	 
    	 z--;
    	 //parsing the datarray to find the amplitude of the maximum
    	 
    	 for(int u = 1; u < PPL - 1; u++){
    	 	
    	 	if(datarr[u-1][0] <= p[z].loc && datarr[u+1][0] >= p[z].loc){
    	 		p[z].amp = datarr[u][1];
    	 		break;
    	 	}
    	 }
    	 //going left and right to find the locations at which half maximum occur
    	 valc1 = 1;
    	 valc2 = 0;
    	 for(int u = 1; u < PPL -1; u++){
    	 	if(datarr[u-1][1] <= p[z].amp/2.0 && datarr[u+1][1] >= p[z].amp/2.0 && valc1){
    	 		p[z].a = datarr[u][0];
    	 		valc1 = 0;
    	 		valc2 = 1;
    	 	}
    	 	
    	 	if(datarr[u-1][1] >= p[z].amp/2.0 && datarr[u+1][1] <= p[z].amp/2.0 && valc2){
    	 		
    	 		if(ind == 0){
    	 			p[z].a = (datarr[u][0] - p[z].a) / 2.0;
    	 		}
    	 		
    	 		else{
    	 			p[z].a = (datarr[u][0] - p[z].a) * sqrt(2 * log(2)) / 2.0;
    	 		}
    	 		valc2 = 0;
    	 	}
    	 }
    	 z++;
    }
    
    int q = 0;
    double sumT = 0, avgT, sumsqT, stdevT;
    
    //the following lines of code perform the common operations to calculate the average and standard deviations of T and a
    for(q = 0; q < z -1; q++){
    	sumT += p[q+1].loc - p[q].loc;
	}
	avgT = sumT / (double)(z-1);

	for(q = 0; q < z-1; q++){
		sumsqT += (p[q+1].loc - p[q].loc - avgT)*(p[q+1].loc - p[q].loc - avgT);
	}

	stdevT = sqrt(sumsqT/(double)(z-2));
		
    double suma = 0, avga = 0, sumsqa = 0, stdeva = 0;
    
    for(q = 0; q < z; q++){
    	suma += p[q].a;
    }
    
    avga = suma / (double)z;
    
    for(q = 0; q < z; q++){
    	sumsqa += (p[q].a - avga)*(p[q].a - avga);
    }
    
    stdeva = sqrt(sumsqa/(double)z);
    
    printf("%lf %lf %lf %lf\n", avgT, avga, stdevT, stdeva);
    
    
    for(int g = 0; g < PPL; g++){
    	free(datarr[g]);
    }

    free(datarr);
    fclose(freader);
    //fclose(fw);
    
}
