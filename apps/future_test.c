#include<xinu.h>
#include<stdlib.h>
#include<future.h>

future **fibfut;
int zero = 0;
int one = 1;

int ffib(int);

int ffib(int n){
	
	int minus1 = 0;
	int minus2 = 0;
	int this = 0;
	
	if(n == 0){
		future_set(fibfut[0],(char *)&zero);
		return OK;
	}
	
	if(n == 1){
		future_set(fibfut[1], (char *)&one);
		return OK;
	}
	
	future_get(fibfut[n - 2], (char *)&minus2);
	future_get(fibfut[n - 1], (char *)&minus1);

	this = minus1 + minus2;

	future_set(fibfut[n], (char *)&this);

	return(0);
}



uint32 future_ring(future *in, future *out);
uint32 future_prod(future *fut, int n);
uint32 future_cons(future *fut);


uint32 future_test(int nargs, char *args[]){
	
	int ring = 0;
	int future_flags = 0;
	int ring_count = atoi(args[2]);
	int final_val;
	int i;
	int fib = -1;

	#ifndef NFUTURE
		printf("no support for futures (NFUTURES not defined.)\n");
		return OK;
	#endif

	
	
	printf("Value of args:  %s %s %s %d\n",args[0], args[1], args[2], nargs);
	
	if(ring == 1){
		future *first, *in, *out = NULL;
		first = future_alloc(future_flags, sizeof(int));
		in = first;
		for(i = 0; i < ring_count; i++){
			out = future_alloc(future_flags, sizeof(int));
			resume( create(future_ring, 1024, 20, "", 2, in, out));
			in = out;
		}
		printf("master sets %d\n", ring_count);
		future_set(first, (char *)&ring_count);
		future_get(out, (char *)&final_val);
		printf("master gets %d\n", final_val);
		return(OK);
	}
	
	if(nargs == 3 && strncmp(args[1], "-f", 2) == 0){
		fib = atoi(args[2]);
		
		if(fib > -1){
			int final_fib;
		
			if((fibfut = (future **)getmem(sizeof(future *) * (fib + 1))) == (future **)SYSERR) {
		
				printf("getmem failed\n");
				return(SYSERR);
			}
	

			for(i = 0; i <= fib; i++){
				if((fibfut[i] = future_alloc(FUTURE_SHARED, sizeof(int))) == (future * )SYSERR) {
					printf("future_alloc failed\n");
					return(SYSERR);	
				}
			}	

		
			for(i = 0; i <=fib; i++){
				resume( create(ffib, 1024, 20, "ffib", 1, i));
			}		

		
			future_get(fibfut[fib], (char *)&final_fib);
		
			for(i = 0; i <= fib; i++){
				future_free(fibfut[i]);
			}
		
			freemem((char *)fibfut, sizeof(future *) * (fib + 1));
			printf("Nth Fibonacci value for N = %d is %d\n", fib, final_fib );
			return(OK);
		}
	}

	if(nargs == 2 && strncmp(args[1], "-pc", 3) == 0){
		
	
		future* f_exclusive,
		      * f_shared,
		      * f_queue;
	
		f_exclusive = future_alloc(FUTURE_EXCLUSIVE, sizeof(int));
		f_shared    = future_alloc(FUTURE_SHARED, sizeof(int));
		f_queue     = future_alloc(FUTURE_QUEUE, sizeof(int));

		resume( create(future_cons, 1024, 20, "fcons1", 1, f_exclusive) );
		resume( create(future_prod, 1024, 20, "fprod1", 2, f_exclusive, 1) );

		resume( create(future_cons, 1024, 20, "fcons2", 1, f_shared) );
		resume( create(future_cons, 1024, 20, "fcons3", 1, f_shared) );
		resume( create(future_cons, 1024, 20, "fcons4", 1, f_shared) );
		resume( create(future_cons, 1024, 20, "fcons5", 1, f_shared) );
		resume( create(future_prod, 1024, 20, "fprod2", 2, f_shared, 2) );
      	
	}


	else{
		printf("No valid option\n");
		return(OK);
	}
	return(OK);
}


uint future_ring(future *in, future *out){
	int val;
	future_get(in, (char *)&val);
	printf("Process %d gets %d, puts %d\n", getpid(), val, val - 1);
	val--;
	future_free(in);
	future_set(out, (char *)&val);
	return OK;
}


uint future_prod(future* fut, int n){
	printf("Produced %d\n", n);
	future_set(fut, &n);
	return OK;
}

uint future_cons(future* fut){
	int i, status;
	status = (int)future_get(fut, &i);
	if(status < 1){
		printf("future_get failed\n");
		return -1;
	}
	printf("Consumed %d\n", i);
	return OK;
}

