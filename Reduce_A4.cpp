#include <climits>
#include <iostream>
#include <fstream>
#include <math.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

using namespace std;

double runtime(timespec start, timespec end) {
	if ((end.tv_nsec-start.tv_nsec) < 0) {
		return (end.tv_sec - start.tv_sec - 1) + (((double)(end.tv_nsec - start.tv_nsec + 1000000000)) / (1e9));
	} else {
		return (end.tv_sec - start.tv_sec) + (((double)(end.tv_nsec - start.tv_nsec)) / (1e9));
	}
}

double speedup(double t1, double tn) {
	return (t1 / tn);
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

	// Calculate sequential runtime and write to stats file
	clock_gettime(CLOCK_REALTIME, &start);
	int minimum = INT_MAX;
	for (int a = 0; a < numsLength; a++)
		if (nums[a] < minimum)
			minimum = nums[a];
	clock_gettime(CLOCK_REALTIME, &end);
	double t1 = runtime(start, end);
	statFile << "Reduce_A4" << endl << "Sequential runtime:" << t1 << endl;
	statFile << "min: " << minimum << endl;

	minimum = INT_MAX;

	// argv[3]=0 for static scheduling
	if (schedule == 0) {
		// Write schedule info to stats file
		statFile << "Static Scheduling using " << (numThreads) << " threads" << endl;

		// find the minimum value in parallel with omp reduction
		clock_gettime(CLOCK_REALTIME, &start);
		#pragma omp parallel for reduction(min: minimum) schedule(static)
		for (int a = 0; a < numsLength; a++)
			if (nums[a] < minimum)
				minimum = nums[a];
		clock_gettime(CLOCK_REALTIME, &end);
	}
	// argv[3]!=0 for dynamic scheduling and argv[4] granularity
	else {
		// get argv[4] for the granularity
		int granularity = (int)strtol(argv[4], NULL, 10);

		// Write schedule info to stats file
		statFile << "Dynamic Scheduling using " << (numThreads) << " threads and granularity " << granularity << endl;

		// find the minimum value in parallel with omp reduction
		clock_gettime(CLOCK_REALTIME, &start);
		#pragma omp parallel for reduction(min: minimum) schedule(dynamic, granularity)
		for (int a = 0; a < numsLength; a++)
			if (nums[a] < minimum)
				minimum = nums[a];
		clock_gettime(CLOCK_REALTIME, &end);
	}

	double tn = runtime(start, end);
	statFile << "runtime: " << tn << endl << "speedup: " << speedup(t1, tn) << endl;
	statFile << "min: " << minimum << endl << endl;

	// Free the memory allocated for the array
	delete[] nums;

	return 0;
}
