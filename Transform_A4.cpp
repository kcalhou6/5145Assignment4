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
	int numsLength = (int)strtol(argv[1], NULL, 10);
	// Allocate space for array of length argv[1]
	int* nums = (int *)(malloc(numsLength * sizeof(int)));
	if (nums == NULL)
		return -1;
	// Initialize the array to random numbers
	unsigned int seed = time(NULL);
	for (int a = 0; a < numsLength; a++) {	
		seed++;		
		nums[a] = rand_r(&seed);
	}

	// Get argv[2] for the number of threads to use	
	int numThreads = (int)strtol(argv[2], NULL, 10);
	// Set openMP num threads
	omp_set_num_threads(numThreads);
	
	// Get argv[3] for the schedule policy to use
	int schedule = (int)strtol(argv[3], NULL, 10);
	
	//Variables to calculate and write runtime to a file
	timespec start, end;
	ofstream statFile;
	statFile.open("stats.txt", ios::app);
	
	// Calculate sequential runtime
	clock_gettime(CLOCK_REALTIME, &start);
	for (int a = 0; a < numsLength; a++)
		nums[a] = pow(nums[a], 2);
	clock_gettime(CLOCK_REALTIME, &end);
	double t1 = runtime(start, end);
	statFile << "Transform_A4" << endl << "Sequential runtime:" << t1 << endl;

	// Return values to original in order to run loop on the same data as sequential had
	for (int a = 0; a < numsLength; a++)
		nums[a] = sqrt(nums[a]);

	// argv[3]=0 for static scheduling
	if (schedule == 0) {
		/*
		 * Using this method to set schedule type as static requires chunk length input
		 * set the chunk length as a uniform as n/threads
		 */	
		omp_set_schedule(omp_sched_static, (numsLength/numThreads));
		
		// Write schedule info to stats file
		statFile << "Static Scheduling using " << (numThreads) << " threads" << endl;
	} 
	// argv[3]!=0 for dynamic scheduling and argv[4] granularity
	else {
		// get argv[4] for the granularity
		int granularity = (int)strtol(argv[4], NULL, 10);

		// set the schedule to type to dynamic and the provided granularity 		
		omp_set_schedule(omp_sched_dynamic, granularity);
		
		// Write schedule info to stats file
		statFile << "Dynamic Scheduling using " << numThreads << " threads and granularity " << granularity << endl;
	}

	clock_gettime(CLOCK_REALTIME, &start);	
	/*
	 * square all of the values in the array in parallel with omp for and write the runtime
	 * and speedup to the stats file
	 */
	#pragma omp parallel 
	{
		#pragma omp for
		for (int a = 0; a < numsLength; a++)
			nums[a] = pow(nums[a], 2);
	}
	clock_gettime(CLOCK_REALTIME, &end);
	double tn = runtime(start, end);
	statFile << "runtime: " << tn << endl << "speedup: " << speedup(t1, tn) << endl << endl;

	free(nums);	

	return 0;
}
