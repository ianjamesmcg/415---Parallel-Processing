#include <stdlib.h>
#include <string.h>
#include <omp.h>


//
// TODO: - Parallelize the *** i-loop *** in this source file.
//
void summercamp_paralleli( const int kids_count, int * const heights, int * const temp, const int numberOfThreads ) {
	int count;
	int k,i,j;
	#pragma omp parallel num_threads(numberOfThreads) private(count,i,j,k) 
	{
		#pragma omp for
		for (  i = 0 ; i < kids_count ; i++ ) {
			count = 0;
			for (  j = 0 ; j < kids_count ; j++ ) {
				if(heights[j] < heights[i])
					count++;
		    		else if((heights[j] == heights[i]) && (j < i))
					count++;
			}
			temp[count] = heights[i];
	    	}
	
	    	// TODO: Modify the code below so that the copy can be made OpenMP parallel
	    	int myRank = omp_get_thread_num();
		int numThreads = omp_get_num_threads();
		int numPerThread = kids_count/numThreads;
		int myNumPerThread = numPerThread;
		if( myRank == numThreads - 1 ) {
			myNumPerThread = numPerThread + kids_count%numThreads;
		}
		#pragma omp for
		for( i = 0 ; i < numThreads ; i++) {
			memcpy( heights + myRank*numPerThread, temp + myRank*numPerThread, myNumPerThread*sizeof( int ) );
		}
	}
}
