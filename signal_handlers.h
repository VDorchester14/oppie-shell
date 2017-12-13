#ifndef SIGHANDLERS_H
#define SIGHANDLERS_H

//SIGNAL HANDLERS
void sigint_handler(int);
void sigchld_handler(int);
void sigstp_handler(int);
void sigalrm_handler(int);

#endif