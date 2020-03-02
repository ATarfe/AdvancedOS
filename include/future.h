#ifndef _FUTURE_H_
#define _FUTURE_H_

/*STATES*/
#define FUTURE_FREE	0
#define FUTURE_EMPTY	1
#define FUTURE_FULL	3
#define FUTURE_WAITING  5

/*MODES*/
#define FUTURE_EXCLUSIVE  1
#define FUTURE_SHARED	  2
#define FUTURE_QUEUE	  3

typedef struct queue{
	pid32 pid;
	struct queue* next;
} queue;


typedef struct futent{
	uint16 state;
	uint16 flags;
	uint32 size;
	char *value;
	pid32 pid;
	queue* set_queue;
	queue* get_queue;
} future;

/*INTERFACE*/
future* future_alloc(int future_flags, uint size);
syscall future_free(future*);
syscall future_get(future*, char *);
syscall future_set(future*, char *);


#endif


