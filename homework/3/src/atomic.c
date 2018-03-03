#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <math.h>

//
// TODO: You need to implement an OpenMP parallel version using atomics
//
double parallelCompute_atomic( int64_t iterations, int numberOfThreads )
{
	double a = 0;
	double b = 1;
	double h = (b-a)/(iterations);
	double total = (h/2)*(sqrt(1-pow(a,2)) + sqrt(1-pow(b,2)));
	
	
	#pragma omp parallel num_threads(numberOfThreads) 
	{
		double x, toadd;
		int my_rank = omp_get_thread_num();
		int thread_count = omp_get_num_threads();
		int my_start = (my_rank == 0)? 1 : ((my_rank)*iterations/thread_count);
		int stop = (my_rank == thread_count-1)?(iterations):((my_rank+1)*iterations/thread_count);
		
		for(int i = my_start; i < stop; i++) {
			x = (a + i*h);

			toadd = (sqrt(1-pow(x,2)) * h);
			#pragma omp atomic
				total += toadd;
		}
	}
	double pi = total*4;
	return pi;
}
