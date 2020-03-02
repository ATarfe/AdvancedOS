/*future_set.c - future_set*/

#include<xinu.h>
#include<future.h>

/*---------------------------------------------------------------------
 *
 *      future_set - Set a value in the future and may change the state
 *
 * --------------------------------------------------------------------
 */

syscall future_set(
                
                future*       f,
                char*           value

        )
{
	intmask mask;				/*Saved interrupt mask*/
	mask = disable;
	
	if(f->flags == FUTURE_EXCLUSIVE){
	
		if(f->state == FUTURE_EMPTY || f->state == FUTURE_WAITING || f->state == FUTURE_FREE){		/*Check the state of the 
future to be Empty, Free or Waiting*/
			f->value = value;
			f->state = FUTURE_FULL;
			ready(f->pid);
			restore(mask);
			return OK;
		}
		else{
			restore(mask);
			return SYSERR;
		}	
	}

	else if(f->flags == FUTURE_SHARED){

		if(f->state == FUTURE_EMPTY || f->state == FUTURE_WAITING || f->state == FUTURE_FREE){
			
			f->value = value;
			f->state = FUTURE_FULL;	
		}

		if(f->state == FUTURE_WAITING){
			queue* temp = f->get_queue;

			while(f->get_queue->next != NULL){
				f->get_queue = f->get_queue->next;
				freemem((char *)temp, sizeof(queue));
				resume(temp->pid);
				temp = f->get_queue;
				
			}
		}
		else if(f->state == FUTURE_FULL){
			restore(mask);
			return SYSERR;
		}	
	}
}
