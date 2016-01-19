/*



 In this project we are implementing it using explicit list with pointers.

 Explicit pointers in block header to next and previous free blocks

 They are simply just double-linked lists

 Each block has previous and next link to a free block

 We are using 'stack' technique from data structure class, where

 we put a new free block at the beginning of the free list which is lptr in our case.

 our case.



 Our block sizes will be of 24 bytes at least.

 We were referring to our course book System Programming. (page 869, edition 2)



 * NOTE TO STUDENTS: Replace this header comment with your own header

 * comment that gives a high level description of your solution.

 */
#include <stdio.h>

#include <stdlib.h>

#include <string.h>

#include <unistd.h>



#include "mm.h"

#include "memlib.h"



/*********************************************************
 * NOTE TO STUDENTS: Before you do anything else, please
 * provide your team information in the following struct.
 ********************************************************/

team_t team = {

	"Muso Chert",

	"Nurzhan Yergozhin",

	"nurzhan@kaist.ac.kr",

	"Musokhon Mukayumkhonov",

	"muso@kaist.ac.kr",

};



#define WSIZE       4		/* Single word (4 bytes) */

#define DSIZE       8		/* Double word (8 bytes) */

#define ALIGNMENT   8		/* Alignment requirement (8 bytes) */

#define OVERHEAD 	16  	/* Header, Prev, Next, footer (16 bytes) */

#define CHUNKSIZE (1<<12)	/* ChunkSize of (bytes) */

#define BSIZE     24  	/* Mininum Block size (24 bytes) */



/* rounds up to the nearest multiple of ALIGNMENT */

#define ALIGN(p) (((size_t)(p) + (ALIGNMENT-1)) & ~0x7)



/*Fuction for getting maximum*/

#define MAX(x, y) ((x) > (y) ? (x) : (y))



/* Pack a size and allocated bit into a word */

#define PACK(size, alloc)  ((size) | (alloc))



/* Read and write a word at address p */

#define GET(p)       (*(int *)(p))

#define PUT(p, val)  (*(int *)(p) = (val))





/* Given block ptr bp, compute address of its header and footer */

#define HDRP(bp)  ((void *)(bp) - WSIZE)

#define FTRP(bp)  ((void *)(bp) + HDRSIZE(bp) - DSIZE)



/* Read the size and allocated fields from address p */

#define GET_SIZE(p) (GET(p) & ~0x7)

#define HDRSIZE(p)  (GET(HDRP(p)) & ~0x7)

#define GET_HDRALLOC(p) (GET(HDRP(p)) & 0x1)

#define GET_FTRALLOC(p) (GET(FTRP(p)) & 0x1)



/* Given block ptr bp, compute address of next and previous blocks */

#define NEXT_BLKP(bp)  ((void *)(bp) + HDRSIZE(bp))

#define PREV_BLKP(bp)  ((void *)(bp) - GET_SIZE((void *)(bp) - DSIZE))

#define NEXT_PTR(bp)  (*(void **)(bp + WSIZE))

#define PREV_PTR(bp)  (*(void **)(bp))





/* Global variables, heap_listp points to the start of heap

listp is holding list of free blocks available*/

static char *heap_listp = 0;

static char *lptr = 0;



/* Function prototypes */

static void *extend_heap(size_t words);

static void place(void *bp, size_t asize);

static void *find_fit(size_t asize);

static void *coalesce(void *bp);

static void insert(void *bp);

static void del(void *bp);

/*

The mem_init function models the virtual memory available to the heap as a

large, double-word aligned array of bytes.The mm_init function gets four words from the memory system and initializes them to create the empty free list

.It then calls the extend_heap function which we extend heap by minimum blocksize

and will create initial free block. Then we will be able to use malloc, free or realloc

afterwards.

*/

int mm_init(void) {



	/* Create initial empty heap

	return -1 if unable to get heap space*/



	if ((heap_listp = mem_sbrk(2*BSIZE)) == (void *)-1)

		return -1;



	PUT(heap_listp, 0); 					/* alignment padding */

	PUT(heap_listp + (1*WSIZE), PACK(BSIZE, 1)); 		/* prologue header */

	PUT(heap_listp + (2*WSIZE), 0); 			/* prev pointer */

	PUT(heap_listp + (3*WSIZE), 0); 			/* next pointer */

	PUT(heap_listp + BSIZE, PACK(BSIZE, 1)); 		/* prologue footer */

	PUT(heap_listp+  BSIZE + WSIZE, PACK(0, 1)); 		/* epilogue header */

	lptr = heap_listp + 2*WSIZE;



	/* Extend the empty heap with a free block of BSIZE bytes */

	if (extend_heap(BSIZE) == NULL) return -1;



	return 0;

}



/*

 mm_malloc requests a block of size bytes of memory by calling the mm_malloc

 function. After checking for spurious requests, the allocator must

 adjust the requested block size to allow room for the header and the footer, and to

 satisfy the double-word alignment requirement. */

