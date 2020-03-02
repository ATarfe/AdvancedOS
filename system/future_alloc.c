/* future_alloc.c - future_alloc*/
#include<xinu.h>
#include<future.h>

/*------------------------------------------------
 *
 * 	future_alloc - Allocates a new future
 *
 *------------------------------------------------
 */

future* future_alloc
	(
		int		future_flags,
		uint32		size
	)
{
	future *f;
	f = (future *)getmem(sizeof(future));		/*Allocates memory to the future*/
	f->state = FUTURE_EMPTY;			/*Initializes the future state as empty */
	f->flags = future_flags;			/*Initializes the future flags*/
	f->size = size;					/*Gives a size*/
	f->value = (char *)getmem(size);		/*Allocates the given size*/
	return f;		
}



