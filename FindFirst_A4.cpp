#include <climits>
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
//argv[2] = number to look for
//argv[3] = number of threads
//argv[4] = time complexity (0 for n, pos otherwise)
int main(int argc, char *argv[]) {
	// Get argv[1] for the length of array
	int numsLength = atoi(argv[1]);

	// Allocate space for array of length argv[1]
	int* nums = new int[numsLength];
	// Initialize the array to random numbers
	/*
	unsigned int seed = time(NULL);
	for (int a = 0; a < numsLength; a++) {
		seed++;
		nums[a] = rand_r(&seed);
	}
	*/

	for (int a = 0; a < numsLength; a++) {
		nums[numsLength - a - 1] = a;
	}

	// Get argv[2] for the number to search for
	int searchVal = atoi(argv[2]);

	// Get argv[3] for the number of threads to use
	int numThreads = atoi(argv[3]);
	// Set openMP num threads
	omp_set_num_threads(numThreads);

	// Get argv[4] for the time complexity to aim for
	int complex = atoi(argv[4]);

	//Variables to calculate and write runtime to a file
	timespec start, end;
	ofstream statFile;
	statFile.open("stats.txt", ios::app);
	statFile << "FindFirst_A4" << endl << "Array Length: " << numsLength << endl;

	int pos;

	// complex == 0 denotes time complexity n
	if (complex == 0) {
		pos = INT_MAX;
		// Mark time right before start of parallel loop
		clock_gettime(CLOCK_REALTIME, &start);
		// Find the first occurence of searchVal in parallel with openMP
		#pragma omp parallel for schedule(static)
		for (int a = 0; a < numsLength; a++)
			if ((nums[a] == searchVal) && (a < pos)) {
				pos = a;
			}
		clock_gettime(CLOCK_REALTIME, &end);
		double tn = runtime(start, end);
		statFile << "Static scheduling using " << numThreads << " threads for (n) complexity " << endl;
		statFile << "runtime: " << tn << endl << "pos: " << pos << endl << endl;
	}
	// complex != 0 denotes time complexity pos
	else {
		// Calculate sequential runtime and write to stats file
		clock_gettime(CLOCK_REALTIME, &start);
		int pos = -1;
		for (int a = 0; a < numsLength; a++)
			if (nums[a] == searchVal) {
				pos = a;
				break;
			}
		clock_gettime(CLOCK_REALTIME, &end);
		double t1 = runtime(start, end);
		statFile << "FindFirst_A4" << endl << "Sequential runtime:" << t1 << endl;
		statFile << "pos: " << pos << endl;

		pos = INT_MAX;
		bool stop = false;
		// Mark time right before start of parallel loop
		clock_gettime(CLOCK_REALTIME, &start);
		// Find the first occurence of searchVal in parallel with openMP
		#pragma omp parallel for schedule(static, 1)
		for (int a = 0; a < numsLength; a++)
			if (!stop && (nums[a] == searchVal) && (a < pos)) {
				pos = a;
				stop = true;
				a = numsLength;
			}
		clock_gettime(CLOCK_REALTIME, &end);
		double tn = runtime(start, end);
		statFile << "Static,1 scheduling using " << numThreads << " threads for (n) complexity " << endl;
		statFile << "runtime: " << tn << endl << "pos: " << pos << endl << endl;
	}

	// Free the memory allocated for the array
	delete[] nums;

	return 0;
}