void *mm_malloc(size_t size) {

	size_t asize;      /* Adjusted block size */

	size_t extendsize; /* amount to extend heap if no fit */

	char *bp;



	/* Ignore spurious requests */

	if (size == 0)

		return NULL;



	/* Adjust block size to include overhead and alignment reqs */

	asize = MAX(ALIGN(size) + ALIGNMENT, BSIZE);



	/* Search the free list for a fit */

	if ((bp = find_fit(asize))!=NULL) {

		place(bp, asize);

		return bp;

	}



	/* No fit found. Get more memory and place the block */

	extendsize = MAX(asize, BSIZE);

	/* return NULL if unable to get additional space */

	if ((bp = extend_heap(extendsize)) == NULL) return NULL;

	/* place block and return bp */

	place(bp, asize);

	return bp;

}

/* mm_free frees desired part and adds it to te list of

 free available blocks which is lptr in our case.

 it then coelesces acordingly beacuse there will be 4 case to

 look after which we wll difcuss*/

void mm_free(void *bp){

	/* just return if the pointer is NULL */

	if(bp == NULL) return;

	size_t size = HDRSIZE(bp);



	/* set this block back to free and coalese */

	PUT(HDRP(bp), PACK(size, 0));

	PUT(FTRP(bp), PACK(size, 0));

	coalesce(bp); // merging block if we need

}



/*

realloc function reallocates the memory with pointer bp to a new size

realloc function that considers some cases when reallocing a block

1) size == 0 just free block

2) else bp == NULL just malloc block

3) else size need to allocate is less than or equal to current size then just we allocate it

4) else we try to use our next free block in order to fit our block and also consider cases with epilogue block

*/

void *mm_realloc (void *bp, size_t size){

	if(size == 0) { // if size == 0 then we can just free bp and return the pointer

		mm_free (bp);

		return NULL;

	}

    if(bp == NULL) { // if bp == NULL then we just allocating a block for appropriate size

		bp = mm_malloc(size);

		return bp;

	}

    // this is the normal case that we will usually face

    size_t cursz = HDRSIZE(bp); // size of the current block

    size_t nsz = ALIGN(size + OVERHEAD); // alligning size, so that it will satisfy our requirements of block size

    /* if the size fits we just return bp */

    if(nsz <= cursz) return bp; // if the size we need to allocate is less or equal than the size we have at current block

    else { // then we can just allocate a block of specific size

            // when our block size is bigger than our current size at bpointer bp

        size_t next_alloc = GET_HDRALLOC(NEXT_BLKP(bp));

        size_t csize;

        size_t asize;

        /* if next block + current free block >= current size (is ok to fit) then we just fitting to it our block */

        if(!next_alloc && ((csize = cursz + HDRSIZE(NEXT_BLKP(bp)))) >= nsz){

            del(NEXT_BLKP(bp)); // we need to delete next block pointer

            PUT(HDRP(bp), PACK(csize, 1)); // and merge them

            PUT(FTRP(bp), PACK(csize, 1));

            return bp;

        }

        /* if the next block is also free and this block is right before the epilogue block */

        if(!next_alloc && ((HDRSIZE(NEXT_BLKP(NEXT_BLKP(bp)))) == 0)){

            csize = nsz - cursz + HDRSIZE(NEXT_BLKP(bp)); // is equal to our new size - current siz + SIZE of next block

            void *temp = extend_heap(csize); // we need to extend heaop by csize

            asize = cursz + HDRSIZE(temp); // our new size curesize + the ammount we extended

            PUT(HDRP(bp), PACK(asize, 1));

            PUT(FTRP(bp), PACK(asize, 1));

            return bp;

        }

        /* the similar case to previous one but here bp already is right before the epilogue */

        if(HDRSIZE(NEXT_BLKP(bp)) == 0){

            csize = nsz - cursz;

            void *temp = extend_heap(csize);

            asize = cursz + HDRSIZE(temp);

            PUT(HDRP(bp), PACK(asize, 1));

            PUT(FTRP(bp), PACK(asize, 1));

            return bp;

        }

        /* if there was not enough space for our new block then we need just ack for additional size */

        void *newbp = mm_malloc(nsz);  /* allocating a space we need */

        place(newbp, nsz); /* placing it */

        memcpy(newbp, bp, nsz); /* copy to newbp from bp nsz bytes */

        mm_free(bp); /* free a block at pointer bp */

        return newbp; /* returning our new pointer */

    }

    /* in any other case returning NULL */

	return NULL;

}



/*

The extend_heap function is invoked in two different circumstances: (1) when

the heap is initialized, and (2) when mm_malloc is unable to find a suitable fit. To

maintain alignment, extend_heap rounds up the requested size to the nearest

multiple of 2 words (8 bytes), and then requests the additional heap space from

the memory system. The remainder of the extend_heap function is somewhat subtle.

The heap begins on a double-word aligned boundary, and every call to extend_

heap returns a block whose size is an integral number of double words.

*/

