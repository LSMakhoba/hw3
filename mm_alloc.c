/*
 * mm_alloc.c
 *
 * Stub implementations of the mm_* routines. Remove this comment and provide
 * a summary of your allocator's design here
					
					<<mm_malloc>>
first find a free block, the search begins at the base address of the heap. If the block is free an attempt is made to ensure it is in line with the requested size by splitting the block if possible.Once done, that address is returned and its free value is set to false. 
If the first chunk is not free or large enough, the itteration continues, keeping track of the last visited block.Traversals continue until the last possible block, in this case the heap will need to be extended. 
To extend the heap, the brk pointer is moved and a new block is initialized, updating the next pointer of the last block on the heap.Keeping track of the last block ckecked ensure the heap can be extended withouth traversing through the entire list again bringing effeciency.

					<<mm_realloc>>

A memory copy operation is used which copies data from block to block. Our previously created mm_malloc function is used to allocate a new block, that data is then copied from the old one to a new one. the old block is freed and the new address pointer is returned.

					<<mm_free>>

first the pointer is checked to be valid, the block is  then marked as free. The blocks neighbours(prev and next)  are then also checked to be free, if they are, the appropriate fusions are made to the blocks. This is done to all free neighbours untill the last is reached.

 */

#include "mm_alloc.h"
#include <stdlib.h>
#define align4(size)//Allign pointer to integer size

void *base;


s_block_ptr find_block(s_block_ptr *last, size_t size){
	
	base = NULL;
	s_block_ptr b=base;

	while(b && !(b->size >= size && b->free)) {
		*last = b;
		b = b->next;
	}
	return (b);
}


void split_block(s_block_ptr b, size_t s){

	s_block_ptr nwBlock;
	nwBlock = b->data + s;
	nwBlock->size = b->size - s - BLOCK_SIZE;
	nwBlock->next = b->next;
	nwBlock->free = 1;
	b->size = s;
	b->next = nwBlock;

}

s_block_ptr extend_heap(s_block_ptr last, size_t s){

	s_block_ptr b;
	b =sbrk(0);

	if(sbrk(BLOCK_SIZE + s) == (void*)-1)
		return (NULL);//sbrk fail check

	b->size = s;
	b->next = NULL;

	if (last)
		last->next = b;
	
	b->free = 0;
	return (b);

}

void* mm_malloc(size_t size){
	
	base=NULL;
	s_block_ptr b,last;
	size_t s;
	s = size;
	if (base) {	//First find a block
		last = base;
		b = find_block(&last,s);
		if (b) {//can we split??
			if ((b->size - s) >= (BLOCK_SIZE + 4))
			split_block(b,s);
		
		b->free=0;
		}
	else{//No fitting block, extend the heap
		b = extend_heap(last,s);

	if (!b)
		return (NULL);
		}

	}
	else{
		b = extend_heap(NULL,s);
		if (!b)
			return (NULL);
		base = b;
	}
	return (b->data);
	//return calloc(1, size);
	//#endif
	}


s_block_ptr get_block(void *p){
	char *tmp;
	tmp = p;
	return (p = tmp -= BLOCK_SIZE);
}

s_block_ptr fusion(s_block_ptr b){

	if(b->next && b->next->free){
		b->size += BLOCK_SIZE + b->next->size;
		b->next = b->next->next;
	
		if (b->next)
		b->next->prev = b;
	}
	return (b);
}

void copy_block(s_block_ptr src, s_block_ptr dst){

	int *ddata,*sdata;
	size_t i;
	ddata = dst->ptr;
	sdata = src->ptr;
			
	for(i=0; i*4<src->size && i*4<dst->size; i++)
		ddata[i] = sdata[i];

}

void* mm_realloc(void* ptr, size_t size){
	//#ifdef MM_USE_STUBS
	//#else

	size_t s;
	s_block_ptr b, new;
	void *newp;
	if (!ptr)
		return (mm_malloc(size));

	if (valid_addr(ptr)){
		s = align4(size) (((((size)-1)>>2)<<2)+4);//assuming a 32 bit case, thus pointer is a multiple of 4(bytes).
		b = get_block(ptr);
		
		if (b->size >= s){
			if (b->size - s >= (BLOCK_SIZE + 4))
				split_block(b,s);
		}
		else{//Try fusion with next if possible
			if (b->next && b->next->free && (b->size + 				BLOCK_SIZE + b->next->size) >= s){
				fusion(b);

			if (b->size - s >= (BLOCK_SIZE + 4))
				split_block(b,s);
		}
		else{//good old realloc with a new block
			newp = mm_malloc(s);

		if (!newp)	//we’re doomed !
			return (NULL);
		
		//I assume this work!
		new = get_block(newp);
		//Copy data
		copy_block(b,new);
		//free the old one
		mm_free(ptr);
		return (newp);
		}
	}
	return (ptr);
}
return (NULL);

return realloc(ptr, size);

//#error Not implemented.
//#endif
}

void mm_free(void* ptr)
{
//#ifdef MM_USE_STUBS
//#else
	base=NULL;
	s_block_ptr b;
	if (valid_addr(ptr)){
		b = get_block(ptr);	
		b->free = 1;
	//fusion with previous if possible
	
		if (b->prev && b->prev->free)
			b = fusion(b->prev);

		//then fusion with next
		if (b->next)
			fusion(b);
	
		else{//free the end of the heap
			if (b->prev)
				b->prev->next = NULL;	
			else //No more blocks!
				base = NULL;
			
			brk(b);
			}
		}
//free(ptr);
//#error Not implemented.
//#endif
}


int valid_addr(void *p){	
	base = NULL;
	if(base){
		if( p>base && p<sbrk(0))
			return(p == (get_block(p))->ptr);
	}
	return (0);
}





