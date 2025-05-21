/***************************************************************************
 * SANJEEV SUBRAHMANIYAN S B
 * EE1103 QUIZ 1
 * <Enter title of assignment here>
 * Last edited on 15 September 2023
 * PURPOSE:To take values of N, Nseed, M, Mseed as command line arguments, generate random numbers, write them to a file and compute the hamming distance
 * OUTPUT:Index at which min hamming distance occurs in the string and the min hamming distance
 ****************************************************************************/

#include<stdio.h>
#include<stdlib.h>

int hammingdistance(int *mbits, int *nbits, int M, int N){ //note that M<N
	int *pt1 = mbits;
	int *pt2 = nbits;
	int hd, hdi, p, k, startind;
	hd=0;

	for(k = 0; k < M; k++){
		if(*pt1 != *pt2)
			hd++;
		pt1++;
		pt2++;
		startind = 0;
	}
	
	for(p = 1; p < N - M + 1; p++){
		int *pt1 = mbits;
		pt1 += p;
		int *pt2 = nbits;
		hdi = hd;
		hd = 0;
		int l;
		for(l = 0; l < M; l++){
			if(*pt1 != *pt2)
				hd++;
			pt1++;
			pt2++;
		}
		
		if(hdi < hd)
			hd = hdi;
		else
			startind = p;
		}
	
	printf("%d %d \n", startind+1, hd);

}

int main(int argc, char** argv){
	int N = atoi(argv[1]), Nseed = atoi(argv[2]), M = atoi(argv[3]), Mseed = atoi(argv[4]);
	
	if(M > N || argc != 5){
		printf("Invalid input!");
	}

	int *nbits = (int*)malloc((N)* sizeof(int));
	int *mbits = (int*)malloc((M)* sizeof(int));

	int i,j,n1,n2;
	
	FILE *f2 = fopen("Mfile.dat","w");

	srand(Mseed);

	for(i=0;i<M;i++){
		n2 = (rand() % 2);
		mbits[i] = n2;
		fprintf(f2, "%d ", n2);
	}
	fclose(f2);
	
	srand(Nseed);

	FILE *f1 = fopen("Nfile.dat","w");

	for(j=0;j<N;j++){
		n1 = (rand() % 2);
		nbits[j] = n1;
		fprintf(f1, "%d ", n1);
	}
	
	fclose(f1);

	hammingdistance(mbits, nbits, M, N);
	free(nbits);
	free(mbits);
	return 0;
}
	



