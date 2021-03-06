/*
 * created by Jackson Hoagland on 11/16/2017
 * Lab 5
 */

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "pack.h"
#include "pqueue.h"

typedef struct node NODE;


int counts[257];
NODE* leaves[257];

static int nodeCompare(NODE* n1, NODE* n2);
void setParent(NODE* current, NODE* parent);
NODE* createNode(NODE* parent, int count);
int depth(NODE* np);

int main(int argc, const char * argv[]) {
	assert(argc==3); //ensure valid args

	PQ* pq = createQueue(nodeCompare);
	int i;
	for(i=0; i<257; i++) { //initialize values in counts and leaves arrays.
		counts[i]=0;
		leaves[i]=NULL;
	}
	
	counts[256]='\0'; //EOF
	leaves[256]=createNode(NULL, 0);//EOF

	FILE *file;

	file = fopen(argv[1], "rb"); //open file

	int c;

	while((c=getc(file)) != EOF) //reads in characters to counts
		counts[c]++;
	
	fclose(file);
	
	addEntry(pq, leaves[256]); //O(logn)

	//int i;
	for(i=0; i<257; i++) { //creates nodes and inserts them in to priority queue
		if(counts[i]>0) {
			leaves[i]=createNode(NULL, counts[i]);
			//printf("counts[i] is %d - dec. %d - oct. %o - char.  %c\n",counts[i],i,i,i);
			addEntry(pq, leaves[i]); //O(logn)
		}
	}
	
	NODE *left, *right;
	
	while(numEntries(pq)>1) { //build trees
		left=removeEntry(pq);
		right=removeEntry(pq);

		NODE* parent=createNode(NULL,left->count + right->count);
		setParent(left, parent);
		setParent(right, parent);
		
		addEntry(pq, parent);
	}

	int numOccurrances;
	int nodeDepth;
	int totalBits;
	for(i=0; i<257; i++) { //creates output for total bits
		if(leaves[i]!=NULL) {
			numOccurrances=leaves[i]->count;
			nodeDepth=depth(leaves[i]);
			totalBits=numOccurrances*nodeDepth;

			if(i>31 && i<256) //if it works...
				printf("\'%c\': %d x %d bits = %d bits\n",i,numOccurrances,nodeDepth, totalBits);
			else //don't change it
				printf(" %o: %d x %d bits = %d bits\n",i,numOccurrances,nodeDepth,totalBits);	
		}
	}
	pack(argv[1], argv[2], leaves);
	return 0;
}

NODE* createNode(NODE* parent, int count) { //creates node 
	struct node* pNode = malloc(sizeof(NODE));
	pNode->parent=parent;
	pNode->count=count;
	return pNode;
}

void setParent(NODE* current, NODE* parent) { //sets node parent
	assert(current!=NULL && parent!=NULL);
	current->parent=parent;
}

int depth(NODE* np) { //gets depth of node. O(logn)
	assert(np!=NULL);

	int depth=0;
	while(np->parent!=NULL) {
		np=np->parent;
		depth++;
	}
	return depth;
}

static int nodeCompare(NODE* n1, NODE* n2) { //compares nodes.
	assert(n1!=NULL && n2!=NULL);

	if(n1->count==n2->count)
		return 0;
	else
		return n1->count>n2->count ? 1 : -1;
}
