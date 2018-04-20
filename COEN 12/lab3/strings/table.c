/*
 * Jackson Hoagland
 * Lab2
 * 10/8/17
 *
 */
#include <stdio.h>
#include "set.h"
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>

#define EMPTY 0
#define FILLED 1
#define DELETED 2

static int* search(SET *sp, char *elt);
static void printElements(SET *sp);
static unsigned strhash(char* s);
struct set {
	int count; //num elements
	int length; //max elements
	char **data; //data array
	char *flags; //flag array
};
/* ***Dummy data and function calls for testing***
int main(int argc, char* argv[]) {
	printf("welcome\n");
	SET * set= createSet(10); //initialize set
	char c[10]="3";
	char* cPointer=c;
	addElement(set, cPointer);
	char d[10]="1";
	char* dPointer=d;
	addElement(set, dPointer);
	char e[10]="2";
	char* ePointer=e;
	addElement(set, ePointer);
	//removeElement(set, ePointer);
	char** strings = getElements(set);
	printElements(set);
	printf("\n");
	char indexToFind[10]="1";
	char* indexpointer=dPointer;
	printf("Number of elements %d\n",numElements(set));
	int *searchReturn = search(set, indexpointer);
	printf("\n\nSearching for %s.\nFound: %d. Index of %s is at %d\n\n",indexpointer, searchReturn[0], indexToFind, searchReturn[1]);
	printf("Removing 1\n");
	removeElement(set, dPointer);
	printElements(set);
	printf("findelement on 1: %p\n",findElement(set, dPointer));
	printf("\n");
}*/

SET * createSet(int maxElts) {
	SET *sp;

	sp = malloc(sizeof(SET)); //reserve memory for set struct
	assert(sp!=NULL);

	sp->count=0; //intialize
	sp->length=maxElts; //initialize
	sp->data=malloc(sizeof(char*)*maxElts); //initialize data array
	sp->flags=malloc(sizeof(int)*maxElts); //initalize flags
//	memset(sp->flags,EMPTY,sizeof(int)*maxElts);
	int i;
	for (i=0; i<maxElts-1; i++) {
		sp->flags[i]=EMPTY;
		}
	
	assert(sp->data!=NULL);
	assert(sp->flags!=NULL);
	return sp;
}

void addElement(SET *sp, char* elt) { //O(1) best case, O(m) worst case
	assert(sp!=NULL && elt!=NULL);

	elt=strdup(elt);
	int* pos=search(sp, elt);

	if(pos[0]==1) //value is in array
	{
		return;

	}
	sp->data[pos[1]]=elt;
	sp->flags[pos[1]]=FILLED;
	sp->count++;
}

void removeElement(SET *sp, char* elt) { //O(1) best case, O(m) worst case
	assert(sp!=NULL && elt!=NULL);
	
	elt=strdup(elt);

	int* pos=search(sp, elt);

	if(pos[0]==0) {
		return;
	}
	
	sp->flags[pos[1]]=DELETED;
	sp->count--;
} 

static int* search(SET* sp, char* elt) { //O(1) ideal, O(m) worst case. Returns integer array of size two. The first index contains a flag, 0 || 1, that indicates if the searched for element is in the array. The second index is either the location of the element, or the location the element belongs in.
	assert(sp!=NULL && elt!=NULL);
	
	int *returnValue=malloc(sizeof(int)*2);
	
	returnValue[0]=0;
	returnValue[1]=-1;

	elt=strdup(elt);
	
	unsigned hash = strhash(elt)%sp->length;
	unsigned iterator=hash;

	while( sp->flags[iterator]!=EMPTY ) {
		iterator=iterator%sp->length;

		
		if((sp->flags[iterator]==DELETED) && (returnValue[1]==-1)) //if deleted, store index to return as insertion position
			returnValue[1]=(int) iterator;

		if((sp->flags[iterator]==FILLED) && (strcmp(sp->data[iterator],elt))==0) {//if filled, compare to see if equals element
			returnValue[0]=1;
			returnValue[1]=(int)iterator;
			return returnValue;
		}
		if(iterator==(hash-1)%sp->length) //terminate loop if iterated through all values
			break;
		iterator++;
	}

	
	if(returnValue[1]==-1)
		returnValue[1]=(int)iterator;
	return returnValue;
}

unsigned strhash(char *s) { //O(1)
	assert(s!=NULL);
	s=strdup(s); //encapsulate

	unsigned hash=0;

	while(*s != '\0') //iterate through and add characters to hash output
		hash = 31 * hash + *s ++;

	return hash;
}

char **getElements(SET *sp) {//O(m)
	assert(sp!=NULL);
	
	char **result = (char **)malloc(sp->count*sizeof(char*)); //initalize char** to be returned

	assert(result!=NULL);
	
	int iterator=0;

	int i;
	for(i=0; i<sp->length; i++) {
		if(sp->flags[i]==FILLED) {
			result[iterator++]=sp->data[i];
		}
	}

	return result; //return data
}

char *findElement(SET *sp, char *elt) { //O(1) ideal, O(m) worst case
	assert(sp!=NULL && elt!=NULL);
	elt=strdup(elt);
	int* index = search(sp, elt); //search for element. O(m) worst case.


	return index[0]==1 ? sp->data[index[1]] : NULL; //ternary; if flag for element presence is not -1, return data[index of element], else return NULL
}

int numElements(SET *sp) { //O(1)
	assert(sp!=NULL);
	return sp->count;
}

void destroySet(SET *sp) { //O(m)
	assert(sp!=NULL);
	int i;
	for(i=0; i<sp->length; i++) 
		free(sp->data[i]); //free array indicies
	free(sp->flags);
	free(sp->data); //free array
	free(sp); //free structi
}

void printElements(SET *sp) { //internal helper method
	int i;
	printf("Printing elements of set:\n\n");
	for(i=0; i<10; i++) {
		printf("%i",sp->flags[i]);
	}
	printf("\n");
	for(i=0; i<10; i++) {
		if(sp->flags[i]==FILLED)
			printf("%s",sp->data[i]);
		else
			printf("_");
	}
	printf("\n\n");
}
