#include <stdio.h>
#include <stdlib.h>

/*standard memory entry in our program*/
typedef struct mementry{
	struct mementry *prev, *succ;
	int isfree;
	unsigned int size;
} mementry;

#define BLOCKSIZE 1<<20;

static char big_block[BLOCKSIZE];

/*our implementation of malloc*/
void* mymalloc(unsigned int size){
	static int initialized_front = 0;
	static int initialized_back = 0;
	static mementry *root, *tail;
	mementry *p,*succ, *small_check, *big_check;

	if(size > 4096) { //Large block cutof at page size
		//TODO ASHNI does big logic

		/*if malloc has not yet been called*/
		if(!initialized){
			root = (mementry*)big_block;
			root->prev = root->succ = 0;
			root->size = BLOCKSIZE- sizeof(mementry);
			root->isfree = 1;
			initialized = 1;
		}
		
		/*start the pointer at the root*/
		p = root;
		do{
			/*iterate through*/
			if(p->size < size)
				p = p->succ;
			else if(!p->isfree)
				p = p->succ;
			
			/* if the chunk is too small to split*/
			else if(p->size < size + sizeof(mementry) + 8) /*fudge factor of 8*/
			{
				p->isfree = 0;
				return (char*) p + sizeof(mementry) + size;
			}
			/*if the chunk is large enough to split*/
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
		} while(p != 0);
		printf("ERROR: Memory saturation. Malloc failed at file: %s and line %d", __FILE__, __LINE__);
		return;

	}
	else{ //TODO HAI-KINH does small logic
		big_check = (mementry*)big_block[BLOCKSIZE-1];
		if(initialized_back) 
			big_check--;
			while(big_check->succ){
				big_check = big_check->succ;
			}
		}

		/*if malloc has not yet been called*/
		if(!initialized){
			root = (mementry*)big_block;
			root->prev = root->succ = 0;
			root->size = BLOCKSIZE- sizeof(mementry);
			root->isfree = 1;
			initialized = 1;
		}
		
		/*start the pointer at the root*/
		p = root;
		do{
			/*iterate through*/
			if(p->size < size)
				p = p->succ;
			else if(!p->isfree)
				p = p->succ;
			
			/* if the chunk is too small to split*/
			/*NOTE do not forget to check size and update appropriately*/
			else if(p->size < size + sizeof(mementry) + 8) /*fudge factor of 8*/
			{
				p->isfree = 0;
				return (char*) p + sizeof(mementry) + size;
			}
			/*if the chunk is large enough to split*/
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
		} while(p != 0);
		printf("ERROR: Memory saturation. Malloc failed at file: %s and line %d", __FILE__, __LINE__);
		return;

	}
}

/*our implementation of free*/
void myfree(void * p1){
	int free_fail = 1 ;
	mementry *ptr, *pred, *succ, *check;
	ptr = (mementry*) p1 - 1;
	check = (mementry*)big_block;

	while(check->succ != 0 && free_fail != 0){
		if(check == ptr){
			if(ptr->isfree){
				printf("ERROR: Redundant free at file: %s and line %d", __FILE__, __LINE__);
				return;
			} 
			free_fail=0;
		} 
		check = check->succ; 
	}

	if(free_fail){
		printf("ERROR: Unallocated pointer free at file: %s and line %d", __FILE__, __LINE__);
		return;
	} 

	if(((pred = ptr->prev) != 0) && (pred->isfree))
	{
		pred->size += sizeof(mementry) + ptr->size;
		pred->succ = ptr -> succ;
		if(ptr->succ != 0)
		{
			ptr->succ->prev = pred;
		}
	}
	else{
		ptr->isfree;
		pred = ptr;
	}
	
	if(((succ = ptr->succ) != 0) && (succ->isfree))
	{
		pred->size += sizeof(mementry) + succ->size;
		pred->succ = succ->succ;
		if(succ->succ != 0) succ->succ->prev = pred;
	}
	
	return;
	
	/*when the pointer is freed more than once*/
}

/*
errors we need to handle:
1) fragmentation
2) saturation
3) redundant freeing
4) freeing pointers to dynamic memory that were never returned from malloc
5) freeing pointers that were never allocated
*/
