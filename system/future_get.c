/* future_get.c - future_get*/
#include<xinu.h>
#include<future.h>

/*-------------------------------------------------------------------
 *
 * 	future_get - Get the value of the future set by an operation
 *
 * ------------------------------------------------------------------
 */

syscall future_get(
		
		future* 	f,
		char*		value
	)

{
	
        intmask mask;								/*Saved interrupt mask*/
        struct procent *prptr;							/*Ptr to process table entry*/
        mask = disable();

	if(f->flags == FUTURE_EXCLUSIVE){
	/*Checks for available future state*/
        	if(f->state == FUTURE_EMPTY || f->state == FUTURE_FREE){
			/*If empty*/
                	f->pid = currpid;
                	prptr = &proctab[currpid];				
                	prptr->prstate = PR_WAIT;					
                	f->state = FUTURE_WAITING;					/*Goes to waiting*/
                	resched();
       		 }
       		 if(f->state == FUTURE_FULL){						/*If available*/				
                	*value = *f->value;						/*Returns the value*/		
                	f->state = FUTURE_FREE;	
                	restore(mask);
               		return OK;
        	}
       		 else{
                	restore(mask);
                	return SYSERR;
        	}
        	restore(mask);
	}
	
	else if(f->flags == FUTURE_SHARED){
		
		if(f->state == FUTURE_EMPTY || f->state == FUTURE_FREE || f->state == FUTURE_WAITING) {
			
			f->pid = currpid;
			f->state = FUTURE_WAITING;
			

			if(f->get_queue == NULL){
				f->get_queue = (struct queue*)getmem(sizeof(struct queue));
				f->get_queue->next = NULL;
				f->get_queue->pid = f->pid;
			}
			else {
				queue* temp = f->get_queue;
				
				while(temp->next != NULL){

					temp = temp->next;
				}
				temp->next = (struct queue*)getmem(sizeof(struct queue));
				temp->next->pid = f->pid;
				temp->next->next = NULL;
			}
			
			prptr = &proctab[currpid];
			prptr->prstate = PR_WAIT;
			resched();

			f->value = value;
		}
		
		else if(f->state == FUTURE_FULL){
			*value = *f->value;
			f->state = FUTURE_FREE;
			restore(mask);
			return OK;
		}
	}

}

