#include "array_addition.h"

float  X1[M2], X2[M2], Y1[M2], test2[M2]; //differently to const_add, we are adding two arrays together and storing it in a new array
										  //where in const_ we added an array with snother number and stored it in a new array - see Add_default() in both files

void initialization_Add() {

	float e = 0.1234f, p = 0.7264f, r = 0.11f;
	unsigned int j;

	for (j = 0; j != M2; j++) {
		Y1[j] = 0.0;
		test2[j] = 0.0;
		X1[j] = (j % 7) + r;
		X2[j] = (j % 13) + e;
	}
}


unsigned short int Add_default() {
	int j;

	for (j = 0; j < M2; j++) {
		Y1[j] = X1[j] + X2[j];
	}


	return 0;
}

unsigned short int Add_SSE() {

	__m128 num1, num2, num3;

	//write your code here
	//need to load from both X1 and X2, add them, and store in Y1 for each array element
	for (int i=0; i < M2; i += 4) {
		num1 = _mm_loadu_ps(&X1[i]); //load X1
		num2 = _mm_loadu_ps(&X2[i]); //load X2
		num3 = _mm_add_ps(num1, num2); //add X1 and X2
		_mm_storeu_ps(&Y1[i], num3); //store result in Y1
	}


	return 0;
}

unsigned short int Add_SSE_anysize() { //code works if M2 is not a multiple of 4
	__m128 num1, num2, num3;

	for (int i = 0; i < (M2/4)*4; i += 4) { //if M2 = 10, 10/4 =2.5 which becomes 2 as an integer,*4=8
		num1 = _mm_loadu_ps(&X1[i]); //load X1
		num2 = _mm_loadu_ps(&X2[i]); //load X2
		num3 = _mm_add_ps(num1, num2); //add X1 and X2
		_mm_storeu_ps(&Y1[i], num3); //store result in Y1
	}

	for (int i = (M2 / 4) * 4; i < M2; i++) { //padding code to handle remaining elements - if M2 is 10 as above, and becomes 8, two elements remain
		Y1[i] = X1[i] + X2[i];
	}
	return 0;
}


unsigned short int Add_AVX() {

	//__m256  ymm0, ymm1, ymm2, ymm3, ymm4;

	//write your code here


	return 0;
}


unsigned short int Compare_Add() {


	for (int j = 0; j < M2; j++) {
		test2[j] = X1[j] + X2[j];
	}


	for (int j = 0; j < M2; j++)
		if (equal(Y1[j], test2[j]) == 1) {
			//printf("\n j=%d\n", j);
			return 1;
		}

	return 0;
}