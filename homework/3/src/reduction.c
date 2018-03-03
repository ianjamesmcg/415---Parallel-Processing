#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <math.h>


double parallelCompute_reduction( int64_t iterations, int numberOfThreads )
{
	double a = 0;
	double b = 1;
	double h = (b-a)/iterations;
	double total = (h/2)*(sqrt(1-pow(a,2)) + sqrt(1-pow(b,2)));

	#pragma omp parallel for num_threads(numberOfThreads) \
		reduction(+: total)	
		for(int i = 1; i < iterations-1; i++) {
		total += (sqrt(1-pow((a + i*h),2)) * h);
		}
	double pi = total*4;
	return pi;

}
