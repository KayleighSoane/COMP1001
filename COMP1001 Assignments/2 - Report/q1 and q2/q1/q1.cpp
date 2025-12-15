#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <emmintrin.h>
#include <limits.h>
#include <pmmintrin.h>
#include <immintrin.h>


// A -	NEED TO CHANGE LOCATION OF SQUARE GRID TO READ BORDERS OF IMAGES
//		NEED TO MAKE VALUE OF COORDS OUTSIDE OF IMAGE 0 - ZERO PADDING
//		IF COORDS ARE OUT OF BOUNDS, SET VALUE TO 0

// B -	NEED CODE TO ACCESS EVERY FILE - MAKE LOOP FOR A1/A2/A3 ETC
//		NEED M AND N TO HAVE DIMENSIONS OF EACH FILE
//		NEED TO DYNAMICALLY ALLOCATE ARRAYS BASED ON IMAGE DIMENSIONS

// PROGRESS:
// M and N aggined to each images dimensions
// File paths update number for each file and read correctly
// Output image is correct and it is stored correctly
// 
// Problem - a1.pgm is a P2, which causes program to exit
// Problem with files?
// Some files ARE P2 but that shouldn't stop it running
// FIXED!!
// I read the code wrong - i thought a file being P2 was a problem and that all the images should be P5
// I thought the if statement in read_image() that checked for P2 images was an error case
// i put freearrays() above the exit_failure in the EOF if statement of P2, which stopped the whole program running
// Worked again when i removed it.


//function declarations
void edge_detection();
void read_image(char* filename);
void write_image2(char* filename);
void openfile(char* filename, FILE** finput);
int getint(FILE* fp);
void createarrays();
void createfilepaths();
void changefilepaths(int num);
void freearrays();


//CRITICAL POINT: images' paths - You need to change these paths
char* IN = NULL;
char* OUT = NULL; // Declarations

//IMAGE DIMENSIONS
int M = 0; //cols = x
int N = 0; //rows = y


//CRITICAL POINT:these arrays are defined statically. Consider creating these arrays dynamically instead.
unsigned char* input_image = NULL;
unsigned char* output_image = NULL; //Declarations


const signed char GxMask[3][3] = {
	{-1,0,1} ,
	{-2,0,2},
	{-1,0,1}
};

const signed char GyMask[3][3] = {
	{-1,-2,-1} ,
	{0,0,0},
	{1,2,1}
};

void freearrays() {
	if (input_image) {
		free(input_image);
		printf("\n input_image freed");
	}
	if (output_image) {
		free(output_image);
		printf("\n output_image freed");
	}
	if (IN) {
		free(IN);
		printf("\n IN filepath freed");
	}
	if (OUT) {
		free(OUT);
		printf("\n OUT filepath freed");
	}
}

char header[100];
errno_t err;

int main() {

	createfilepaths();

	for (int i = 0; i < 31; i++) {

		changefilepaths(i);

		read_image(IN);//read image from disc

		edge_detection(); //apply edge detection

		write_image2(OUT); //store output image to the disc
		
	}

	freearrays();

	return 0;
}





void edge_detection() {

	int row, col, rowOffset, colOffset;
	int Gx, Gy;

	for (row = 0; row < N; row++) {		//change from row=1 to row=0 to start right in corner. Need to add padding to make out of bounds coords 0.
		for (col = 0; col < M; col++) { // from 0 to M and N are the borders
			Gx = 0;
			Gy = 0;

			for (rowOffset = -1; rowOffset <= 1; rowOffset++) {
				for (colOffset = -1; colOffset <= 1; colOffset++) {
					if ((row + rowOffset < 0) || (row + rowOffset >= N) || (col + colOffset < 0) || (col + colOffset >= M)) { //if coords are outside of 0 and M & N - out of bounds
						Gx = 0; Gy = 0; // Value is 0 if out of bounds
					}
					else{
						Gx += input_image[M * (row + rowOffset) + col + colOffset] * GxMask[rowOffset + 1][colOffset + 1]; 
						Gy += input_image[M * (row + rowOffset) + col + colOffset] * GyMask[rowOffset + 1][colOffset + 1];
					}
				}
			}

			output_image[M * row + col] = (unsigned char)sqrt(Gx * Gx + Gy * Gy);

		}
	}


}




void read_image(char* filename)
{

	int c;
	FILE* finput;
	int i, j, temp;

	printf("\nReading %s image from disk ...", filename);
	finput = NULL;
	openfile(filename, &finput);
	createarrays(); //	moved to here from main because createarrays() needs the info above it, 
	//					and the function needs info from createarrays() for the rest of its execution.

	if ((header[0] == 'P') && (header[1] == '5')) { //if P5 image

		for (j = 0; j < N; j++) {
			for (i = 0; i < M; i++) {

				//if (fscanf_s(finput, "%d", &temp,20) == EOF)
				//	exit(EXIT_FAILURE);
				temp = getc(finput);

				input_image[M * j + i] = (unsigned char)temp;
			}
		}
	}
	else if ((header[0] == 'P') && (header[1] == '2')) { //if P2 image
		for (j = 0; j < N; j++) {
			for (i = 0; i < M; i++) {

				if (fscanf_s(finput, "%d", &temp, 20) == EOF)
					exit(EXIT_FAILURE);

				input_image[M * j + i] = (unsigned char)temp;
			}
		}
	}
	else {
		printf("\nproblem with reading the image");
		freearrays();
		exit(EXIT_FAILURE);
	}

	fclose(finput);
	printf("\nimage successfully read from disc\n");

}



