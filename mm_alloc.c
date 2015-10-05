/*
 * mm_alloc.c
 *
 * Stub implementations of the mm_* routines. Remove this comment and provide
 * a summary of your allocator's design here.




	
 */

#include "mm_alloc.h"

#include <stdlib.h>
void *base;
//#define align4(1024) //(((((x)-1)>>2)<<2)+4)
/* Your final implementation should comment out this macro. */
//#define MM_USE_STUBS

s_block_ptr find_block(s_block_ptr *last, size_t size){
	
	base = NULL;
	s_block_ptr b=base;

	while(b && !(b->free && b->size >= size)) {

		*last = b;
		b = b->next;
		}
	return (b);

}


s_block_ptr extend_heap(s_block_ptr last, size_t s){

	s_block_ptr b;
	b =sbrk(0);

	if(sbrk(BLOCK_SIZE + s) == (void*)-1)
		/*
	sbrk fails,go to die
	*/
		return (NULL);
	
	b->size = s;nasty c

	b->next = NULL;

	if (last)
		last->next = b;
		b->free = 0;

	return (b);

}

void split_block(s_block_ptr b, size_t s){

	s_block_ptr new;
	new = b->data + s;
	new->size = b->size - s - BLOCK_SIZE;
	new->next = b->next;
	new->free = 1;
	b->size = s;
	b->next = new;

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

	int *sdata,*ddata;
	size_t i;
	sdata = src->ptr;
	ddata = dst->ptr;	
	
	for(i=0; i*4<src->size && i*4<dst->size; i++)
		ddata[i] = sdata[i];

}

int valid_addr(void *p){	
	base = NULL;
	if(base){
		if( p>base && p<sbrk(0))
		{
		return(p == (get_block(p))->ptr);

		}
	}

	return (0);

}



s_block_ptr get_block(void *p){
	char *tmp;
	tmp = p;
	return (p = tmp -= BLOCK_SIZE);
}

void* mm_malloc(size_t size){
	
	base=NULL;
	#define align4(size) 

	s_block_ptr b,last;
	size_t s;
	s = size;
	if (base) {	
	//First find a block
	last = base;
	b = find_block(&last,s);
	if (b) {
		//can we split
		if ((b->size - s) >= (BLOCK_SIZE + 4))
		split_block(b,s);
		
		b->free=0;
		}
	else
	{
	//No fitting block, extend the heap
	b = extend_heap(last,s);
	if (!b)
		return (NULL);
		}

	}
	else
		{

		/*et_blo
		first time
		*/
		b = extend_heap(NULL,s);

		if (!b)
		return (NULL);
		
		base = b;
	}

	return (b->data);
	//return calloc(1, size);
	//#endif
	}

void* mm_realloc(void* ptr, size_t size){

	//#ifdef MM_USE_STUBS
	//#else

	size_t s;
	s_block_ptr b, new;
	void *newp;
	if (!ptr)
		return (mm_malloc(size));

	if (valid_addr(ptr))
		{
	#define align4(size) 
	s = align4(size) (((((size)-1)>>2)<<2)+4);

	b = get_block(ptr);

	if (b->size >= s)

	{
		if (b->size - s >= (BLOCK_SIZE + 4))
	split_block(b,s);

	}

	else

	{
		//Try fusion with next if possible
		if (b->next && b->next->free && (b->size + BLOCK_SIZE + b->next->size) >= s)
		{
		fusion(b);

		if (b->size - s >= (BLOCK_SIZE + 4))
		split_block(b,s);
		}
		else
		{
		//good old realloc with a new block
		newp = mm_malloc(s);

		if (!newp)
		//we’re doomed !

		return (NULL);
		//I assume this work!
		new = get_block(newp);
		//Copy data
		copy_block(b,new);
		//free the old one
		free(ptr);
	
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

	if (valid_addr(ptr))
	{

	b = get_block(ptr);	

	b->free = 1;

	
	//fusion with previous if possible
	

	if (b->prev && b->prev->free)

	b = fusion(b->prev);


	//then fusion with next


	if (b->next)

	fusion(b);

	else
		
	{
	//free the end of the heap
		if (b->prev)
		b->prev->next = NULL;
	
		else
		//No more block!
		base = NULL;

	brk(b);

	}

	}

    //free(ptr);
//#error Not implemented.
//#endif
}
