#include "signal_handlers.h"
#include <stdio.h>
extern int verbose;
/*
*
*  SIGNAL HANDLERS
*
*/
void sigint_handler(int sig){
	if(verbose){printf("Entering Sigint Handler\n");}
	printf("\n");//newline

	//DO WORK

	if(verbose){printf("Exiting Sigchld Handler.\n");}
	return;
}
void sigchld_handler(int sig){
	if(verbose){printf("Entering Sigchld Handler.\n");}

	//DO WORK

	if(verbose){printf("Exiting Sigchld Handler.\n");}
	return;
}
void sigstp_handler(int sig){
	if(verbose){printf("Entering Sigstp Handler.\n");}

	//DO WORK

	if(verbose){printf("Exitring Sigstp Handler.\n");}
	return;
}
void sigalrm_handler(int sig){
	if(verbose){printf("Entering Sigalrm Handler.\n");}

	//DO WORK

	if(verbose){printf("Exiting Sigalrm Handler.\n");}
	return;
}