#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <omp.h>

#include "summercamp_paralleli.h"
#include "summercamp_parallelj.h"

#include "summercamp_paralleli_static.h"
#include "summercamp_parallelj_static.h"

#include "summercamp_paralleli_dynamic.h"
#include "summercamp_parallelj_dynamic.h"

#include "summercamp_paralleli_static_chunk.h"
#include "summercamp_paralleli_dynamic_chunk.h"
#include "summercamp_parallelj_static_chunk.h"
#include "summercamp_parallelj_dynamic_chunk.h"
int cmp_func(const void *a , const void *b)
{
    return ( *(int *) a - *(int *) b );
}


//
// Compares the arrays stored at a and b. Returns 1 if their elements are the same, 0 otherwise.
//
int difference( int * a, int * b, int n ) {

    int i;
    for(i = 0 ; i < n ; ++i){
        if(a[i] != b[i])
            return 0;
    }
    return 1;
}


//
// Main method. Runs tests and outputs results to stdout.
//
// Results are output on a single line with values separated by commas.
// Values are output in the following order:
//
//  1.  Number of threads.
//  2.  Runtime of sequential quicksort routine.
//  3.  Runtime of OpenMP i-parallel summercamp routine.
//  4.  Value indicating correctness of OpenMP i-parallel summercamp implementation.
//  5.  Runtime of OpenMP j-parallel summercamp routine.
//  6.  Value indicating correctness of OpenMP j-parallel summercamp implementation.
//
int main(int argc, char* argv[])
{
    int n, diff;
    int numberOfThreads;
    double time_i, time_j, time_quicksort;

    srand(time(NULL));

    // Get array size and number of threads from the command line
    if (argc > 1) {

        n = strtol(argv[1], NULL, 10);
        numberOfThreads = strtol(argv[2], NULL, 10);

    } else {

        fprintf( stderr, "\nWhen running this program, please include the array size and the number of threads on command line.\n");
        return 0;
    }

    int *a = malloc(n*sizeof(int));
    int *b = malloc(n*sizeof(int));
    int *c = malloc(n*sizeof(int));
    int *temp = malloc(n*sizeof(int));

    for ( int i = 0; i < n; ++i ) {

        c[i] = b[i] = a[i] = rand() % 1000;
        temp[i] = 0;
    }

    // Output number of threads.
    printf("%d, %d,", n,numberOfThreads);

    // Run quicksort library routine.
    {
        double start = omp_get_wtime();
        qsort(a, n, sizeof(int), cmp_func);
        double stop = omp_get_wtime();
        time_quicksort = stop - start;
    }
    printf("quicksort: %.4e\n", time_quicksort);
/*
    // Run i-version of summercamp.
    {
        double start = omp_get_wtime();
        summercamp_paralleli( n, b, temp, numberOfThreads );
        double stop = omp_get_wtime();
        time_i = stop - start;
    }
    printf("i %.4e,", time_i);

    diff = difference(a,b,n);
    printf("%d\n", diff);

    if ( diff == 0 )
        fprintf( stdout, "i-version of summercamp produces incorrect result.\n" );

    // Run j-version of summercamp.
    {
        double start = omp_get_wtime();
        summercamp_parallelj( n, c, temp, numberOfThreads );
        double stop = omp_get_wtime();
        time_j = stop - start;
    }
    printf("j %.4e,", time_j);

    diff = difference(a,c,n);
    printf("%d\n", diff);

    if ( diff == 0 )
        fprintf( stdout, "j-version of summercamp produces incorrect result.\n" );

    // Run i-version of summercamp with static scheduling
    {
        double start = omp_get_wtime();
        summercamp_paralleli_static( n, b, temp, numberOfThreads );
        double stop = omp_get_wtime();
        time_i = stop - start;
    }
    printf("i static %.4e,", time_i);

    diff = difference(a,b,n);
    printf("%d\n", diff);

    if ( diff == 0 )
        fprintf( stdout, "i-static-version of summercamp produces incorrect result.\n" );

    // Run j-version of summercamp with static scheduling
    {
        double start = omp_get_wtime();
        summercamp_parallelj_static( n, c, temp, numberOfThreads );
        double stop = omp_get_wtime();
        time_j = stop - start;
    }
    printf("j static %.4e,", time_j);

    diff = difference(a,c,n);
    printf("%d\n", diff);

    if ( diff == 0 )
        fprintf( stdout, "j-static-version of summercamp produces incorrect result.\n" );
    // Run i-version of summercamp with dynamic scheduling
    {
        double start = omp_get_wtime();
        summercamp_paralleli_dynamic( n, b, temp, numberOfThreads );
        double stop = omp_get_wtime();
        time_i = stop - start;
    }
    printf("i dynamic %.4e,", time_i);

    diff = difference(a,b,n);
    printf("%d\n", diff);

    if ( diff == 0 )
        fprintf( stdout, "i-dynamic-version of summercamp produces incorrect result.\n" );

    // Run j-version of summercamp with dynamic scheduling
    {
        double start = omp_get_wtime();
        summercamp_parallelj_dynamic( n, c, temp, numberOfThreads );
        double stop = omp_get_wtime();
        time_j = stop - start;
    }
    printf("j dynamic %.4e,", time_j);

    diff = difference(a,c,n);
    printf("%d\n", diff);

    if ( diff == 0 )
        fprintf( stdout, "j-dynamic-version of summercamp produces incorrect result.\n" );
*/
    //CHUNK SIZE VERSIONS

	//Static i Chunk
/*
    // Run i-version of summercamp with static scheduling and chunk size 1
    {
        double start = omp_get_wtime();
        summercamp_paralleli_static_chunk( n, b, temp, numberOfThreads, 1 );
        double stop = omp_get_wtime();
        time_i = stop - start;
    }
    printf("i static chunk 1 %.4e,", time_i);

    diff = difference(a,b,n);
    printf("%d\n", diff);

    if ( diff == 0 )
        fprintf( stdout, "i-static-version of summercamp produces incorrect result.\n" );

    // Run i-version of summercamp with static scheduling and chunk size 2
    {
        double start = omp_get_wtime();
        summercamp_paralleli_static_chunk( n, b, temp, numberOfThreads, 2 );
        double stop = omp_get_wtime();
        time_i = stop - start;
    }
    printf("i static chunk 2 %.4e,", time_i);

    diff = difference(a,b,n);
    printf("%d\n", diff);

    if ( diff == 0 )
        fprintf( stdout, "i-static-version of summercamp produces incorrect result.\n" );

    // Run i-version of summercamp with static scheduling and chunk size 5
    {
        double start = omp_get_wtime();
        summercamp_paralleli_static_chunk( n, b, temp, numberOfThreads, 5 );
        double stop = omp_get_wtime();
        time_i = stop - start;
    }
    printf("i static chunk 5 %.4e,", time_i);

    diff = difference(a,b,n);
    printf("%d\n", diff);

    if ( diff == 0 )
        fprintf( stdout, "i-static-version of summercamp produces incorrect result.\n" );

    // Run i-version of summercamp with static scheduling and chunk size 10
    {
        double start = omp_get_wtime();
        summercamp_paralleli_static_chunk( n, b, temp, numberOfThreads, 10 );
        double stop = omp_get_wtime();
        time_i = stop - start;
    }
    printf("i static chunk 10 %.4e,", time_i);

    diff = difference(a,b,n);
    printf("%d\n", diff);

    if ( diff == 0 )
        fprintf( stdout, "i-static-version of summercamp produces incorrect result.\n" );
*/
    // Run i-version of summercamp with static scheduling and chunk size 1000
    {
        double start = omp_get_wtime();
        summercamp_paralleli_static_chunk( n, b, temp, numberOfThreads, 1000 );
        double stop = omp_get_wtime();
        time_i = stop - start;
    }
    printf("i static chunk 1000 %.4e,", time_i);

    diff = difference(a,b,n);
    printf("%d\n", diff);

    if ( diff == 0 )
        fprintf( stdout, "i-static-version of summercamp produces incorrect result.\n" );
/*
	//Dynamic i Chunk

    // Run i-version of summercamp with dynamic scheduling and chunk size 1
    {
        double start = omp_get_wtime();
        summercamp_paralleli_dynamic_chunk( n, b, temp, numberOfThreads, 1 );
        double stop = omp_get_wtime();
        time_i = stop - start;
    }
    printf("i dynamic chunk 1 %.4e,", time_i);

    diff = difference(a,b,n);
    printf("%d\n", diff);

    if ( diff == 0 )
        fprintf( stdout, "i-dynamic-version of summercamp produces incorrect result.\n" );

    // Run i-version of summercamp with dynamic scheduling and chunk size 2
    {
        double start = omp_get_wtime();
        summercamp_paralleli_dynamic_chunk( n, b, temp, numberOfThreads, 2 );
        double stop = omp_get_wtime();
        time_i = stop - start;
    }
    printf("i dynamic chunk 2 %.4e,", time_i);

    diff = difference(a,b,n);
    printf("%d\n", diff);

    if ( diff == 0 )
        fprintf( stdout, "i-dynamic-version of summercamp produces incorrect result.\n" );

    // Run i-version of summercamp with dynamic scheduling and chunk size 5
    {
        double start = omp_get_wtime();
        summercamp_paralleli_dynamic_chunk( n, b, temp, numberOfThreads, 5 );
        double stop = omp_get_wtime();
        time_i = stop - start;
    }
    printf("i dynamic chunk 5 %.4e,", time_i);

    diff = difference(a,b,n);
    printf("%d\n", diff);

    if ( diff == 0 )
        fprintf( stdout, "i-dynamic-version of summercamp produces incorrect result.\n" );

    // Run i-version of summercamp with static scheduling and chunk size 10
    {
        double start = omp_get_wtime();
        summercamp_paralleli_dynamic_chunk( n, b, temp, numberOfThreads, 10 );
        double stop = omp_get_wtime();
        time_i = stop - start;
    }
    printf("i dynamic chunk 10 %.4e,", time_i);

    diff = difference(a,b,n);
    printf("%d\n", diff);

    if ( diff == 0 )
        fprintf( stdout, "i-dynamic-version of summercamp produces incorrect result.\n" );
*/
    // Run i-version of summercamp with dynamic scheduling and chunk size 1000
    {
        double start = omp_get_wtime();
        summercamp_paralleli_dynamic_chunk( n, b, temp, numberOfThreads, 1000 );
        double stop = omp_get_wtime();
        time_i = stop - start;
    }
    printf("i dynamic chunk 1000 %.4e,", time_i);

    diff = difference(a,b,n);
    printf("%d\n", diff);

    if ( diff == 0 )
        fprintf( stdout, "i-dynamic-version of summercamp produces incorrect result.\n" );
/*
	//Static j Chunk

    // Run j-version of summercamp with static scheduling and chunk size 1
    {
        double start = omp_get_wtime();
        summercamp_parallelj_static_chunk( n, b, temp, numberOfThreads, 1 );
        double stop = omp_get_wtime();
        time_j = stop - start;
    }
    printf("j static chunk 1 %.4e,", time_j);

    diff = difference(a,b,n);
    printf("%d\n", diff);

    if ( diff == 0 )
        fprintf( stdout, "j-static-version of summercamp produces incorrect result.\n" );

    // Run j-version of summercamp with static scheduling and chunk size 2
    {
        double start = omp_get_wtime();
        summercamp_parallelj_static_chunk( n, b, temp, numberOfThreads, 2 );
        double stop = omp_get_wtime();
        time_j = stop - start;
    }
    printf("j static chunk 2 %.4e,", time_j);

    diff = difference(a,b,n);
    printf("%d\n", diff);

    if ( diff == 0 )
        fprintf( stdout, "j-static-version of summercamp produces incorrect result.\n" );

    // Run j-version of summercamp with static scheduling and chunk size 5
    {
        double start = omp_get_wtime();
        summercamp_parallelj_static_chunk( n, b, temp, numberOfThreads, 5 );
        double stop = omp_get_wtime();
        time_j = stop - start;
    }
    printf("j static chunk 5 %.4e,", time_j);

    diff = difference(a,b,n);
    printf("%d\n", diff);

    if ( diff == 0 )
        fprintf( stdout, "j-static-version of summercamp produces incorrect result.\n" );

    // Run j-version of summercamp with static scheduling and chunk size 10
    {
        double start = omp_get_wtime();
        summercamp_parallelj_static_chunk( n, b, temp, numberOfThreads, 10 );
        double stop = omp_get_wtime();
        time_j = stop - start;
    }
    printf("j static chunk 10 %.4e,", time_j);

    diff = difference(a,b,n);
    printf("%d\n", diff);

    if ( diff == 0 )
        fprintf( stdout, "j-static-version of summercamp produces incorrect result.\n" );
*/
    // Run j-version of summercamp with static scheduling and chunk size 1000
    {
        double start = omp_get_wtime();
        summercamp_parallelj_static_chunk( n, b, temp, numberOfThreads, 1000 );
        double stop = omp_get_wtime();
        time_j = stop - start;
    }
    printf("j static chunk 1000 %.4e,", time_j);

    diff = difference(a,b,n);
    printf("%d\n", diff);

    if ( diff == 0 )
        fprintf( stdout, "j-static-version of summercamp produces incorrect result.\n" );
/*
	//Dynamic j Chunk


    // Run j-version of summercamp with dynamic scheduling and chunk size 1
    {
        double start = omp_get_wtime();
        summercamp_parallelj_dynamic_chunk( n, b, temp, numberOfThreads, 1 );
        double stop = omp_get_wtime();
        time_j = stop - start;
    }
    printf("j dynamic chunk 1 %.4e,", time_j);

    diff = difference(a,b,n);
    printf("%d\n", diff);

    if ( diff == 0 )
        fprintf( stdout, "j-dynamic-version of summercamp produces incorrect result.\n" );

    // Run j-version of summercamp with dynamic scheduling and chunk size 2
    {
        double start = omp_get_wtime();
        summercamp_parallelj_dynamic_chunk( n, b, temp, numberOfThreads, 2 );
        double stop = omp_get_wtime();
        time_j = stop - start;
    }
    printf("j dynamic chunk 2 %.4e,", time_j);

    diff = difference(a,b,n);
    printf("%d\n", diff);

    if ( diff == 0 )
        fprintf( stdout, "j-dynamic-version of summercamp produces incorrect result.\n" );

    // Run j-version of summercamp with dynamic scheduling and chunk size 5
    {
        double start = omp_get_wtime();
        summercamp_parallelj_dynamic_chunk( n, b, temp, numberOfThreads, 5 );
        double stop = omp_get_wtime();
        time_j = stop - start;
    }
    printf("j dynamic chunk 5 %.4e,", time_j);

    diff = difference(a,b,n);
    printf("%d\n", diff);

    if ( diff == 0 )
        fprintf( stdout, "j-dynamic-version of summercamp produces incorrect result.\n" );

    // Run j-version of summercamp with static scheduling and chunk size 10
    {
        double start = omp_get_wtime();
        summercamp_parallelj_dynamic_chunk( n, b, temp, numberOfThreads, 10 );
        double stop = omp_get_wtime();
        time_j = stop - start;
    }
    printf("j dynamic chunk 10 %.4e,", time_j);

    diff = difference(a,b,n);
    printf("%d\n", diff);

    if ( diff == 0 )
        fprintf( stdout, "j-dynamic-version of summercamp produces incorrect result.\n" );
*/
    // Run j-version of summercamp with dynamic scheduling and chunk size 1000
    {
        double start = omp_get_wtime();
        summercamp_parallelj_dynamic_chunk( n, b, temp, numberOfThreads, 1000 );
        double stop = omp_get_wtime();
        time_j = stop - start;
    }
    printf("j dynamic chunk 1000 %.4e,", time_j);

    diff = difference(a,b,n);
    printf("%d\n", diff);

    if ( diff == 0 )
        fprintf( stdout, "j-dynamic-version of summercamp produces incorrect result.\n" );
    
    free(a);
    free(b);
    free(c);
    free(temp);
}
