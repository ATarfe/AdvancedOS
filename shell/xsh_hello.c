#include<xinu.h>
#include<stdio.h>

shellcmd xsh_hello(int nargs, char *args[]) {
	
	if(nargs == 2){

		printf("Hello %s, Welcome to the world of Xinu!!", args[1]);

	} else if(nargs < 2){

		printf("Too less arguments!");	

	} else {

		printf("Too many arguments!");
	}

}