void write_image2(char* filename)
{

	FILE* foutput;
	int i, j;

	printf("  Writing result to disk ...\n");

	if ((err = fopen_s(&foutput, filename, "wb")) != NULL) {
		fprintf(stderr, "Unable to open file %s for writing\n", filename);
		freearrays();
		exit(-1);
	}

	fprintf(foutput, "P2\n");
	fprintf(foutput, "%d %d\n", M, N);
	fprintf(foutput, "%d\n", 255);

	for (j = 0; j < N; ++j) {
		for (i = 0; i < M; ++i) {
			fprintf(foutput, "%3d ", output_image[M * j + i]);
			if (i % 32 == 31) fprintf(foutput, "\n");
		}
		if (M % 32 != 0) fprintf(foutput, "\n");
	}
	fclose(foutput);


}




void openfile(char* filename, FILE** finput)
{
	int x0, y0, x, aa;

	if ((err = fopen_s(finput, filename, "rb")) != NULL) {
		fprintf(stderr, "Unable to open file %s for reading\n", filename);
		freearrays();
		exit(-1);
	}

	aa = fscanf_s(*finput, "%s", header, 20);

	x0 = getint(*finput);//this is M
	y0 = getint(*finput);//this is N
	printf("\t header is %s, while x=%d,y=%d", header, x0, y0);


	//CRITICAL POINT: AT THIS POINT YOU CAN ASSIGN x0,y0 to M,N 
	M = x0;
	N = y0;
	printf("\n Image dimensions are M=%d,N=%d",M,N);
	

	x = getint(*finput); /* read and throw away the range info */
	printf("\n range info is %d",x);

}



//CRITICAL POINT: you can define your routines here that create the arrays dynamically; now, the arrays are defined statically.
void createarrays() {
	input_image = (unsigned char*)malloc(size_t(M) * size_t(N)); //use size_t since M and N are ints
	output_image = (unsigned char*)malloc(size_t(M) * size_t(N));
	if (input_image == NULL || output_image == NULL) {
		fprintf(stderr, "createarrays() failed\n");
		freearrays();
		exit(EXIT_FAILURE);
	}
}

void createfilepaths() {
	//creating dynamic strings with a character that i want to change every loop and replace with an int?
	IN = (char*)malloc(512); 
	OUT = (char*)malloc(512);
	if (IN == NULL || OUT == NULL) {
		fprintf(stderr, "createfilepaths() failed\n");
		exit(EXIT_FAILURE);
	}
}

void changefilepaths(int num) {
	sprintf_s(IN, 512, "C:\\Users\\kayle\\OneDrive - University of Plymouth\\Github Repositories\\COMP1001\\COMP1001 Assignments\\2 - Report\\q1_input\\a%d.pgm", num);
    sprintf_s(OUT, 512, "C:\\Users\\kayle\\OneDrive - University of Plymouth\\Github Repositories\\COMP1001\\COMP1001 Assignments\\2 - Report\\q1_output\\a%d_output.pgm", num);
} // included the file name in the path instead of concatenating them separately



int getint(FILE* fp) /* adapted from "xv" source code */
{
	int c, i, firstchar;//, garbage;

	/* note:  if it sees a '#' character, all characters from there to end of
	   line are appended to the comment string */

	   /* skip forward to start of next number */
	c = getc(fp);
	while (1) {
		/* eat comments */
		if (c == '#') {
			/* if we're at a comment, read to end of line */
			char cmt[256], * sp;

			sp = cmt;  firstchar = 1;
			while (1) {
				c = getc(fp);
				if (firstchar && c == ' ') firstchar = 0;  /* lop off 1 sp after # */
				else {
					if (c == '\n' || c == EOF) break;
					if ((sp - cmt) < 250) *sp++ = c;
				}
			}
			*sp++ = '\n';
			*sp = '\0';
		}

		if (c == EOF) return 0;
		if (c >= '0' && c <= '9') break;   /* we've found what we were looking for */

		/* see if we are getting garbage (non-whitespace) */
	   // if (c!=' ' && c!='\t' && c!='\r' && c!='\n' && c!=',')
		//	garbage=1;

		c = getc(fp);
	}

	/* we're at the start of a number, continue until we hit a non-number */
	i = 0;
	while (1) {
		i = (i * 10) + (c - '0');
		c = getc(fp);
		if (c == EOF) return i;
		if (c < '0' || c>'9') break;
	}
	return i;
}