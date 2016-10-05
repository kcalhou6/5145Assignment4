#include <iostream>
#include <fstream>
#include <math.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

using namespace std;

//argv[1] = length of array
//argv[2] = number of threads
//argv[3] = scheduling policy
int main(int argc, char *argv[]) {
	// Allocate space for, and get argv[1] for the length of array
	int * numsLength = (int *)(malloc(sizeof(int)));
	*numsLength = (int)strtol(argv[1], NULL, 10);
	// Allocate space for array of length argv[1]
	int* nums = (int *)(malloc((*numsLength) * sizeof(int)));

	// Initialize the array to random numbers
	for (int a = 0; a < *numsLength; a++)
		//nums[a] = rand();
		nums[(*numsLength) - a] = a;

	// Allocate space for, and get argv[2] for the number of threads to use	
	int *numThreads = (int*)(malloc(sizeof(int)));
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
	int* schedule = (int*)(malloc(sizeof(int)));
	*schedule = (int)strtol(argv[3], NULL, 10);
	
	/*
	// Allocate space for, and get argv[4] for the granularity to use
	int* granularity = malloc(sizeof(int));
	*granularity = (int)strtol(argv[4], NULL, 10);
	*/
	
	ofstream statFile;
	statFile.open("stats.txt", ios::app);
	timespec start, end; 
	
	if ((*schedule) == 0) {
		clock_gettime(CLOCK_REALTIME, &start);
		//#pragma omp parallel for schedule(static)
		for (int a = 0; a < *numsLength; a++)
			nums[a] = pow(nums[a], 2);
		clock_gettime(CLOCK_REALTIME, &end);
		statFile << "Static Scheduling using " << (*numThreads) << " threads" << std::endl;
	} else if ((*schedule) == 1) {
		clock_gettime(CLOCK_REALTIME, &start);
		#pragma omp parallel for schedule(dynamic, 1)
		for (int a = 0; a < *numsLength; a++)
			nums[a] = pow(nums[a], 2);
		clock_gettime(CLOCK_REALTIME, &end);
		statFile << "Dynamic Scheduling using " << (*numThreads) << " threads and granularity 1" << std::endl;
	} else if ((*schedule) == 2) {
		clock_gettime(CLOCK_REALTIME, &start);
		#pragma omp parallel for schedule(dynamic, 1000)
		for (int a = 0; a < *numsLength; a++)
			nums[a] = pow(nums[a], 2);
		clock_gettime(CLOCK_REALTIME, &end);
		statFile << "Dynamic Scheduling using " << (*numThreads) << " threads and granularity 10^3" << std::endl;
	} else {
		clock_gettime(CLOCK_REALTIME, &start);
		#pragma omp parallel for schedule(dynamic, 10000)
		for (int a = 0; a < *numsLength; a++)
			nums[a] = pow(nums[a], 2);
		clock_gettime(CLOCK_REALTIME, &end);
		statFile << "Dynamic Scheduling using " << (*numThreads) << " threads and granularity 10^5" << std::endl;
	}
		
	if ((end.tv_nsec-start.tv_nsec) < 0) {
			statFile << (end.tv_sec - start.tv_sec - 1) << ":";
			statFile << (end.tv_nsec - start.tv_nsec + 1000000000) << std::endl;
	} else {
			statFile << (end.tv_sec - start.tv_sec) << ":";
			statFile << (end.tv_nsec - start.tv_nsec) << std::endl;	
	}
	
	return 0;
}