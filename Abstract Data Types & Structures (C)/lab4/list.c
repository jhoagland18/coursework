/* created by Jackson Hoagland on 10/26/2017
 * Lab 4
 */

#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef struct node NODE;


struct node {
	void * data;
	NODE* next;
	NODE* prev;
};

struct list {
	int count;
	NODE * head;
	int (*compare)();
};

extern LIST *createList(int (*compare)()) { //Creates list struct. O(1)
	LIST *lp;
	lp=malloc(sizeof(LIST));
	assert(lp!=NULL);

	lp->count=0;
	lp->compare=compare;
	lp->head=malloc(sizeof(NODE));
	assert(lp->head!=NULL);

	lp->head->next=lp->head;
	lp->head->prev=lp->head;

	return lp;
}

NODE *createNode(void* newData, NODE* newNext, NODE* newPrev) { //creates node taking next and prev arguments. O(1)
	NODE *np;
	np=malloc(sizeof(NODE));
	assert(np!=NULL);

	np->data=newData;
	np->next=newNext;
	np->prev=newPrev;

	return np;
}

extern void destroyList(LIST *pList) { //destroys list. O(n)
	assert(pList!=NULL);
	NODE *pDel;
	NODE *pPrev=pList->head->prev;

	do {
		pDel=pPrev;
		pPrev=pDel->prev;
		free(pDel);
	} while(pDel!=pList->head);
	
	free(pList);
}

extern int numItems(LIST *lp) { //O(1)
	assert(lp!=NULL);
	return lp->count;
}

extern void addFirst(LIST *lp, void *item) { //adds node to beginning of list. O(1)
	assert(lp!=NULL && item!=NULL);
	NODE *pNew=createNode(item, lp->head->next, lp->head);
	lp->head->next->prev=pNew;
	lp->head->next=pNew;
	lp->count++;
}

extern void addLast(LIST *lp, void *item) { //adds node to end of list circularly. O(1)
	assert((lp!=NULL) && (item!=NULL));
	NODE *pNew=createNode(item, lp->head, lp->head->prev);
	lp->head->prev->next=pNew;
	lp->head->prev=pNew;
	lp->count++;
}

extern void *removeFirst(LIST *lp) { //removes first element. O(1)
	assert(lp!=NULL);
	NODE *pDel=lp->head->next;
	
	pDel->next->prev=lp->head;

	void* returnValue=pDel->data;

	lp->head->next=pDel->next;
		
	free(pDel);
	lp->count--;
	return returnValue;
}

extern void *removeLast(LIST *lp) { //removes last element circularly. O(1)
	assert(lp!=NULL);
	NODE *pDel=lp->head->prev;

	pDel->prev->next=lp->head;

	void* returnValue=pDel->data;

	lp->head->prev=pDel->prev;

	free(pDel);
	lp->count--;
	return returnValue;
}

extern void *getFirst(LIST *lp) { //gets data at first node. O(1)
	assert(lp!=NULL);
	
	return lp->head->next->data;
}

extern void *getLast(LIST *lp) { //gets data at last node circularly. O(1)
	assert(lp!=NULL);
	
	return lp->head->prev->data;

}

extern void removeItem(LIST *lp, void *item) { //searches for and removes node. O(n)
	assert((lp!=NULL) && (item!=NULL));
	
	NODE *currentNode=lp->head->next;

	while(currentNode!=lp->head) {
		if((*lp->compare)(currentNode->data, item)==0) {
			currentNode->next->prev=currentNode->prev;
			currentNode->prev->next=currentNode->next;
			free(currentNode);
			lp->count--;
			break;
		}
	currentNode=currentNode->next;
	}
}

extern void *findItem(LIST *lp, void *item) { //find data in list. O(n)
	assert(lp!=NULL && item!=NULL);
	
	//printf("finding item\n");

	NODE *currentNode = lp->head->next;

	while(currentNode!=lp->head) {
		if((*lp->compare)(currentNode->data, item)==0) {
			return currentNode->data;	
		}
		currentNode=currentNode->next;
	}
	return NULL;
}

extern void *getItems(LIST *lp) { //returns items in list. O(n)
	assert(lp!=NULL);
	void** returnList = malloc(lp->count*sizeof(void*));
	
	NODE* currentNode = lp->head->next;

	int iterator=0;

	while(currentNode!=lp->head) {
	returnList[iterator++]=currentNode->data;	
	currentNode=currentNode->next;
	}

	return returnList;
}
