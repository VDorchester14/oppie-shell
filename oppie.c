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

//shell constants
#define MAXLINE 1024//max line length
#define MAXARGS 128//max number of arguments
#define MAXJOBS 16//max number of jobs
#define MAXJID 1<<16//max jobid
#define BUFSIZE 64//not used rn
#define DELIM " \t\r\n\a"//also not used rn

//job states
#define UNDEF 0
#define FG 1
#define BG 2
#define ST 3
#define KL 4

//prototypes
void cmd_loop(void);//main loop
char *read_line(void);//reads the user input
int parseline(const char *cmdline, char **argv);
void eval(char **argv, int bg);//evaluate input

//main
int main(int argc, char **argv){
	//TODO: load config file if any

	//command loop
	cmd_loop();

	//TODO: perform shutdown cleanup

	return EXIT_SUCCESS;
}

/*
*
*
*  
*   implementation
*
*
*
*
*/  

//main looping function
void cmd_loop(){
	char *line;//user input
	char *argv[MAXARGS];//user args
	int status=1;//return from execute
	int bg=0;//send to background?

	//busy loop
	do{
		printf("> ");//print the thing
		fflush(stdout);

		line = read_line();//read the line
		bg = parseline(line, argv);//parse
		eval(argv, bg);//evaluate what to do

		//free vars
		//TODO
	} while(status);

	return;
}

/*  
*   
*   reading the line input
*
*/
char *read_line(void){
	char *line = NULL;//to store input
	size_t bufsize = 0; //getline allocates a buffer
	getline(&line, &bufsize, stdin);//size: bufsize from: stdin store: line
	return line;//return it
}

/* 
 * parseline - Parse the command line and build the argv array.
 * 
 * Characters enclosed in single quotes are treated as a single
 * argument.  Return true if the user has requested a BG job, false if
 * the user has requested a FG job.  
 */
int parseline(const char *cmdline, char **argv) 
{
    static char array[MAXLINE]; /* holds local copy of command line */
    char *buf = array;          /* ptr that traverses command line */
    char *delim;                /* points to first space delimiter */
    int argc;                   /* number of args */
    int bg;                     /* background job? */

    strcpy(buf, cmdline);//copy command line into local value
    buf[strlen(buf)-1] = ' ';  /* replace trailing '\n' with space */
    while (*buf && (*buf == ' ')) /* ignore leading spaces */
	buf++;//move through white spaces

    /* Build the argv list */
    argc = 0;//init
    if (*buf == '\'') {//if it's a backslash
		buf++;//skip
		delim = strchr(buf, '\'');//delim equals backslash
    } else {
		delim = strchr(buf, ' ');
    }

    //while there is more deliminators?
    while (delim) {
		argv[argc++] = buf;
		*delim = '\0';
		buf = delim + 1;
		
		while (*buf && (*buf == ' ')) /* ignore spaces */
	       buf++;

		if (*buf == '\'') {
	    	buf++;
	    	delim = strchr(buf, '\'');
		} else {
	    	delim = strchr(buf, ' ');
		}
    }

    //sets after last arg to NULL
    argv[argc] = NULL;
    
    //blank lines
    if (argc == 0)  /* ignore blank line */
	return 1;

	//if it is just &
	if((argc == 1)&&(cmdline[0]=='&')){
		return -1;
	}

    /* should the job run in the background? */
    if ((bg = (*argv[argc-1] == '&')) != 0) {
		argv[--argc] = NULL;//sets & to NULL
    }

    return bg;//true
}

/*  
*
*
*   Evaluate Command
*   Takes: bg, 
*
*
*   
*/
void eval(char **argv, int bg){
	printf("Stuff was entered\n");

	return;
}