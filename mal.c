#include <stdio.h>
#include <stdlib.h>

typedef struct mementry{
	struct mementry *prev, *succ;
	int isfree;
	unsigned int size;
} mementry;

# define BLOCKSIZE 1<<20;

static char big_block[BLOCKSIZE];
static int initialized = 0;
static mementry *root;

void* myfree(void* p1){
	mementry *p,*succ;
	if(!initialized){
		root = (mementry*)big_block;
		root->prev = root->succ = 0;
		root->size = BLOCKSIZE- sizeof(mementry);
		root->isfree = 1;
		initialized = 1;
	}
}