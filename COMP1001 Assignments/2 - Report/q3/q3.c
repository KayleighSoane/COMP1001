#include <stdio.h>	/* for printf */
#include <stdint.h>	/* for uint64 definition */
#include <stdlib.h>	/* for exit() definition */
#include <time.h>	/* for clock_gettime */
#include <unistd.h> //for sleep
#include <pthread.h>//for cpu_set_t
#include <omp.h> //for openmp

int N = 0;
int NUM_THREADS = 0;
int run = 0;

//functions declaration
void MMM_init();
int MMM();
void allocateArrays();
void freeArrays();
void checkResults();
void setRun();

float **C, **A, **B; //double pointers for 2D arrays

int main(int argc, char *argv[]){
//argc = number of inputs: ./q3exec, 64, 4 = 3
//argv[] = elements of inputs: argv[1] = 64, argv[2] = 4
//want to assign 64 and 4 from argv[] to N and NUM_THREADS
	
	if (argc !=3){ // argv should have 3 elements. if not, error
		fprintf(stderr, "\n Error - Too many arguments\n");
		return -1;
	}

	N = atoi(argv[1]);
	NUM_THREADS = atoi(argv[2]); //update variables
	if (N == 0 || NUM_THREADS == 0){
		fprintf(stderr, "\n Error - N or NUM_THREADS value not set\n");
		return -1;
	}
	printf("N = %d    THREADS = %d\n", N, NUM_THREADS); //confirms update
	
	allocateArrays(); //allocate memory for arrays 

	double start_1, end_1, time_1 = 0.0;
	setRun();
	printf("Running MMM() for %d loops...\n", run);
	start_1 = omp_get_wtime();
	for (int i = 0; i < run; i++){
		MMM_init(); //have to re-initialize arrays each loop
		MMM(); //run the routine
	}
	end_1 = omp_get_wtime();
	printf("Total loop time: %fs\n", end_1 - start_1);

	checkResults();

	time_1 = (end_1 - start_1) / run; //average time over 5 loops
	double flops = (2.0 * N * N * N) / time_1; // FLOPs = (2 x N^3)/time

	printf("Average execution Time: %fs\n", time_1);
	printf("FLOPs: %f\n", flops);
	// CHECK ACCURACY OF RESULTS BEFORE FINISHING - RUN PROGRAM A FEW MORE TIMES AND COMPARE RESULTS
	freeArrays();
	
	printf("\n");
	return 0;

}

void allocateArrays() {

	C = (float**) malloc(N * sizeof(float*));
	A = (float**) malloc(N * sizeof(float*));
	B = (float**) malloc(N * sizeof(float*)); // initialized first dimension of arrays
	for (int i = 0; i < N; i++) {
		C[i] = (float*) malloc(N * sizeof(float));
		A[i] = (float*) malloc(N * sizeof(float));
		B[i] = (float*) malloc(N * sizeof(float)); // initialized second simension
	}
}

void freeArrays() {

	for (int i = 0; i < N; i++) {
		free(C[i]);
		free(A[i]);
		free(B[i]); // frees second dimension of arrays
	}
	free(C);
	free(A);
	free(B);
}

void checkResults() {

	printf("Checking results...\n");

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			float sum = 0.0f;
			for (int k = 0; k < N; k++) {
				sum += A[i][k] * B[k][j];
			}
			if (C[i][j] != sum) {
				printf("Results are incorrect\n");
			}
		}
	}
	printf("Results are correct\n");
}

void setRun() {
	//threads = 4 - run has to last 30 secs
	//threads = 1 - run has to last 3 secs
	if (NUM_THREADS == 4) {
		if (N == 64) {
			run = 90000;
		} else if (N == 128) {
			run = 10500;
		} else if (N == 256) {
			run = 1500;
		} else if (N == 512) {
			run = 200;
		} else if (N == 1024) {
			run = 25;
		} else if (N == 2048) {
			run = 3;
		}

	} else if (NUM_THREADS == 1) {
		if (N == 64) {
			run = 5000;
		} else if (N == 128) {
			run = 500;
		} else if (N == 256) {
			run = 80;
		} else if (N == 512) {
			run = 10;
		} else if (N == 1024) {
			run = 1;
		} else if (N == 2048) {		
			run = 1;
		}
	}
}

void MMM_init() {

	float e = 0.1234f, p = 0.7264f;

	for (unsigned int i = 0; i < N; i++) { 
		for (unsigned int j = 0; j < N; j++) {
			C[i][j] = 0.0f;
			A[i][j] = ( (j+i) % 99) + e; 
			B[i][j] = ( (j-i) % 99) - p; 
		}
	}
}

int MMM() {

	int i,j,k;

	omp_set_num_threads(NUM_THREADS);//specify the number of threads

	   #pragma omp parallel for private (j,k) //parallelize i loop
		for (i = 0; i < N; i++)
			for (j = 0; j < N; j++)
				for (k = 0; k < N; k++)
					C[i][j] += A[i][k] * B[k][j];

	return 0;
}
