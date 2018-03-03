#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <math.h>

double sequentialCompute( int64_t iterations )
{
	double a = 0;
	double b = 1;
	double h = (b-a)/iterations;
	double total = (h/2)*(sqrt(1-pow(a,2)) + sqrt(1-pow(b,2)));
	
	double x;
	for(int i = 1; i < iterations-1; i++) {
		x = (a + i*h);
		total += (sqrt(1-pow(x,2)) * h);
	}
	double pi = total*4;
	return pi;
}
