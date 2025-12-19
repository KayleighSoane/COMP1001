#include <stdio.h> //this library is needed for printf function
#include <stdlib.h> //this library is needed for rand() function
#include <windows.h> //this library is needed for pause() function
#include <time.h> //this library is needed for clock() function
#include <math.h> //this library is needed for abs()
#include <omp.h> //this library is needed for the timer
#include <xmmintrin.h> // required for __m128
#include <immintrin.h> // required for __m256

unsigned short int equal(float a, float b); //in C, functions must be declared before main()
void init();
void q1();
void q2();
void q3();

void q1_vec();
void checkq1_vec();
void q2_vec();
void checkq2_vec();
void q3_vec();
void checkq3_vec();


#define N 256 //input size
#define EPSILON 1e-6 //relative error margin

float A[N][N], u1[N], u2[N], v1[N], v2[N], x[N], y[N], w[N], z[N];
float alpha = 0.23, beta = 0.45;


int main() {

	double start_1, end_1; //define the timers measuring execution time

	init();//initialize the arrays

	start_1 = omp_get_wtime(); //start the timer 

	//q1();
	//q2();
	//q3();

	q1_vec();
	//q2_vec();
	//q3_vec();


	end_1 = omp_get_wtime(); //end the timer - timer checks that vec path is faster

	printf("Time in seconds is %f\n", end_1 - start_1 );//print the ex.time

	checkq1_vec();
	//checkq2_vec();
	//checkq3_vec(); 

	system("pause"); //this command does not let the output window to close

	return 0; //normally, by returning zero, we mean that the program ended successfully. 
}


void init() {

	int i, j;

	for (i = 0; i < N; i++)
		for (j = 0; j < N; j++) {
			A[i][j] = 0.0f;

		}

	for (i = 0; i < N; i++) {
		u1[i] = (i % 9) * 0.22f;
		u2[i] = (i % 9) * 0.33f;
		v1[i] = (i % 9) * 0.44f;
		v2[i] = (i % 9) * 0.55f;
		w[i] = 0.0f;
	}

}


void q1() {

	for (int i = 0; i < N; i++) {
		w[i] = alpha * u1[i] + beta; // multiplies alpha by u1[i] and adds beta, stores in w[i]
	}								 // does this for each element in w and u
}

void q2() {

	int i, j;

	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			A[i][j] += u1[i] * v1[j] + u2[i] * v2[j];
		}
	}
}

void q3() {

	int i, j;

	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			w[i] += alpha * A[i][j] * u1[j];
		}
	}
}


void q1_vec() { // need to use fmadd instruction, dont need padding since N is multiple of 8
	__m256 num1, num2, num3, num4;
	num1 = _mm256_set1_ps(alpha);
	num2 = _mm256_set1_ps(beta);

	for (int i = 0; i < ((N / 8) * 8); i += 8) {
		num3 = _mm256_loadu_ps(&u1[i]); //load u1 values
		num4 = _mm256_fmadd_ps(num1, num3, num2); // multiply alpha (num1) and u1 (num3), add beta (num2), store in w (num4)
		_mm256_storeu_ps(&w[i], num4); //store result back to w
	}

	for (int i = ((N / 8) * 8); i < N; i++) {
		w[i] = alpha * u1[i] + beta;
		printf("Padding used for w[%d]\n", i);
	}
}

void checkq1_vec() {
	bool correct = true;
	float expected[N] = { 0 };

	for (int i = 0; i < N; i++) {
		expected[i] = alpha * u1[i] + beta;
		if (equal(w[i], expected[i]) != 0) {
			correct = false;
			break;
		}
	}
	if (correct) { printf("Results are correct\n"); }
	else { printf("Results are incorrect\n"); }
}

