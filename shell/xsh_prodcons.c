#include<xinu.h>
#include<prodcons.h>
#include<stdio.h>
#include<stdlib.h>

int n;

shellcmd xsh_prodcons(int nargs, char *args[]){

	int32 count = 2000;
	sid32 produced, consumed;

	if(nargs == 2){
		count = atoi(args[1]);
	} 
	else if(nargs > 2){
		printf("Too many arguments!");
		return 0;
	}

	consumed = semcreate(0);
	produced = semcreate(1);

	resume( create(producer, 1024, 20, "producer", 3, count, consumed, produced));
	resume( create(consumer, 1024, 20, "consumer", 3, count, consumed, produced));
	return(0);

}

