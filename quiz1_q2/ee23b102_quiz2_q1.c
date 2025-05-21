/***************************************************************************
 * SANJEEV SUBRAHMANIYAN S B
 * EE1103 NUMERICAL METHODS QUIZ 1: PROBLEM 2: QUESTION 1
 * BUILDING ROADS
 * Last edited on 20 September 2023
 * PURPOSE: To read a text file containing the a number of planets and requests for road building, and the details of the planets to be connected. The program checks if the planets are already connected and 'builds' a road if not so.
 * INPUT: Takes as command line input a text file that contains the number of planets, number of requests and the details of the requests(i.e. the planets to be connected).
 * OUTPUT: Writes to a file a stream of numbers with 1 indicating a road having been built and 0 showing no road was built. It also writes to the file the runtime of the program rounded to the nearest millisecond.
 ****************************************************************************/
#include<stdio.h>
#include<stdlib.h>
#include<time.h>

int main(int argc, char** argv){
    clock_t start = clock(), end; //initializing time variable and setting the clock to start running to compute program run time
    
    long long runtime;
    FILE* fptr = fopen(argv[1], "r"); //creates a file pointer for reading data from the input file
    
    int nplanets, nrequests, i, k;
    fscanf(fptr, "%d %d", &nplanets, &nrequests);
    
    int *arrp = (int*)malloc(nplanets*sizeof(int)); //creation of an array and allocation of memory to it which is used to check if given planets are already connected
    
    for(i = 0; i < nplanets; i++){
        arrp[i] = i; //intializing each element of the array to hold the index of the planet itself as its root as it initially is not connected to any other planet
    }
    
    int *outArr = (int*)malloc(nrequests*sizeof(int)); //an array is created to store the output from each iteration(request for building a road and if the road has been built or not is tracked by this array) before writing it into the output file

    int planet1, planet2, p; //the variable p is used to check if the other planets that were already linked also get linked to the new planet

    for(i = 0; i < nrequests; i++){
    
        fscanf(fptr, "%d %d", &planet1, &planet2);
        
        if(arrp[planet1] != arrp[planet2]){ //checks if the two planets are already connected and then connects them if not so
            
            outArr[i] = 1; //keying that the road has been built
            p = arrp[planet2];
            arrp[planet2] = arrp[planet1]; //redefining the root of planet 2
            
            for(k = 0; k < nplanets; k++){ //checking if other planets are linked to this planet to which the road was built and updating them correspondingly
            
            	if(arrp[k] == p)
            		arrp[k] = arrp[planet1];
            }
        }
        
        else
            outArr[i] = 0;
            
    }
    
    fclose(fptr);
    
    FILE* fw = fopen("ee23b102_quiz2_q1_output.txt", "w");
    
    for(i = 0; i < nrequests; i++){
        fprintf(fw, "%d", outArr[i]); //writes the output stream of 1s and 0s into the file as mentioned
    }
    
    free(outArr);
    free(arrp);
    
    end = clock(); //stops the clock and gives us the timestamps to find the runtime
    runtime = (long long)(((double)(end - start)/(double)CLOCKS_PER_SEC)*1000); //rounding the read runtime to the nearest integer millisecond
    fprintf(fw, "\n%lld ms", runtime); //writes the runtime onto the output file 
    
    fclose(fw);
}
