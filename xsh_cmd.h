#ifndef XSH_CMD_H_
#define XSH_CMD_H_

#include <xsh.h>

int xsh_interpret(struct xsh_cntxt *);

int xsh_execute_process(char *cmd, boolean fg, char *argv[], boolean refd, int proc_stdin, int proc_stdout, int proc_stderr);

typedef enum {
    PROC_T,
    TASK_T,
    PIPE_T,
    REDIRR_T,
    REDIRL_T
    
} job_type;

typedef struct str_llist proc_desc_t;

struct job_desc;

typedef struct job_desc job_desc_t;

typedef struct task_desc {
    job_type type;
    union {
        proc_desc_t *proc;
        struct {
            proc_desc_t *proc;
            struct task_desc  *task;
        } pipe;
    } task;
    struct {
        FILE *_stderr;
        FILE *_stdout;
        FILE *_stdin;
    } io;
} task_desc_t;

typedef struct job_desc {
    job_type type;
    union {
        task_desc_t *task;
        struct {
            task_desc_t *task;
            job_desc_t *job;
        } pipe;
    } job;
    struct {
        FILE *_stderr;
        FILE *_stdout;
        FILE *_stdin;
    } io;
} job_desc_t;

typedef struct redir_desc {
    int fromfd;
    int tofd;
} redir_desc_t;

typedef struct redirr_llist {
    redir_desc_t *redir;
    struct redirr_llist *next;
} redirr_llist_t;

redirr_llist_t *new_redirr_llist(redir_desc_t *, redirr_llist_t *);

redir_desc_t *new_redir_to_file(int, char *, boolean);
redir_desc_t *new_redir_to_fd(int, int);

void execute_job(job_desc_t *, boolean);

void destroy_job(job_desc_t *);

job_desc_t *create_job_from_task(task_desc_t *);
task_desc_t *create_task_from_proc(proc_desc_t *, redirr_llist_t *);

task_desc_t *pipe_task_to_proc(task_desc_t *, proc_desc_t *, redirr_llist_t *);
job_desc_t *pipe_job_to_task(job_desc_t *, task_desc_t *);

#endif
