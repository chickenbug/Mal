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
	p = root;
	do{
		if(p->size < size)
			p = p->succ;
		else if(!p->isfree)
			p = p->succ;
		else if(p->size < size + sizeof(mementry)) /*in BKR's notes he says he adds a fudge factor*/
		{
			p->isfree = 0;
			return (char*) p + sizeof(mementry) + size;
		}
		else{
			succ = (mementry*)((char*)p + sizeof(mementry)+size);
			succ -> prev = p;
			succ -> succ = p -> succ;
			if(p->succ != 0)
			{
				p->succ->prev = succ;
			}
			p->succ = succ;
			succ->size = p->size - sizeof(mementry) - size;
			succ->isfree = 1;
			p->size = size;
			p->isfree = 0;
			return ((char*) p + sizeof(mementry));
		}
	} /*should this be a do while because it's def not in the notes*/
}
