#include<stdio.h> 
#include <windows.h> //this library is needed for pause() function

#define N 2

struct student //this is how a struct is declared. Structure members cannot be initialized here (when a datatype is declared, no memory is allocated for it. Memory is allocated only when variables are created.). 
{
	char student_name[50];
	int module_code;
	float mark;
};

int main()
{
	int i;
	struct student arr_student[N];

	for (i = 0; i < N; i++) {
		printf("Student Record System\n");
		printf("---------------------\n\n");
		printf("Student %d name: ", i + 1);
		scanf_s("%s", arr_student[i].student_name, sizeof(arr_student[i].student_name));
		printf("Module code: ");
		scanf_s("%d", &arr_student[i].module_code, sizeof(arr_student[i].module_code));
		printf("Mark: ");
		scanf_s("%f", &arr_student[i].mark, sizeof(arr_student[i].mark));
	}

	int n = 0;
	for (i = 0; i < N; i++) {
		printf("\nEnter Student 1 or 2: ");
		scanf_s("%d", &n, sizeof(n));
		if (n == 1) {
			printf("%s\t%d\t%.2f\n", arr_student[0].student_name, arr_student[0].module_code, arr_student[0].mark);
		}
		else if (n == 2) {
			printf("%s\t%d\t%.2f\n", arr_student[1].student_name, arr_student[1].module_code, arr_student[1].mark);
		}

		system("pause");
		return 0;
	}
}
