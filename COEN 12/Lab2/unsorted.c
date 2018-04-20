/*
 * Jackson Hoagland
 * Lab2
 * 10/8/17
 */
#include <stdio.h>
#include "set.h"
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>

static int search(SET *sp, char *elt);
struct set {
	int count; //num elements
	int length; //max elements
	char **data; //data array
};
/*
int main(int argc, char* argv[]) {
	SET * set = createSet(10); //initialize set
	char c[10]="Test";
	char* cPointer=c;
	addElement(set, cPointer);
	char d[10]="Hello";
	char* dPointer=d;
	addElement(set, dPointer);
	char** strings = getElements(set);
	printElements(set);
	printf("Number of elements %d",numElements(set));
	printf("Index of Hello: %d",search(set, dPointer));

	printf("\n");
}
*/
SET * createSet(int maxElts) {
	SET *sp;

	sp = malloc(sizeof(SET)); //reserve memory for set struct
	assert(sp!=NULL);
	sp->count=0; //intialize
	sp->length=maxElts; //initialize
	sp->data=malloc(sizeof(char*)*maxElts); //initialize
	assert(sp->data!=NULL);

	return sp;
}

void addElement(SET *sp, char* elt) {
	assert(sp!=NULL && elt!=NULL);

	if(sp->count==sp->length) { //if set is full
		return;
	}

	elt=strdup(elt); //encapsulate

	if(findElement(sp, elt)!=NULL) { //if element is already in set
		return;
	}

	sp->data[sp->count] = strdup(elt); //assign array value to encapsulated string literal
	sp->count++;
}

void removeElement(SET *sp, char* elt) {
	assert(sp!=NULL && elt!=NULL);

	char *e;
	elt=strdup(elt); //encapsulate string
	e=findElement(sp, elt); //check element to remove exists in array
	if(e!=NULL) { //if element exists
		strcpy(e, sp->data[sp->count-1]); //copy last element into this index to keep array full
		free(sp->data[sp->count-1]); //free data from last element
		sp->count--;
	}
	
} 

static int search(SET* sp, char* elt) { //sequentially searches through set's data. O(n) worst case. Returns -1 if element not found
	assert(sp!=NULL && elt!=NULL);

	char* temp=strdup(elt); //encapsulate element

	int i=0;
	for(i=0; i<sp->count; i++) { //iterate through data -- O(n)
		if(strcmp(sp->data[i], temp)==0) { //if data[i] is equal to searched for string, return
			return i;
		}
	}

	return -1;
}

char **getElements(SET *sp) {
	assert(sp!=NULL);

	char **result = (char **)malloc(sp->count*sizeof(char));

	memcpy(result, sp->data, sizeof(char*)*sp->count);

	return result;
	}

char *findElement(SET *sp, char *elt) {
	assert(sp!=NULL && elt!=NULL);
	
	int index = search(sp, elt); //search for element

	return index!=-1 ? sp->data[index] : NULL; //ternary; if index >=0 return data[i], else return NULL
}

int numElements(SET *sp) {
	assert(sp!=NULL);
	return sp->count;
}

void destroySet(SET *sp) {
	assert(sp!=NULL);

	int i;
	for(i=0; i<sp->count; i++) free(sp->data[i]); //free array indicies
	free(sp->data); //free array
	free(sp); //free struct
}
