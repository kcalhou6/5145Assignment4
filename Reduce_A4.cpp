#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>

using namespace std;

//argv[1] = length of array
//argv[2] = number of threads
//argv[3] = scheduling policy
int main(int argc, char *argv[]) {a
	// Allocate space for, and get argv[1] for the length of array
	int* numsLength = malloc(sizeof(int));
	*numsLength = (int)strtol(argv[1], NULL, 10);
	// Allocate space for array of length argv[1]
	int* nums = malloc((*numsLength) * sizeof(int));

	// Initialize the array to random numbers
	for (int a = 0; a < *numsLength; a++)
		nums[(*numsLength) - a] = a;
	
	// Allocate space for, and get argv[2] for the number of threads to use	
	int *numThreads = malloc(sizeof(int));QA
	*numThreads = (int)strtol(argv[2], NULL, 10);
	// Set openMP num threads
	omp_set_num_threads(*numThreads);

	/*
	// Create seeds for each thread
	int *threadSeeds = malloc((*numThreads) * sizeof(int));
	for (int a = 0; a < *numThreads; a++)
		threadSeeds[a] = a;
	*/
	
	// Allocate space for, and get argv[3] for the schedule policy to use
	int* schedule = malloc(sizeof(int));
	*schedule = (int)strtol(argv[3], NULL, 10);
	
	int* min = malloc(sizeof(int));
	*min = INT_MAX;
	#pragma omp parallel for schedule(dynamic)
	for(int a = 0; a < *numsLength; a++)
		if (nums[a] < *min)
			*min = nums[a];
		
	printf("%d\n", *min);
	
	return 0;
}