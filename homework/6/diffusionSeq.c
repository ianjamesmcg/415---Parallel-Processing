// diffusionSeq.c
//
// Program to model 1D heat diffusion equation (sequential)

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

float* seq_diffusion(float *a, float *b, long int size, long int time);

int main (int argc, char *argv[])
{
	float *temp;
	long int i, rodsize, tempsteps;
	struct timeval start, finish;

	if (argc != 3) {
		printf ("usage: diffusionSeq partitions tempsteps\n");
		exit(-1);
	}
	else {
		rodsize = atol (argv[1]);
		tempsteps = atol (argv[2]);
	}
	  
	float *array1 = (float *) malloc ((rodsize + 2) * sizeof (float));
	float *array2 = (float *) malloc ((rodsize + 2) * sizeof (float));
    
	array1[0] = 100.0;
	array2[0] = 100.0;
	for (i=1; i <= rodsize; i++) {
		array1[i] = 23.0;
		array2[i] = 23.0;
	}
	array1[rodsize+1] = 23.0;
	array2[rodsize+1] = 23.0;
	  
	gettimeofday (&start, NULL);
		temp = seq_diffusion (array1, array2, rodsize, tempsteps);
	gettimeofday (&finish, NULL);
	long int index = (int) (0.7 * rodsize);
	printf ("Temperature at location 0.7 at time %ld: %f\n", tempsteps, temp[index]);
	printf ("Elapsed time: %f sec.\n\n", (finish.tv_sec - start.tv_sec) 
	                                   + (finish.tv_usec - start.tv_usec)/1000000.0);
	
	return 0;
}


float* seq_diffusion(float *a, float *b, long int size, long int time)
{
	long int i, k;
	float *tempPtr;

	for (i=0; i<time; i++) {
		for (k=1; k <= size; k++) {
			a[k] = (b[k-1] + b[k+1]) / 2.0;
		}
		a[size+1] = a[size];
		tempPtr = a;
		a = b;
		b = tempPtr;
	}
		return a;
}
