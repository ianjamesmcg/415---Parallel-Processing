#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <mpi.h>

#define ROTL32(x,y) ((x<<y)|(x>>(32-y)))
#define ROTR32(x,y) ((x>>y)|(x<<(32-y)))
#define ROTL24(x,y) ((x<<y)|(x>>(24-y)))
#define ROTR24(x,y) ((x>>y)|(x<<(24-y)))
#define ROTL16(x,y) ((x<<y)|(x>>(16-y)))
#define ROTR16(x,y) ((x>>y)|(x<<(16-y)))
#define ROTL8(x,y)  ((x<<y)|(x>>(8-y)))
#define ROTR8(x,y)  ((x>>y)|(x<<(8-y)))

int num_entries = 8;
const char * dictionary [] = {  "bird",
                                "campus",
                                "class",
                                "of",
                                "spring",
                                "sun",
                                "the",
                                "tree" };

// checks if a significant portion of the words resulting from
// the decryption with the tried key are found in the dictionary
int isValid( char *decoded, int len ) {

    int nmatches = 0;
    char * word;

    word = strtok( decoded, " ,.;-()\n\r" );

    while ( word != NULL ) {

        int flag = 0;

        for ( int i = 0; i < num_entries; ++i ) {
            if( !strcmp( word, dictionary[i] ) ) {
                flag = 1;
                break;
            }
        }

        if (flag) {  nmatches += strlen(word);  }

        word = strtok( NULL, " ,.;-()\n\r" );
    }

    // different criteria may be used for deciding whether the tried
    // key was a valid one. here we identify it as valid if words in
    // the decrypted message that can be located in the dictionary account
    // for more than half of the original message length.
    if (nmatches > len * 0.50)
        return 1;

    return 0;
}

void decrypt32( unsigned char *inp, uint32_t key, unsigned char *decoded ) {

    int i, iend, oend;
    uint32_t block;
    uint32_t a, b, c, d, magnitude, polarity, xor;

    srand(key);

    iend = 0;
    decoded[0] = 0; // C strings are zero-terminated
    oend = 0;

    /* main loop for decoding -- all 4 bytes are valid */
    while (     (a = inp[iend++]) != 0
            &&  (b = inp[iend++]) != 0
            &&  (c = inp[iend++]) != 0
            &&  (d = inp[iend++]) != 0
    ) {
        // printf("a = %x, b = %x, c = %x, d=%x\n", a, b, c, d);

        polarity = rand() % 2;
        magnitude = rand() % 32;

        block = (d << 24) | (c << 16) | (b << 8) | a;

        if (polarity)
            block = ROTR32(block, magnitude);
        else
            block = ROTL32(block, magnitude);

        xor =   (rand() % 256 << 24) | (rand() % 256 << 16)
              | (rand() % 256 << 8)  | (rand() % 256);

        block ^= xor;

        decoded[oend++] = block;
        decoded[oend++] = (block = block >> 8);
        decoded[oend++] = (block = block >> 8);
        decoded[oend++] = (block = block >> 8);
        decoded[oend] = 0;

        // printf("p = %d, mag = %d, xor = %d\n", polarity, magnitude, xor);
    }

    /* end cases */
    if ( a != 0 && b != 0 && c != 0 && d == 0 ) {

        polarity = rand() % 2;
        magnitude = rand() % 24;
        block = (c << 16) | (b << 8) | a;

        if (polarity)
            block = ROTR24(block, magnitude);
        else
            block = ROTL24(block, magnitude);

        xor = (rand() % 256 << 16) | (rand() % 256 << 8) | (rand() % 256);

        block ^= xor;

        decoded[oend++] = block;
        decoded[oend++] = (block = block >> 8);
        decoded[oend++] = (block = block >> 8);
        decoded[oend] = 0;

    } else if ( a != 0 && b != 0 && c == 0 ) {

        polarity = rand() % 2;
        magnitude = rand() % 16;
        block = (b << 8) | a;

        if (polarity)
            block = ROTR16(block, magnitude);
        else
            block = ROTL16(block, magnitude);

        xor = (rand() % 256 << 8) | (rand() % 256);

        block ^= xor;

        decoded[oend++] = block;
        decoded[oend++] = (block = block >> 8);
        decoded[oend] = 0;

    } else if ( a != 0 && b == 0 ) {

        polarity = rand() % 2;
        magnitude = rand() % 8;
        block = a;

        if (polarity)
            block = ROTR8(block, magnitude);
        else
            block = ROTL8(block, magnitude);

        xor = rand() % 256;
        block ^= xor;

        decoded[oend++] = block;
        decoded[oend] = 0;
    }
}


