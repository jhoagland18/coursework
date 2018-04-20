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

static int* search(SET *sp, char *elt);
struct set {
	int count; //num elements
	int length; //max elements
	char **data; //data array
};
/* ***Dummy data and function calls for testing***
int main(int argc, char* argv[]) {
	printf("welcome\n");
	SET * set= createSet(10); //initialize set
	char c[10]="300";
	char* cPointer=c;
	addElement(set, cPointer);
	char d[10]="1";
	char* dPointer=d;
	addElement(set, dPointer);
	char e[10]="267";
	char* ePointer=e;
	addElement(set, ePointer);
	//removeElement(set, ePointer);
	char** strings = getElements(set);
	printf("Set contents: ");
	printElements(set);
	printf("\n");
	char indexToFind[10]="4";
	char* indexpointer=indexToFind;
	printf("Number of elements %d\n",numElements(set));
	int *searchReturn = search(set, indexpointer);
	printf("findelement on 1: %p\n",findElement(set,dPointer));
	printf("Searching for %s.\nFound: %d. Index of %s is at %d\n",searchReturn[0], indexToFind, indexToFind, searchReturn[1]);
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
	sp->data=malloc(sizeof(char*)*maxElts); //initialize
	
	assert(sp->data!=NULL);

	return sp;
}

void addElement(SET *sp, char* elt) {
	assert(sp!=NULL && elt!=NULL);
	
	if(sp->count==sp->length) { //if set is full, return
		return;
	}

	char* temp=strdup(elt); //encapsulate string

	int *searchResult=search(sp, temp); //O(logn)

	if(searchResult[0]==1) { //element already in set, return
		return;
	}
	else if(searchResult[0]==0) //element not in set, nove all greater values up one, insert value.
	{
		if(sp->count==0) //if no elements in set, add element at 0
		{
			sp->data[0]=temp;
			sp->count++;
			return;
		}
		//else continue

		int i;
		for(i=sp->count-1; i>=searchResult[1]; i--) { //start at last element, move element over until reaches target location of elt to add. This is O(n) for worst case
			sp->data[i+1]=sp->data[i];
			}
	
		sp->data[searchResult[1]]=temp;//insert element
		sp->count++;
	}

//	sp->data[sp->count] = temp; //assign array value to encapsulated string literal
//	sp->count++;
}

void removeElement(SET *sp, char* elt) {
	assert(sp!=NULL && elt!=NULL);

	elt=strdup(elt); //encapsulate string
	int *searchResult=search(sp, elt);//check element to remove exists in array. O(logn) worst case
	
	if(searchResult[0]==1) { //if element exists in set

		int i;
		for(i=searchResult[1]+1;i<sp->count; i++) { //shift all elements greater than elt left one index
			sp->data[i-1]=sp->data[i];
		}

		sp->count--;
	}
	
} 

static int* search(SET* sp, char* elt) { //O(logn). Returns integer array of size two. The first index contains a flag, 0 || 1, that indicates if the searched for element is in the array. The second index is either the location of the element, or the location the element belongs in.
	assert(sp!=NULL && elt!=NULL);

	char* temp = strdup(elt); //encapsulation

	static int low,high,mid;

	high=sp->count-1;
	low=0;

	int *result=malloc(2*sizeof(int)); //initialize array to be returned

	if(sp->count==0) { //if array is empty, return
		result[0]=0;
		result[1]=0;
		return result;
	}

	while(low<=high) {
		mid=(low+high)/2;
		if(strcmp(temp,sp->data[mid])==0) { //return if set[mid] equals element searched for
			result[0]=1;
			result[1]=mid;
			return result;
		}
		else if(strcmp(temp, sp->data[mid])>0)
			low=mid+1;
		else
			high=mid-1;
	}
	if(low<high && strcmp(sp->data[low], temp)==0) //checks if element was found or not. Returns appropriate flag
	{
		result[0]=1;
		result[1]=low;
		return result;
	} else {
		result[0]=0;
		result[1]=low;
		return result;
	}
}

char **getElements(SET *sp) {
	assert(sp!=NULL);

	char **result = (char **)malloc(sp->count*sizeof(char)); //initalize char** to be returned

	memcpy(result, sp->data, sizeof(char*)*sp->count); //copy sp->data into char**

	return result; //return data
}

char *findElement(SET *sp, char *elt) {
	assert(sp!=NULL && elt!=NULL);

	int* index = search(sp, elt); //search for element. O(logn) worst case.

	return index[0]==1 ? sp->data[index[1]] : NULL; //ternary; if flag for element presence is not -1, return data[index of element], else return NULL
}

int numElements(SET *sp) {
	assert(sp!=NULL);
	return sp->count;
}

void destroySet(SET *sp) {
	assert(sp!=NULL);

	int i;
	for(i=0; i<sp->count; i++) 
		free(sp->data[i]); //free array indicies
	free(sp->data); //free array
	free(sp); //free struct
}
