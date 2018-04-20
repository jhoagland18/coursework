#include <stdio.h>
#define MAX_WORD_LENGTH 30

int main (int argc, char* argv[]) 
{
	if(argv[2]!=NULL) { //checks if user has defined target file
		printf("Insufficient arguments\n");
	} else { //valid target file
		FILE *f = fopen(argv[1],"r"); //open file stream
		int numWords=0;
		char x[MAX_WORD_LENGTH];
		while(fscanf(f, "%s", x)==1) { //iterate through strings returned by fscanf while a string is returned 
			numWords++;
		}
		fclose(f); //close file stream
		printf("%d words read\n", numWords); //print output
	}
	return 0;
}
