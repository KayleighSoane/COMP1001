#include <stdio.h> //this library is needed for printf() function
#include <windows.h> //this library is needed for pause() function

int array1[5], array2[5], sum[5];
void initialize();
void add();
void print();

#define N 10
int A[N];
void initialize2();
void print2();
int findmax();

int findmin();


void initialize() {
	int i;
	for (i = 0; i < 5; i++) {
		array1[i] = i;
		array2[i] = i + 3;
		sum[i] = 0;
	}
}
void add() {
	int i;
	for (i = 0; i < 5; i++) {
		sum[i] = array1[i] + array2[i];
	}
}
void print() {
	int i;
	for (i = 0; i < 5; i++) {
		printf("%d", array1[i]);
	}
	printf("\n");
	for (i = 0; i < 5; i++) {
		printf("%d", array2[i]);
	}
	printf("\n");
	for (i = 0; i < 5; i++) {
		printf("%d", sum[i]);
	}
}


void initialize2() {
	int i;
	for (i = 0; i < 10; i++) {
		A[i] = rand() % 20;
	}
}
int findmax() {
	int i, max;
	max = A[0];
	for (i = 1; i < 10; i++) {
		if (A[i] > max)
			max = A[i];
	}
	return max;
}
void print2() {
	int i;
	for (i = 0; i < 10; i++) {
		printf("%d ", A[i]);
	}
}

int findmin() {
	int i, min;
	min = A[0];
	for (i = 1; i < 10; i++) {
		if (A[i] < min)
			min = A[i];
	}
	return min;
}


void main() {
	printf("Size of char: %lu\n", sizeof(char));
	printf("Size of short: %lu\n", sizeof(short));
	printf("Size of int: %lu\n", sizeof(int));
	printf("Size of long: %lu\n", sizeof(long));
	printf("Size of long long: %lu\n", sizeof(long long));
	printf("Size of float: %lu\n", sizeof(float));
	printf("Size of double: %lu\n", sizeof(double));
	printf("Size of long double: %lu\n", sizeof(long double));
	system("pause"); //this function prevents from closing the output window
	printf("\n");


	initialize();
	add();
	print();
	printf("\n");
	system("pause");
	printf("\n");


	char name[20];
	printf("Enter your name: "); //get input from keyboard
	scanf_s("%19s", name, sizeof(name));
	printf("Your name is %s\n", name);
	system("pause"); //this command does not let the output window to close
	printf("\n");


	char string[1000];//input string
	char pattern[2]; //pattern string. The first character is the one we need to find in the string array. the second character is always zero as it is the last character of the array
	unsigned int counter = 0;
	printf("Hi - Enter a large string: ");
	scanf_s("%999s", string, sizeof(string)); //read the string from keyboard
	printf("Enter a character to find its frequency: ");
	scanf_s("%s", pattern, sizeof(pattern));
	for (int i = 0; string[i] != '\0'; ++i) {
		if (pattern[0] == string[i])
			counter++;
	}
	printf("Frequency of character %c is %d\n", pattern[0], counter);
	system("pause");
	printf("\n");


	char stringlen[1000];//input string
	printf("Hi - Enter a large string: ");
	scanf_s("%s", stringlen, sizeof(stringlen)); //read the string from keyboard
	printf("Length of String: %ld\n", strlen(stringlen));
	system("pause");
	printf("\n");


	int var1 = 4;
	printf("The memory address of %p contains %d\n", &var1, var1);
	system("pause");
	printf("\n");


	int max;
	initialize2();
	print2();
	max = findmax();
	printf("Max value in the array is: %d\n", max);
	system("pause");
	printf("\n");

	int min;
	min = findmin();
	printf("Min value in the array is: %d\n", min);
	system("pause");
	printf("\n");
}