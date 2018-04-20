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

static int* search(SET *sp, void *elt);
static void printElements(SET *sp);
struct set {
	int count; //num elements
	int length; //max elements
	char **data; //data array
	char *flags; //flag array
	int (*compare)();
	unsigned (*hash)();
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

SET * createSet(int maxElts, int (*compare)(), unsigned (*hash)()) {
	SET *sp;

	sp = malloc(sizeof(SET)); //reserve memory for set struct
	assert(sp!=NULL);

	sp->count=0; //intialize
	sp->length=maxElts; //initialize
	sp->data=malloc(sizeof(char*)*maxElts); //initialize data array
	sp->flags=malloc(sizeof(int)*maxElts); //initalize flags
//	memset(sp->flags,EMPTY,sizeof(int)*maxElts);
	sp->compare=compare;
	sp->hash=hash;
	int i;
	for (i=0; i<maxElts-1; i++) { //o(m)
		sp->flags[i]=EMPTY;
		}
	
	assert(sp->data!=NULL);
	assert(sp->flags!=NULL);
	return sp;
}

void addElement(SET *sp, void* elt) { //O(1) ideal case, O(m) wost
	assert(sp!=NULL && elt!=NULL);

	int* pos=search(sp, elt);

	if(pos[0]==1) //value is in array
	{
		return;

	}
	sp->data[pos[1]]=elt;
	sp->flags[pos[1]]=FILLED;
	sp->count++;
}

void removeElement(SET *sp, void* elt) { //O(1) ideal, O(m) worst
	assert(sp!=NULL && elt!=NULL);

	int* pos=search(sp, elt);
	if(pos[0]==0) {
		return;
	}
	
	sp->flags[pos[1]]=DELETED;
	sp->count--;
} 

static int* search(SET* sp, void* elt) { //O(1) ideal case, O(m) worst case. Returns integer array of size two. The first index contains a flag, 0 || 1, that indicates if the searched for element is in the array. The second index is either the location of the element, or the location the element belongs in.
	assert(sp!=NULL && elt!=NULL);
	
	int *returnValue=malloc(sizeof(int)*2);
	
	returnValue[0]=0;
	returnValue[1]=-1;
	
	unsigned hash = sp->hash(elt)%sp->length;
	unsigned iterator=hash;

	while( sp->flags[iterator]!=EMPTY ) {
		iterator=iterator%sp->length;

		
		if((sp->flags[iterator]==DELETED) && (returnValue[1]==-1)) //if deleted store the location to insert
			returnValue[1]=(int) iterator;

		if((sp->flags[iterator]==FILLED) && (sp->compare(sp->data[iterator],elt))==0) { //if filled, compare to see if value present
			returnValue[0]=1;
			returnValue[1]=(int)iterator;
			return returnValue;
		}
		if(iterator==(hash-1)%sp->length) //if have cycled through all elements, break
			break;
		iterator++;
	}

	
	if(returnValue[1]==-1)
		returnValue[1]=(int)iterator;
	return returnValue;
}

void *getElements(SET *sp) { //O(m)
	assert(sp!=NULL);
	
	void **result = malloc(sp->count*sizeof(void*)); //initalize char** to be returned
	
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

void *findElement(SET *sp, void *elt) { //O(1) best case, O(m) worst case
	assert(sp!=NULL && elt!=NULL);

	int* index = search(sp, elt); //search for element. O(m) worst case.


	return index[0]==1 ? sp->data[index[1]] : NULL; //ternary; if flag for element presence is not -1, return data[index of element], else return NULL
}

int numElements(SET *sp) {
	assert(sp!=NULL);
	return sp->count;
}

void destroySet(SET *sp) { //O(m)
	assert(sp!=NULL);
	int i;
//	printf("deleting set\n");
	for(i=0; i<sp->length; i++) 
		free(sp->data[i]); //free array indiciesi
	free(sp->data); //free array
	free(sp); //free structi
}

void printElements(SET *sp) { //private helper method
	int i;
	printf("Printing elements of set:\n\n");
	for(i=0; i<10; i++) {
		printf("%i",sp->flags[i]);
	
	}
}
