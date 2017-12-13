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
#include "signal_handlers.h"

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

//Global variables
int verbose = 0;
extern char **environ;//set by libc

//job struct
struct job_t {              /* The job struct */
    pid_t pid;              /* job PID */
    int jid;                /* job ID [1, 2, ...] */
    int state;              /* UNDEF, BG, FG, or ST */
    char cmdline[MAXLINE];  /* command line */
};
struct job_t jobs[MAXJOBS]; /* The job list */
//end globals

//prototypes

//the functions
void cmd_loop(void);//main loop
char *read_line(void);//reads the user input
int parseline(const char *cmdline, char **argv);
void eval(char **argv, int bg, char *line);//evaluate input
int is_builtin_cmd(char **argv);//checks if command is a builtin command
void fg_wait(pid_t pid, sigset_t temp);//makes the foreground wait for process
//

/*
* MAIN
*/
int main(int argc, char **argv){
	//TODO: load config file if any

	//Install signal handlers
	signal(SIGINT, sigint_handler);
	signal(SIGCHLD, sigchld_handler);
	signal(SIGSTOP, sigstp_handler);
	signal(SIGALRM, sigalrm_handler);

	//command loop
	cmd_loop();

	//TODO: perform shutdown cleanup

	return EXIT_SUCCESS;
}

/*
*  
*   implementation
*
*/  

//main looping function
void cmd_loop(){
	char *prompt="oppie> ";//prompt
	char *line;//user input
	char *argv[MAXARGS];//user args
	int status=1;//return from execute
	int bg=0;//send to background?

	//busy loop
	do{
		printf("%s", prompt);//print the thing
		fflush(stdout);

		line = read_line();//read the line
		bg = parseline(line, argv);//parse
		eval(argv, bg, line);//evaluate what to do

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
	if (feof(stdin)) { /* End of file (ctrl-d) */
	    fflush(stdout);
	}
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
*   Evaluate Command
*   Takes: bg, 
*
*/
void eval(char **argv, int bg, char *line){
	//vars
	pid_t pid;//process id
	sigset_t mask, prev;//signal mask

	//ignore empties
	if(argv[0] == NULL) {
		return;
	}

	//ignore singleton &
	if(bg==-1){
		//TODO ignore singleton
		return;
	}

	//check for valid command
	if(!is_builtin_cmd(argv)){
		//blocking sigchild
		sigemptyset(&mask);//inits an empty set in mask
		sigaddset(&mask, SIGCHLD);//adds sigchld to that set
		sigprocmask(SIG_BLOCK, &mask, &prev);//block signals in mask (SIGCHLD)

		//child
		if((pid = fork()) == 0){
			setpgid(0, 0);//sets current process gid to 0
			sigprocmask(SIG_UNBLOCK, &mask, NULL);//unblocks SIGCHILD in child

			//execute program or command
			if(execve(argv[0], argv, environ) < 0 ){
				printf("%s: command not found\n", argv[0]);
				exit(0);
			}
		}

		//parent
		if(!bg){//foreground
			//add job
			sigprocmask(SIG_UNBLOCK, &mask, NULL);//unblock child
			fg_wait(pid, prev);//now wait for it to finish
		} else {//background
			sigprocmask(SIG_UNBLOCK, &mask, NULL);//unblock child
			printf("[%d] (%d) %s", 1, 2, line);
		}

	}

	return;
}

int is_builtin_cmd(char **argv){
	//exit command
	if (!strcmp(argv[0], "exit")) {
		exit(0);
        //do_exit();
        return 1;
    }
    //jobs
    else if (!strcmp(argv[0], "jobs")) {
        //do_show_jobs();
        return 1;
    }
    //bg
    else if (!strcmp(argv[0], "bg")) {
        //do_bgfg(argv);
        return 1;
    }
    //fg
    else if (!strcmp(argv[0], "fg")) {
        //do_bgfg(argv);
        return 1;
    }
    //killall
    else if (!strcmp(argv[0], "killall")) {
        //do_killall(argv);
        return 1;
    }

    //term
    else if(!strcmp(argv[0], "term")){
    	//do_remove_terminated();
    	return 1;
    }

    return 0;
}

void fg_wait(pid_t pid, sigset_t temp){
	//wait for SIGCHILD
	pid=0;
	while(!pid){
		sigsuspend(&temp);
	}
	//potentially unblock sigchild

	return;
}