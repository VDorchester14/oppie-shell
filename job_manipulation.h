#ifndef JOBMANIP_H
#define JOBMANIP_H

/***********************************************
 * Helper routines that manipulate the job list
 **********************************************/

/* clearjob - Clear the entries in a job struct */
void clearjob(struct job_t *job);
/* initjobs - Initialize the job list */
void initjobs(struct job_t *jobs);
/* maxjid - Returns largest allocated job ID */
int maxjid(struct job_t *jobs);
/* addjob - Add a job to the job list */
int addjob(struct job_t *jobs, pid_t pid, int state, char *cmdline);
/* removejob - Delete a job whose PID=pid from the job list */
int removejob(struct job_t *jobs, pid_t pid);
/* fgpid - Return PID of current foreground job, 0 if no such job */
pid_t fgpid(struct job_t *jobs);
/* getprocessid  - Find a job (by PID) on the job list */
struct job_t *getprocessid(struct job_t *jobs, pid_t pid);
/* getjobid  - Find a job (by JID) on the job list */
struct job_t *getjobid(struct job_t *jobs, int jid);
/* get_jid_from_pid - Map process ID to job ID */
int get_jid_from_pid(pid_t pid);
/* showjobs - Print the job list */
void showjobs(struct job_t *jobs);
/******************************
 * end job list helper routines
 ******************************/
#endif