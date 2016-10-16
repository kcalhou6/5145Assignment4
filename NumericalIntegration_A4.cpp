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

// Returns the value of f(x)=x^3 if funct=0, or the value of f(x) = sqrt(x) otherwise
// Either of which is looped to the value of intensity
double f(double x, int function, int intensity) {
	double y = x;
	if (function == 0)
		for(int i = 0; i < intensity; i++)
			y = pow(y, 3);
	else
		for(int i = 0; i < intensity; i++)
			y = sqrt(y);
	return y;
}

//argv[1] = a
//argv[2] = b
//argv[3] = n
//argv[4] = function
//argv[5] = intensity
//argv[6] = number of threads
//argv[7] = granularity
int main(int argc, char *argv[]) {
	// Get argv[1] for lower limit a
	double a = strtod(argv[1], NULL);

	// Get argv[2] for upper limit b
	double b = strtod(argv[2], NULL);

	// Get argv[3] for number of points n
	int n = atoi(argv[3]);

	// Get argv[4] for function
	int function = atoi(argv[4]);

	// Get argv[5] for intensity
	int intensity = atoi(argv[5]);

	// Get argv[6] for upper limit b
	int numThreads = atoi(argv[6]);

	// Set openMP num threads
	omp_set_num_threads(numThreads);

	//Variables to calculate and write runtime to a file
	timespec start, end;
	ofstream statFile;
	statFile.open("stats.txt", ios::app);
	statFile << "NumericalIntegration_A4" << endl << "Array Length: " << numsLength << endl;
	statFile << "Number of Points: " << n << endl << "Intensity: " << intensity << endl;

	// Set the schedule type to dynamic and granularity to argv[7]
	int granularity = (int)strtol(argv[7], NULL, 10);
	omp_set_schedule(omp_sched_dynamic, granularity);

	// Write schedule info to stats file
	statFile << "Dynamic Scheduling using " << numThreads << " threads and granularity " << granularity << endl;

	approx = 0;
	// Mark time right before start of parallel loop
	clock_gettime(CLOCK_REALTIME, &start);
	// execute numerical integration in parallel with omp
	#pragma omp parallel for reduction(+:approx)
	for(int i = 0; i < n; i++)
		approx += (f(a + (i + 0.5) * (b - a) / n, function, intensity) * (b - a) / n);
	// Mark time right after end of parallel loop
	clock_gettime(CLOCK_REALTIME, &end);
	double tn = runtime(start, end);

	// Write runtime and speedup to stats file
	statFile << "runtime: " << tn << endl << endl;

	return 0;
}