int main( int argc,char *argv[] ) {

    char a;
    char outfilename[100];
    unsigned char encrypted[1000], decrypted[1000], dcopy[1000];
    FILE *fin, *fout;
    int success = 0;
    uint32_t i, len;
    double tstart, tend;

    int myrank, procs;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &procs);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank); 
    MPI_Request request; MPI_Status status;
    if(myrank == 0) {
	    printf("\n\nx0r 32-bit code breaker\n\n");

        if (argc == 1) {
			fprintf(stderr, "ERROR: No file(s) supplied.\n");				
			fprintf(stderr, "USAGE: This program requires a filename to be provided as argument!");
			exit(1);
	    }

	    printf("decrypting file %s by trying all possible keys...\n", argv[1]);
	    printf("To quit, press ctrl + c\n\n");
	    printf("Status:\n");

        if ( (fin = fopen(argv[1], "r")) == NULL ) {
	        fprintf(stderr, "ERROR: Could not open: %s\n", argv[1]);
		exit(1);
	    }

	    encrypted[0] = 0;
	    len = 0;
	    while ( (a = fgetc(fin)) != EOF ) {
	        encrypted[len++] = a;
	        encrypted[len] = 0;
	    }
	    fclose(fin);

	    //printf("encrypted: ");
	    // for (i=0; i<end; ++i)
            //  printf("[%d]: %c", i, encrypted[i]);

    	    tstart = MPI_Wtime();
	}
	
    MPI_Bcast(&len, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&encrypted, len, MPI_CHAR, 0, MPI_COMM_WORLD);
	MPI_Irecv(&success, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &request);

	//uint64_t tot =  pow( 2, sizeof(uint32_t)*8 );
	//uint64_t start = tot*(myrank / procs);
	//uint64_t end = tot*((myrank+1) / procs); 
	//if(myrank == procs-1){end = tot;}

	for ( i = myrank ; i < pow( 2, sizeof(uint32_t)*8 ); i+=procs ) {
        decrypt32( encrypted, i, decrypted );
		
        //printf( "i = %d - decrypted: %s\n", i, decrypted );

        strcpy(dcopy, decrypted);

		if(i % 7) {
	    	MPI_Test(&request, &success, &status);
			if(success){
	  			break;
	    	}
		}
        if (isValid(dcopy, len)) {
						
            success = 1;
			MPI_Cancel(&request);
			for(int p=0 ; p < procs ; p++) {
				MPI_Send(&success, 1, MPI_INT, p,11, MPI_COMM_WORLD); 
			}
			sprintf(outfilename, "%s.out", argv[1]);
			fout = fopen(outfilename, "w");
			fprintf(fout, "%s", decrypted);
			printf("\nFile decrypted successfully using key %u\n", (unsigned) i);
			printf("See the file %s\n\n\n", outfilename);
			fclose(fout);

            break;
        }
    }

    if(myrank == 0) {
    	tend = MPI_Wtime();
    	printf("\nTime elapsed: %.6e seconds\n", tend - tstart);
    }

    MPI_Finalize();
    if (success) {
        return 0;
    }

    printf("\nWARNING: File could not be decrypted.\n\n\n");
    return 1;
}
