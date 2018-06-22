#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	char buffer[10]; //buffer size

	if(argc != 3) { //verify args
		printf("Incorrect arg count. Must have only input and output files.\n");
		exit(EXIT_FAILURE);
	}

	FILE *fpsource = fopen(argv[1], "r"); //open source file
	FILE *fpdest = fopen(argv[2], "w+"); //open dest file
	size_t data;

	while((data=fread(buffer, 1, sizeof(buffer), fpsource))>0) { //iterate over input file
		fwrite(buffer,1,data,fpdest); //write to output file
	}

	return 0;
}
