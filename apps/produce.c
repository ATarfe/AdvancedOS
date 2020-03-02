#include <xinu.h>
#include <prodcons.h>

void producer(int count, sid32 consumed, sid32 produced) {
	int32 i;
	
	for( i = 1; i <= count; i++){
		wait(consumed);
		printf("The program produced %d \n", n);
		n++;
		signal(produced);
	};
}



