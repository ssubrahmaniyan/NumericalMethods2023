/***************************************************************************
 * SANJEEV SUBRAHMANIYAN S B
 * EE1103 NUMERICAL METHODS QUIZ 1: PROBLEM 2: QUESTION 4
 * BUILDING ROADS
 * Last edited on 22 September 2023
 * PURPOSE: To read a text file containing the a number of planets and requests for road building, and the details of the planets to be connected. The program checks if the planets are already connected and 'builds' a road if not so. This implementation uses the common weight array and root array method and is less efficient as opposed to the weighted union with path compression.
 * INPUT: Takes as command line input a text file that contains the number of planets, number of requests and the details of the requests(i.e. the planets to be connected).
 * OUTPUT: Writes to a file a stream of numbers with 1 indicating a road having been built and 0 showing no road was built. It also writes to the file the runtime of the program rounded to the nearest millisecond.
 MISCELLANEOUS COMMENT:In this problem also I have called the planets as cities and do not indicate any different instance.
 ****************************************************************************/
#include<stdio.h>
#include<stdlib.h>
#include<time.h>

//as this implementation uses arrays of roots and weights, they are also passed in all the defined funtions.'

int root(int* Arr, int* rArr, int city1){ //root function is defined to find the root of the specified city.
    if(rArr[Arr[city1]] == city1)
        return rArr[Arr[city1]];
    
    int ret = root(Arr, rArr, rArr[Arr[city1]]);
    rArr[Arr[city1]] = ret; //path compression is applied to dynamically flatten the height of the tree
    return ret;
}

void connect(int *Arr, int *rArr, int *wArr, int city1, int city2){//the connect function connects the two cities by using the weighted union method.
    int root1 = root(Arr, rArr, city1);
    int root2 = root(Arr, rArr, city2);
    if(wArr[root1] >= wArr[root2]){
        rArr[root2] = root1;
        wArr[root1] += wArr[root2];
    }
    
    else{
        rArr[root1] = root2;
        wArr[root2] += wArr[root1];
    }
}

int isconnected(int* Arr, int* rArr, int city1, int city2){ //isconnected function returns 1 or 0 depending on if the two cities are connected which it checks by checking for the equality of the root elements.
    int root1 = root(Arr, rArr, city1);
    int root2 = root(Arr, rArr, city2);
    
    if (root1 == root2) {
        return 0; // The cities are connected.
    }
    
    else{
        return 1; // The cities are not connected.
    }
}

int main(int argc, char** argv){
    clock_t start = clock(), end; //initializing time variable to compute program run time
    long long runtime;
    
    FILE* freader = fopen(argv[1], "r");
    
    int ncities, nrequests;
    fscanf(freader, "%d %d", &ncities, &nrequests);
    
    int *Arr = (int*)malloc(ncities*sizeof(int));
    int *rArr = (int*)malloc(ncities*sizeof(int));
    int *wArr = (int*)malloc(ncities*sizeof(int));
    int *resArr = (int*)malloc(nrequests*sizeof(int));
    
    int i;
    for(i = 0; i < ncities; i++){
        rArr[i] = i; // Initialize each city to be its own root.
        Arr[i] = i;
        wArr[i] = 1;
    }
    
    for(i = 0; i < nrequests; i++){ //iterating through the requests to connect the cities that are not already connected and write the operation output into the result array.
        int city1, city2;
        fscanf(freader, "%d %d", &city1, &city2);
        
        int r = isconnected(Arr, rArr, city1, city2);
        if(r)
            connect(Arr, rArr, wArr, city1, city2);
        resArr[i] = r;
    }
    fclose(freader);
    
    FILE* fwriter = fopen("ee23b102_quiz2_q4_output.txt","w");
    
    for(i = 0; i < nrequests; i++)
        fprintf(fwriter, "%d", resArr[i]); //writing the objects of the result array into the output file.

    free(Arr);
    free(resArr);
    free(wArr);
    free(rArr);
    
    end = clock(); //stops the clock and gives us the timestamps to find the runtime
    runtime = (int)(((double)(end - start)/(double)CLOCKS_PER_SEC)*1000);
    fprintf(fwriter, "\n%lld ms", runtime); //writes the runtime onto the output file 
    fclose(fwriter);
    return 0;
}


