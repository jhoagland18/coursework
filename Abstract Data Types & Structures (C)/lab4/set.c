#include "set.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "list.h"

#define EMPTY 0
#define FILLED 1
#define DELETED 

struct set {
	int length;
	int count;
	LIST** data;
	int (*compare)();
	unsigned (*hash)();
};

SET *createSet(int maxElts, int (*compare)(), unsigned (*hash)()) { //O(m) to inistialize data

	SET *sp;
	

	sp=malloc(sizeof(SET));
	assert(sp!=NULL);

	sp->count=0;
	sp->length=maxElts/20;

	sp->compare=compare;

	sp->hash=hash;

	sp->data=malloc(sp->length*sizeof(LIST*));

	int i;
	for(i=0; i<sp->length; i++) {
		sp->data[i]=createList((*compare));	
	}
	
	return sp;
}

void destroySet(SET *sp) { //O(m)
	assert(sp!=NULL);
	int i;
	for(i=0; i<sp->length; i++) { //clear dynamically allocated array
		destroyList(sp->data[i]);
	}

	free(sp->data);
	free(sp);
}

int numElements(SET *sp) { //O(1)
	assert(sp!=NULL);
	return sp->count;
}

void addElement(SET *sp, void *elt) { //O(1) optimal, O(n) worst case
	assert(sp!=NULL && elt!=NULL);

	unsigned hash = (*sp->hash)(elt)%sp->length;

	if(findItem(sp->data[hash],elt)==NULL) {
		addFirst(sp->data[hash], elt);
		sp->count++;
	}
}

void removeElement(SET *sp, void *elt) { //O(1) optimal case, O(n) worst case
	assert(sp!=NULL && elt!=NULL);

	unsigned hash = (*sp->hash)(elt)%sp->length;
	
	if(findItem(sp->data[hash],elt)!=NULL) {
		removeItem(sp->data[hash], elt);
		sp->count--;
	}
}

void *findElement(SET *sp, void *elt) { //O(1) optimal case, O(n) worst case
	assert(sp!=NULL && elt!=NULL);

	unsigned hash = (*sp->hash)(elt)%sp->length;
	
	return findItem(sp->data[hash], elt);
}

void *getElements(SET *sp) { //O(n)
	assert(sp!=NULL);

	void** toReturn=malloc(sizeof(void*)*sp->count);

	int iterator=0;

	int i;
	for(i=0; i<sp->length; i++) {
		void** listItems = getItems(sp->data[i]);
		int j;
		for(j=0; j<numItems(sp->data[i]); j++) {
			toReturn[iterator++]=listItems[j];
		}
	}
	return toReturn;
}
