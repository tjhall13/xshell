#ifndef XSH_PROCESS_H_
#define XSH_PROCESS_H_

#include <xsh.h>
#include <sys/types.h>

typedef enum {
	RUNNING,
	STOPPED,
	COMPLETED,
	STATE_NUM
} state_t;

typedef struct xsh_process {
	pid_t 		pid;
	state_t 	state;
	char *      cmd;
	int         status;
	
	struct xsh_process *next;
} xsh_process_t;

typedef struct xsh_job {
    char *      cmd;
    pid_t       pgid;
    int         jobid;
    xsh_process_t *proc_llist;
    
    struct xsh_job *next;
} xsh_job_t;

typedef struct xsh_job_table {
    xsh_job_t   *job_entries;
    int         num_jobs;
} xsh_job_table_t;

extern xsh_job_table_t *job_table;

int xsh_init_job_table(xsh_job_table_t *table);
int xsh_insert_job_entry(xsh_job_table_t *table, xsh_job_t *job);
xsh_job_t *xsh_remove_job_entry_by_jobid(xsh_job_table_t *table, int jobid);
xsh_job_t *xsh_remove_job_entry_by_pgid(xsh_job_table_t *table, int pgid);
xsh_job_t *xsh_get_job_entry_by_jobid(xsh_job_table_t *table, int jobid);
xsh_job_t *xsh_get_job_entry_by_pgid(xsh_job_table_t *table, int pgid);

#endif
