/* future_free.c - future_free */
#include<xinu.h>
#include<future.h>

/*---------------------------------------------------
 *
 * 	future_free - Frees the allocated future
 * 
 *---------------------------------------------------
 */

syscall future_free(
		
		future* f 
	)
{
	intmask mask;				/*Saved interrpt mask*/
	mask = disable();

	freemem(f->value, f->size);		/*Free memory of the value*/
	freemem((char *)f, sizeof(future));	/*Free memory of the future*/
	
	while(f->get_queue != NULL){
		queue* temp = f->get_queue;
		f->get_queue = f->get_queue->next;
		freemem((char *) temp, sizeof(queue));
	}

	restore(mask);
	return OK;
}	
