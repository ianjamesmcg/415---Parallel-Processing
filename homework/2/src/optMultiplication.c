#include "optMultiplication.h"

void optMultiplication(double* output, double* input_matrix, double* input_vector,int N,int M,int B){

	int i,j,k,l;
	int b = B/8; //doubles per block

	for (i = 0; i < N/b; i++){	//i is N side block index
		for (j = 0; j < M/b; j++){	//j is M side block index
			for (k = 0; k<b; k++){		//k is N side offset within block
				for(l = 0; l<b; l++){		//l is M side offset within block
					output[i*b+k] += input_matrix[(i*b+k)*M + (j*b+l)] * input_vector[j*b+l];
				}
			}
		}
		//at end of last even M block, cover remainder
		for (k = 0; k<b; k++){	
			for(l = M-M%b; l<M; l++){
				output[i*b+k] += input_matrix[(i*b+k)*M + l] * input_vector[l];
			}
		}
	}
	//at end of last even N block, cover remainder
	for (k = N-N%b; k < N; k++){
		for(j = 0; j < M/b; j++){
			for(l = 0; l<b; l++){
				output[k] += input_matrix[k*M + (j*b+l)] * input_vector[j*b+l];
			}
		} 
		//last block, remainder on both dimensions	
		for(l = M-M%b; l<M; l++){
			output[k] += input_matrix[k*M + l] * input_vector[l];
		}
	}
}
