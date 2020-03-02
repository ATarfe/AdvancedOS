#include <xinu.h>
#include <prodcons.h>


void consumer(int count, sid32 consumed, sid32 produced) {
	int32 i;
	
	for( i = 1; i <= count; i++){
		wait(produced);
		printf("The program consumed %d \n", n);
		signal(consumed);
	};


}



