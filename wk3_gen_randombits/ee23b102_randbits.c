/***************************************************************************
 * SANJEEV SUBRAHMANIYAN S B
 * EE1103 NUMERICAL METHODS ASSIGNMENT 3
 * GENERATING RANDOM BITS
 * Last edited on 25 August 2023
 * PURPOSE: To accept user values from the command line using flags and generate random bit streams of arbitrary lengths using the rand and srand functions. Also to find the Hamming distance between two user given bit stream files.
 * ASSUMPTIONS: The user is assumed to know to use the command line without errors such as combining a seed flag and time flag, passing both arguments to create a file and check etc. i.e, it is assumed that either creation or comparison is done at a time.
 * INPUT: Takes arguments on the command line with flags as follows: 
 * -n number of random bits in the stream
 * -s to set the seed value
 * -t to use the inbuilt time function as a seed
 * -h to take two files and compute the Hamming distance between the streams in them
 * OUTPUT: Creates a text file with a random stream of numbers.
 ****************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h> // importing of the unistd library to use the getopt function
#include<time.h>

void writetofile(int N){ //function definition to write a stream of N bits to file
		FILE* fileptr = fopen("randbit.txt", "w"); // initialising and opening the file to write the bitstream in
		int i;
		int num;
		for(i = 0; i < N; i++){ // bits are written one by one on the file
			num = rand() % 2;
			fprintf(fileptr, "%d", num);
		}
}

int main(int argc, char **argv){
	int c, seed = 0, N, hc=0; // definition of variables for use in the function
	char str1, str2;
	time_t t; //initializing the time variable for seeding
	while((c = getopt(argc, argv, "n:h:s:t")) != -1) // using the getopt function to take arguments from the command line
	{
		switch(c) // switch function to check for the respective flags
		{
			case 'n':
				N = atoi(optarg);
				break;
			case 's':
				seed = atoi(optarg);
				srand(seed);
				break;
			case 't':
				srand(time(&t));
				break;
			case 'h':
				FILE * file1 = fopen(optarg, "r");
				FILE * file2 = fopen(argv[optind], "r");
				str1 = fgetc(file1);
				str2 = fgetc(file2);
				while ( str1 != EOF && str2 != EOF){ // checking if the characters are not EOF
					if(str1 != str2) // checking if the characters are equal or otherwise incrementing the Hamming distance variable
						hc += 1;
					str1 = fgetc(file1);
					str2 = fgetc(file2);
				}
				printf("The Hamming distance is %d\n", hc);
		}
	}
	writetofile(N); // writing to file function is called for the creation of the random bit stream
	return 0;
}
