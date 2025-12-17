// Wk7 Lab 1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <Windows.h>
#include "array_addition.h"
#include <omp.h> //this library is needed for the timer

#define TIMES_TO_RUN 1000 //how many times the function will run


int main() {

	unsigned short int output;
	int t;

	//define the timers measuring execution time
	double start_1, end_1;

	//the following command pins the current process to the 1st core
	//otherwise, the OS tongles this process between different cores
	BOOL success = SetProcessAffinityMask(GetCurrentProcess(), 1);
	if (success == 0) {
		//cout << "SetProcessAffinityMask failed" << endl;
		printf("\nSetProcessAffinityMask failed\n");
		system("pause");
		return -1;
	}

	//initialize the arrays
	initialization_Add();

	start_1 = omp_get_wtime(); //start the timer 

	//auto start = std::chrono::high_resolution_clock::now(); //ACCURATE timer provided in C++ only

	//run this several times because this routine runs very fast 
	//The execution time needs to be at least some seconds in order to have a good measurement (why?) 
	//			because other processes run at the same time too, preempting our thread
	//---Appropriately MODIFY the 'TIMES_TO_RUN' and the input size (defined in the appropriate header file)---
	for (t = 0; t < TIMES_TO_RUN; t++) {

		//output = Add_default();
		//output = Add_SSE();
			output = Add_SSE_anysize();
		//output = ConstAdd_AVX();


	}



	//auto finish = std::chrono::high_resolution_clock::now(); 
	end_1 = omp_get_wtime(); //start the timer 


	if (Compare_Add() == 0)
		printf("\n\n\r ----- output is correct -----\n\r");
	else
		printf("\n\n\r -----output is INcorrect -----\n\r");

	printf(" Time in seconds is %f\n", end_1 - start_1);//print the ex.time
	//std::chrono::duration<double> elapsed = finish - start;
	//std::cout << "Elapsed time: " << elapsed.count() << " s\n";

	system("pause"); //this command does not let the output window to close
	return 0;
}
