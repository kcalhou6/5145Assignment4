#include <climits>
#include <iostream>
#include <fstream>
#include <math.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <exception>

using namespace std;

// Given two timespec objects, start and end, return the amount of time elapsed from start to end
double runtime(timespec start, timespec end) {
	if ((end.tv_nsec-start.tv_nsec) < 0) {
		return (end.tv_sec - start.tv_sec - 1) + (((double)(end.tv_nsec - start.tv_nsec + 1000000000)) / (1e9));
	} else {
		return (end.tv_sec - start.tv_sec) + (((double)(end.tv_nsec - start.tv_nsec)) / (1e9));
	}
}

// Given two doubles, t1 and tn, return S(N) = t1/tn
double speedup(double t1, double tn) {
	return (t1 / tn);
}

//argv[1] = length of array
//argv[2] = number of threads
int main(int argc, char *argv[]) {
	// Get argv[1] for the length of array
	int numsLength = atoi(argv[1]);//(int)strtol(argv[1], NULL, 10);

	// Allocate space for array of length argv[1]
	int* nums = new int[numsLength];

	// Initialize the array to random numbers
	unsigned int seed = time(NULL);
	for (int a = 0; a < numsLength; a++) {
		seed++;
		//nums[a] = rand_r(&seed);
		nums[a] = 1;
		//nums[a] = a;
	}

	// Get argv[2] for the number of threads to use
	int numThreads = (int)strtol(argv[2], NULL, 10);
	// Set openMP num threads
	omp_set_num_threads(numThreads);

	//Variables to calculate and write runtime to a file
	timespec start, end;
	ofstream statFile;
	statFile.open("stats.txt", ios::app);

	// Calculate sequential runtime and write to stats file
	clock_gettime(CLOCK_REALTIME, &start);
	for (int a = 1; a < numsLength; a++)
		nums[a] += nums[a - 1];
	clock_gettime(CLOCK_REALTIME, &end);
	double t1 = runtime(start, end);
	statFile << "PrefixSum_A4" << endl << "Sequential runtime:" << t1 << endl;

	printf("\nSequential sum: %d\n", nums[numsLength - 1]);

	// Return the array to its original values to ensure calculation runs on the same data
//	for (int a = numsLength; a > 0; a--)
//		nums[a] -= nums[a - 1];
	for (int a = 0; a < numsLength; a++)
		nums[a] = 1;
//		nums[a] = a;

	printf("Initial end value: %d\n", nums[numsLength - 1]);

	// Allocate space for array of length argv[1]
	int * sub = new int[numThreads];
	int i, thread, tStart, tEnd, chunkSize = (numsLength / numThreads), extendedThreads = ((numsLength) % numThreads);

	clock_gettime(CLOCK_REALTIME, &start);
	#pragma omp parallel private(i, thread, tStart, tEnd)
	{
		// Divide nums into sub-arrays and calculate the prefix sum of each
		#pragma omp for schedule(static)
		for (i = 0; i < numsLength; i++) {
			// Find the boundaries for the thread
			thread = omp_get_thread_num();
			tStart = (thread * chunkSize);
			tEnd = ((thread + 1) * chunkSize);
			if (extendedThreads == 0)
				tEnd--;
			else {
				if (thread < extendedThreads) {
					tStart += thread;
					tEnd += thread;
				}
				else {
					tStart += extendedThreads;
					tEnd += (extendedThreads - 1);
				}
			}

			// if i is at index 1+ of the sub-array, Increment nums[1] by nums[i-1]
			if (i > tStart) {
				nums[i] += nums[i - 1];
			}

			// if at the last index of the sub-array and not in the final thread, increment
			// the offset (prefix sum of the sub-array) for the thread by the sub-array prefix sum
			if ((i == tEnd) && (thread != (numThreads - 1)))
				sub[thread + 1] = nums[i];
		}

		// Compute the prefix sum of the partial array, this step must be sequential
		#pragma omp single
		{
			for (int i = 1; i < numThreads; i++)
				sub[i] += sub[i-1];
		}

		// Increment nums with the corresponding value from sub
		#pragma omp for schedule(static)
		for (i = 0; i < numsLength; i++) {
			thread = omp_get_thread_num();
			if (thread != 0)
				nums[i] += sub[thread];
		}
	}
	clock_gettime(CLOCK_REALTIME, &end);
	double tn = runtime(start, end);
	statFile << "runtime: " << tn << endl << "speedup: " << speedup(t1, tn) << endl << endl;

	printf("%d\n\n", nums[numsLength -1]);

	// Free the memory allocated for the arrays
	delete[] nums;
	delete[] sub;

	return 0;
}
