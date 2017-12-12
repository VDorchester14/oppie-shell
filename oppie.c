/* 
*  AUTHOR: Vail Dorchester
*  
*  
*  
*  
*/ 

//libraries
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//prototypes
void cmd_loop();//main loop

//main
int main(int argc, char **argv){
	//TODO: load config file if any

	//command loop
	cmd_loop();

	//TODO: perform shutdown cleanup

	return EXIT_SUCCESS;
}

/*  
*   implementation
*/  

void cmd_loop(){
	printf("This is the main loop\n");

	return;
}