void q2_vec() {
	// multiply a * b, multiplr c * d, then do ab + cd + A
	__m256 uone, vone, utwo, vtwo, aarr, tone, tsum, finv;

	for (int i = 0; i < N; i++) {
		uone = _mm256_set1_ps(u1[i]); // moved out of inner loop - only have to load once per loop
		utwo = _mm256_set1_ps(u2[i]); // not sure of loadu or set1 is better/faster?

		for (int j = 0; j < ((N / 8) * 8); j += 8) {
			vone = _mm256_loadu_ps(&v1[j]);
			vtwo = _mm256_loadu_ps(&v2[j]);
			aarr = _mm256_loadu_ps(&A[i][j]);

			tone = _mm256_mul_ps(uone, vone); // a * b = ab
			tsum = _mm256_fmadd_ps(utwo, vtwo, tone); // c * d + ab = sum
			finv = _mm256_add_ps(aarr, tsum); // A + sum = final answer
			_mm256_storeu_ps(&A[i][j], finv); // store final answer back into A
		} 

		for (int j = ((N / 8) * 8); j < N; j++) {
			A[i][j] += u1[i] * v1[j] + u2[i] * v2[j];
			printf("Padding used for A[%d][%d]\n", i, j);
		}
	}
}

void checkq2_vec() { 
	bool correct = true;
	float expected[N][N] = { 0 };

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			expected[i][j] += u1[i] * v1[j] + u2[i] * v2[j];
			if (equal(A[i][j], expected[i][j]) != 0) {
				correct = false;
				printf("Index [%d][%d] incorrect\n", i, j);
			}
		}
	}
	if (correct) { printf("Results are correct\n"); }
}

void q3_vec() {
	__m256 zero, alph, aarr, uone, uoar;
	alph = _mm256_set1_ps(alpha);

	for (int i = 0; i < N; i++) {
		zero = _mm256_setzero_ps();

		for (int j = 0; j < ((N / 8) * 8); j += 8) {
			aarr = _mm256_loadu_ps(&A[i][j]);
			uone = _mm256_loadu_ps(&u1[j]);

			uoar = _mm256_mul_ps(alph, uone); // alpha * u1[j]
			zero = _mm256_fmadd_ps(aarr, uoar, zero); // zero = A[i][j] * alpha * u1[j] + zero
		} 
		// need to store all of j (8 values) into singlular w[i]
		// cant work out how to get hadd to work
		zero = _mm256_hadd_ps(zero, zero); // horizontal add 2
		zero = _mm256_hadd_ps(zero, zero); // horizontal add now 4

		//float temp[8];
		//_mm256_storeu_ps(temp, zero);
		//w[i] = temp[0] + temp[1] + temp[2] + temp[3] + temp[4] + temp[5] + temp[6] + temp[7];

		for (int j = ((N / 8) * 8); j < N; j++) {
			float sum = alpha * A[i][j] * u1[j];
			w[i] += sum;
			printf("Padding used for A[%d][%d]\n", i, j);
		}

		// ALL ANSWERS ARE ZERO BECAUSE A[i][j] IS ZERO, WHICH IS WHY CHECKER FAILS
		// NEED TO CHANGE CHECKER TO ACCOUNT FOR ZEROS
	}
}

void checkq3_vec() {
	bool correct = true;
	float expected[N] = { 0 };

	for (int i = 0; i < N; i++) {
        expected[i] = 0.0f;
		for (int j = 0; j < N; j++) {
			expected[i] += alpha * A[i][j] * u1[j];
			if (equal(w[i], expected[i]) != 0) {
				correct = false;
				printf("[%d][%d]- Expected: %f || Answer: %f || Value of A: %f\n", i, j, expected[i], w[i], A[i][j]);
			}
		}
	}
	if (correct) { printf("Results are correct\n"); }
}


//this function becomes problematic when b is zero or when both a and b are zero
// problematic because it divides by b, potential divide by zero error
// if b is too small to divide, just compare it to epsilon
// Epsilon used for error margin of floats
unsigned short int equal(float a, float b) {
	float temp = a - b;

	if (fabs(b) < EPSILON) {
		if (fabs(temp) < EPSILON)
			return 0; //success
	}
	else if (fabs(temp / b) < EPSILON) {
		return 0; //success
	} else {
		return 1; //wrong result
	}
}
