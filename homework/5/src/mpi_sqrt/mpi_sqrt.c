#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include<math.h>
#include <mpi.h>

void get_random(int * const num,int N,int p){
    
    int r,i,sum,prev,lower,upper;
    
    srand(time(NULL));

    sum = 0;

    for(i=0;i<p-1;++i){
        lower = N/p - 5;
        upper = N/p + 6;
        r = lower + rand()%(upper-lower);

        sum += r;
        num[i] = r;
    }
    num[i] = N - sum;

}

int main(int argc, char** argv)
{
    int size, MyRank, i,s,N;
    double *arr, *newarr, recvbuf,*seq_arr;
    double start_time,end_time;
    MPI_Status status;	
    
	srand(time(NULL));
    int procs;
    //procs = atoi(argv[2]);
    
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &MyRank);
    MPI_Comm_size(MPI_COMM_WORLD, &procs);

	N = atoi(argv[1]);
    //this is the given array which has the record of distribution
    int *proc_elems = (int*)malloc(sizeof(int)*procs);
	//Here the given array is generated
	get_random(proc_elems,N,procs);	
    
	if(MyRank == 0) {

		// original array
		arr = (double *)malloc(sizeof(double) * N);
		//sequentially calculated new array
    	newarr = (double*)malloc(sizeof(double) * N);
		// mpi implemented new array
		seq_arr = (double*)malloc(sizeof(double) * N);
		//we are printing first 10 elements of the real array for checking
		printf("The old array: \n");
		for (i = 0; i < N; i++)
		{
			arr[i] = rand() % 100;
			if(i<10)
				printf("%f ", arr[i]);
		}
		printf("\n");
		// taking the sequential time
		start_time = MPI_Wtime();
		for(i = 0 ; i < N ; i++){
			seq_arr[i] = sqrt(arr[i]);
		}
		end_time = MPI_Wtime();
		printf("Sequential Time = %lf\n",end_time-start_time);
		free(seq_arr);

    	start_time = MPI_Wtime();
		s=0;
		for (i=1 ; i < procs ; i++) {	
			s += proc_elems[i-1];
			MPI_Send(&arr[s], proc_elems[i], MPI_DOUBLE, i, 100, MPI_COMM_WORLD); 	
		}
	}

	else {
		arr = (double *)malloc(sizeof(double) * proc_elems[MyRank]);
		MPI_Recv(arr, proc_elems[MyRank], MPI_DOUBLE, 0, 100, MPI_COMM_WORLD, &status);			
    	newarr = (double*)malloc(sizeof(double) * proc_elems[MyRank]);
	}
    
    //Your mpi implementation should start from here
	for(i=0 ; i < proc_elems[MyRank] ; i++){
		newarr[i] = sqrt(arr[i]);
	} 

	if(MyRank == 0) {
		s=0;
		for(i=1 ; i < procs ; i++) {
			s += proc_elems[i-1];	
			MPI_Recv(&newarr[s], proc_elems[i], MPI_DOUBLE, i, 123, MPI_COMM_WORLD, &status);
		}	
    	end_time = MPI_Wtime();
    	
		printf("new array after mpi implementation : ");
    	for(i = 0 ; i < 10 ; i++){
        	printf("%.6lf  ",newarr[i]); 
    	}
    	printf("\n");
    	printf("parallel time needed %.6lf\n",end_time-start_time);    
	}
	else {
		MPI_Send(newarr, proc_elems[MyRank], MPI_DOUBLE, 0, 123, MPI_COMM_WORLD);
	}
	
	MPI_Finalize();
	return 0;
}

