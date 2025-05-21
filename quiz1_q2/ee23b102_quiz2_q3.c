/***************************************************************************
 * SANJEEV SUBRAHMANIYAN S B
 * EE1103 NUMERICAL METHODS QUIZ 1: PROBLEM 2: QUESTION 3
 * BUILDING ROADS
 * Last edited on 21 September 2023
 * PURPOSE: To read a text file containing the a number of planets and requests for road building, and the details of the planets to be connected. The program checks if the planets are already connected and 'build
 s' a road if not so. This implementation uses structs and pointers with the concept of path compression.
 * INPUT: Takes as command line input a text file that contains the number of planets, number of requests and the details of the requests(i.e. the planets to be connected).
 * OUTPUT: Writes to a file a stream of numbers with 1 indicating a road having been built and 0 showing no road was built. It also writes to the file the runtime of the program rounded to the nearest millisecond.
 MISCELLANEOUS COMMENT:In this problem I have called the planets as cities and do not indicate any different instance.
 ****************************************************************************/
#include<stdio.h>
#include<stdlib.h>
#include<time.h>

struct element { //a struct corresponding to a city is created which contains the index of the city, the weight of the tree that is linked to it and a pointer to its parent element.
    int citynumber;
    int weight;
    struct element* upptr;
};

struct element* root(struct element* instance) { //the function root takes in a pointer to an instance of the city as argument and returns its root by traversing the tree it is part of. A root is ad hoc defined as one whose upptr value is null, that is it does not have a parent element.
    if (instance->upptr == NULL)
        return instance;

    struct element* ret = root(instance->upptr);
    
    instance->upptr = ret;// Apply path compression by updating the upptr to the root directly

    return ret;
}

int isconnected(struct element* instance1, struct element* instance2) { //isconnected function returns 1 or 0 depending on if the two cities are connected which it checks by checking for the equality of the root elements.
    if (root(instance1) == root(instance2)) {
        return 0;
    }
    
    else {
        return 1;
    }
}

void connect(struct element* instance1, struct element* instance2) { //the connect function connects the two cities by using the weighted union method.
    struct element* root1 = root(instance1);
    struct element* root2 = root(instance2);

    if (root1->weight >= root2->weight) { //checking for the element with the lesser weight to connect to the larger.
        root2->upptr = root1;
        root1->weight += root2->weight;
    }
    
    else {
        root1->upptr = root2;
        root2->weight += root1->weight;
    }
}

int main(int argc, char** argv) {

    clock_t start = clock(), end; //initializing time variable to compute program run time
    long long runtime;
    
    int ncities, nrequests, i;
    
    FILE* freader = fopen(argv[1], "r"); //a file read instance is created to read the number of cities and the number of requests from the file given
    fscanf(freader, "%d %d", &ncities, &nrequests);
    
    struct element* instance = (struct element*)malloc(ncities * sizeof(struct element)); //memory is allocated for the required number of structs

    int *outArr = (int*)malloc(nrequests*sizeof(int));

    for (i = 0; i < ncities; i++) { // a number of instances of the structs are created and the default values set for them
        instance[i].citynumber = i;
        instance[i].weight = 1;
        instance[i].upptr = NULL;
    }

    for (i = 0; i < nrequests; i++) {
        
        int p, q;
        fscanf(freader, "%d %d", &p, &q);
        
        int w = isconnected(&instance[p], &instance[q]); //the two cities are checked if they are connected and subsequently operated upon.
        if (w) {
            connect(&instance[p], &instance[q]);
            outArr[i] = w;
        }
        
        else {
            outArr[i] = w;
        }
    }
    
    fclose(freader);

    FILE* fwriter = fopen("ee23b102_quiz2_q3_output.txt", "w"); //a file writer instance is also created

    for (i = 0; i < nrequests; i++) {
        fprintf(fwriter, "%d", outArr[i]);
    }

    free(instance);
    
    end = clock(); //stops the clock and gives us the timestamps to find the runtime
    runtime = (int)(((double)(end - start)/(double)CLOCKS_PER_SEC)*1000);
    fprintf(fwriter, "\n%lld ms", runtime); //writes the runtime onto the output file 
    fclose(fwriter);
    
    return 0;
}

