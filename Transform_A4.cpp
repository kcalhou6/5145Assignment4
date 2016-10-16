#include <iostream>
#include <fstream>
#include <math.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

using namespace std;

// Given two timespec objects, start and end, return the amount of time elapsed from start to end
double runtime(timespec start, timespec end) {
	if ((end.tv_nsec-start.tv_nsec) < 0) {
		return (end.tv_sec - start.tv_sec - 1) + (((double)(end.tv_nsec - start.tv_nsec + 1000000000)) / (1e9));
	} else {
		return (end.tv_sec - start.tv_sec) + (((double)(end.tv_nsec - start.tv_nsec)) / (1e9));
	}
}

//argv[1] = length of array
//argv[2] = number of threads
//argv[3] = scheduling policy
//argv[4] = granularity (chunk size)
int main(int argc, char *argv[]) {
	// Get argv[1] for the length of array
	int numsLength = atoi(argv[1]);
	// Allocate space for array of length argv[1]
	int* nums = new int[numsLength];
	// Initialize the array to random numbers
	unsigned int seed = time(NULL);
	for (int a = 0; a < numsLength; a++) {
		seed++;
		nums[a] = rand_r(&seed);
	}

	// Get argv[2] for the number of threads to use
	int numThreads = atoi(argv[2]);
	// Set openMP num threads
	omp_set_num_threads(numThreads);

	// Get argv[3] for the schedule policy to use
	int schedule = atoi(argv[3]);

	//Variables to calculate and write runtime to a file
	timespec start, end;
	ofstream statFile;
	statFile.open("stats.txt", ios::app);

	// Return values to original in order to run loop on the same data as sequential had
	for (int a = 0; a < numsLength; a++)
		nums[a] = cbrt(nums[a]);

	/*
	 * cube all of the values in the array in parallel with omp for and write the runtime
	 * and speedup to the stats file
	 */
	// argv[3]=0 for static scheduling
	if (schedule == 0) {
		// Write schedule info to stats file
		statFile << "Static Scheduling using " << (numThreads) << " threads" << endl;

		// Execute parallel for
		clock_gettime(CLOCK_REALTIME, &start);
		#pragma omp parallel for schedule(static)
		for (int a = 0; a < numsLength; a++)
			nums[a] = pow(nums[a], 3);
		clock_gettime(CLOCK_REALTIME, &end);
	}
	// argv[3]!=0 for dynamic scheduling and argv[4] granularity
	else {
		// get argv[4] for the granularity
		int granularity = (int)strtol(argv[4], NULL, 10);

		// Write schedule info to stats file
		statFile << "Dynamic Scheduling using " << numThreads << " threads and granularity " << granularity << endl;

		// Execute parallel for
		clock_gettime(CLOCK_REALTIME, &start);
		#pragma omp parallel
		{
			#pragma omp for schedule(dynamic, granularity)
			for (int a = 0; a < numsLength; a++)
				nums[a] = pow(nums[a], 3);
		}
		clock_gettime(CLOCK_REALTIME, &end);
	}

	double tn = runtime(start, end);
	statFile << "runtime: " << tn << endl << endl;

	delete[] nums;

	return 0;
}