static void *extend_heap(size_t words) {

	char *bp;

	size_t size;



	/* Allocate an even number of words to maintain alignment */

	size = (words % 2) ? (words+1) * WSIZE : words * WSIZE;

    size = MAX(size, BSIZE);

	/* call for more memory space */

	if ((int)(bp = mem_sbrk(size)) == -1) return NULL;



	/* Initialize free block header/footer and the epilogue header */

	PUT(HDRP(bp), PACK(size, 0));         /* free block header */

	PUT(FTRP(bp), PACK(size, 0));         /* free block footer */

	PUT(HDRP(NEXT_BLKP(bp)), PACK(0, 1)); /* new epilogue header */

	/* coalesce bp with next and previous blocks */

	return coalesce(bp);

}

/*We know that allocator the minimum block size is 24 bytes. If the remainder

of the block after splitting would be greater than or equal to the minimum block

size, then we go ahead and split the block

we need to place the new allocated block  before moving to the next block */

static void place(void *bp, size_t asize){

	size_t csize = HDRSIZE(bp);



	if ((csize - asize) >= BSIZE) {

		PUT(HDRP(bp), PACK(asize, 1));

		PUT(FTRP(bp), PACK(asize, 1));

		del(bp); // deleting current block

		bp = NEXT_BLKP(bp);

		PUT(HDRP(bp), PACK(csize-asize, 0));

		PUT(FTRP(bp), PACK(csize-asize, 0));

		coalesce(bp);

	}

	else {

		PUT(HDRP(bp), PACK(csize, 1));

		PUT(FTRP(bp), PACK(csize, 1));

		del(bp);

	}

}

/*

 * find fit goes through the free list and uses a first-fit search

 * to find the first free block with size greater than or equal to the requested block size

 * we will send null if it doesnt find.

*/

static void *find_fit(size_t asize){

	void *bp;



	/* for loop through list to find first fit */

	for (bp = lptr; GET_HDRALLOC(bp) == 0; bp = NEXT_PTR(bp))

		if (asize <= (size_t) HDRSIZE(bp))

			return bp;

	return NULL; /* returns NULL if can't find it in the list */

}

/*

 while freeing a head we will face 4 cases in which,

1. The previous and next blocks are both allocated.

2. The previous block is allocated and the next block is free.

3. The previous block is free and the next block is allocated.

4. The previous and next blocks are both free. We linp previous

and next free blocks after coeleseing the desired part.

*/

static void *coalesce(void *bp){

	size_t prev_alloc;

	prev_alloc = GET_FTRALLOC(PREV_BLKP(bp)) || PREV_BLKP(bp) == bp;

	size_t next_alloc = GET_HDRALLOC(NEXT_BLKP(bp));

	size_t size = HDRSIZE(bp);



	/* Case 1 next block is free */

	if (prev_alloc && !next_alloc) {

		size += HDRSIZE(NEXT_BLKP(bp));

		del(NEXT_BLKP(bp)); // delete next block pointer

		PUT(HDRP(bp), PACK(size, 0));

		PUT(FTRP(bp), PACK(size, 0));

	}/* Case 2 prev block is free */

    if (!prev_alloc && next_alloc) {

		size += HDRSIZE(PREV_BLKP(bp));

		bp = PREV_BLKP(bp);

		del(bp); // delete block

		PUT(HDRP(bp), PACK(size, 0)); // merge it sizes to the previous block

		PUT(FTRP(bp), PACK(size, 0));

	}/* Case 3 both blocks are free */

    if (!prev_alloc && !next_alloc) {

		size += HDRSIZE(PREV_BLKP(bp)) + HDRSIZE(NEXT_BLKP(bp));

		del(PREV_BLKP(bp)); // delete previous block

		del(NEXT_BLKP(bp)); // delete next block

        // merge all 3 sizes to the current block

		bp = PREV_BLKP(bp);

		PUT(HDRP(bp), PACK(size, 0));

		PUT(FTRP(bp), PACK(size, 0));

	}/* lastly insert bp into free list and return bp */

	insert(bp);

	return bp;

}



/*

Putting a block in the beginning of our 'stack'

and relinking all the links

*/



static void insert(void *bp){

	/* linking bp to our stack top */

	NEXT_PTR(bp)=lptr;

	/* previous of our stack top will be our bp */

	PREV_PTR(lptr)=bp;

	/* prev bp will be NULL  because it is our new top */

	PREV_PTR(bp)=NULL;

	/* now we need to save our new top */

	lptr = bp;

}

/*

Removing block locatted at bp

*/



static void del(void *bp){

	/* if previous bp exists, then we link it to next to bp next */

	if (PREV_PTR(bp))

		NEXT_PTR(PREV_PTR(bp))=NEXT_PTR(bp);

	else	/* else our new top will be next of bp, if we are deleting our top */

		lptr = NEXT_PTR(bp);

	/*  relinking */

	PREV_PTR(NEXT_PTR(bp))=PREV_PTR(bp);

}

