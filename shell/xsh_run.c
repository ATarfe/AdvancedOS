#include<xinu.h>
#include<stdlib.h>
#include<prodcons.h>
#include<functions.h>
#include<future.h>

shellcmd xsh_run(int nargs, char *args[]){

	if((nargs == 1) || (strncmp(args[1], "list", 5) == 0)){
		printf("my_function_1\n");
		printf("my_function_2\n");
		return OK;
	
	}
	args++;
	nargs--;

	if(strncmp(args[0], "my_function_1", 13) == 0){
		my_function_1(nargs, args);

		resume( create((void *)my_function_2, 4096, 20, "my_function_2", 2, nargs, args));
	}
	
	else if(strncmp(args[0], "my_function_2", 13) == 0){
		my_function_2(nargs, args);

		resume( create((void *)my_function_1, 4096, 20, "my_function_1", 1, nargs, args));
	}

	else if(strncmp(args[0], "prodcons", 8) == 0){
		xsh_prodcons(nargs, args);
	}
	else if(strncmp(args[0], "stream_proc", 11) == 0){
		nargs--;
		stream_proc(nargs, args);
	}
	else if(strncmp(args[0], "stream_proc_future", 18) == 0){
		nargs--;
		stream_proc(nargs, args);
	}
	else if(strncmp(args[0], "future_test", 11) == 0){
		future_test(nargs, args);	
	}


}

