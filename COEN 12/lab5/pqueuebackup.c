/*
 * jackson hoagland
 * lab5
 * 11/9/17
 */

#include "pqueue.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

void reheap(PQ* pq);
void deheap(PQ* pq);
void expandQueue(PQ* pq);
int getParent(int i);
int getLeft(int i);
int getRight(int i);


struct pqueue {
	int count;
	int length;
	void** data;
	int (*compare)();
};

PQ *createQueue(int (*compare)()) {
	PQ* pqueue;
	pqueue=malloc(sizeof(PQ));
	pqueue->data=malloc(10*sizeof(void*));
	
	assert(pqueue!=NULL);
	pqueue->length=10;
	pqueue->count=0;
	pqueue->compare=compare;

	return pqueue;
}

void destroyQueue(PQ *pq) {
	assert(pq!=NULL);
	free(pq->data);
	
}

int numEntries(PQ *pq) {
	assert(pq!=NULL);
	return pq->count;
}

void addEntry(PQ *pq, void *entry) {
	assert(pq!=NULL && entry!=NULL);

	if(pq->count == pq->length) {
//		printf("expanding queue");
		expandQueue(pq);
		
	}

	pq->data[pq->count]=entry;
	//pq->count++;
	reheap(pq);

	pq->count++;
}

void *removeEntry(PQ *pq) {
	assert(pq!=NULL);
	
	void* toReturn=pq->data[0];
	/*int i;
	for(i=0; i<pq->count-1; i++) {
		pq->data[i]=pq->data[i+1];
	}*/
	deheap(pq);
	
	return toReturn;
}

void expandQueue(PQ* pq) {
	int newLength = pq->length*2;

	pq->data=(void**) realloc(pq->data, newLength * sizeof(void*));
	pq->length=newLength;
}

void reheap(PQ* pq) {
	int index = pq->count;
	int parent;
	void* temp;;

	while(index>=0) {
		parent=getParent(index);
	   if((*pq->compare)(pq->data[index], pq->data[parent])<0) {

		   	temp=pq->data[parent];

			pq->data[parent]=pq->data[index];
			pq->data[index]=temp;

			index=parent;	
	   } else {
			break;
	   }
	}
}

void deheap(PQ* pq) {
	int index=0;
	int leftChild, rightChild;
	int smallerChild=-1;
	
	pq->data[index]=pq->data[pq->count-1];
	pq->count--;

	while(index<pq->count) {
		leftChild=getLeft(index);
		rightChild=getRight(index);

		if(rightChild>=pq->count) {
			if(leftChild>=pq->count) {
				return;
			} else {
				smallerChild=leftChild;
			}
		} else { 
			if((*pq->compare)(pq->data[leftChild], pq->data[rightChild])<=0) {
				smallerChild=leftChild;
			} else {
				smallerChild=rightChild;
			}
		}

		if((*pq->compare)(pq->data[index],pq->data[smallerChild])>0) {
			void* temp = pq->data[smallerChild];
			pq->data[smallerChild]=pq->data[index];
			pq->data[index]=temp;
			index=smallerChild;
		} else {
			break;
		}
	}
}

int getParent(int i) {
	return (i-1)/2;
}

int getLeft(int i) {
	return (2*i+1);
}

int getRight(int i) {
	return (2*i+2);
}
