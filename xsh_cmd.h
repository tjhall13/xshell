#ifndef XSH_CMD_H_
#define XSH_CMD_H_

#include <xsh.h>

void prompt();

int xsh_interpret(struct xsh_cntxt *);

int xsh_execute_process(char *cmd, boolean fg, char *argv[], int proc_stdin[2], int proc_stdout[2], int proc_stderr[2]);

typedef enum {
    PROC_T,
    TASK_T,
    PGRP_T,
    PIPE_T,
    REDIRR_T
    
} job_type;

typedef struct str_llist proc_desc_t;

struct job_desc;

typedef struct job_desc job_desc_t;

typedef struct redir_desc {
    int fromfd;
    int tofd;
} redir_desc_t;

typedef struct redir_llist {
    redir_desc_t *redir;
    struct redir_llist *next;
} redir_llist_t;

typedef struct task_desc {
    proc_desc_t *proc;
    redir_llist_t *redir_l;
} task_desc_t;

typedef struct {
    int _stderr;
    int _stdout;
    int _stdin;
} io_t;

typedef struct pgrp_desc {
    job_type type;
    union {
        task_desc_t *task;
        struct {
            task_desc_t *task;
            struct pgrp_desc  *pgrp;
        } pipe;
    } pgrp;
    io_t io;
} pgrp_desc_t;

typedef struct job_desc {
    job_type type;
    union {
        pgrp_desc_t *pgrp;
        struct {
            pgrp_desc_t *pgrp;
            job_desc_t *job;
        } pipe;
    } job;
    io_t io;
} job_desc_t;

redir_llist_t *new_redir_llist(redir_desc_t *, redir_llist_t *);

redir_desc_t *new_redir_to_file(int, char *, boolean);
redir_desc_t *new_redir_to_fd(int, int);

void execute_job(job_desc_t *, boolean);

void destroy_job(job_desc_t *);

job_desc_t *create_job_from_pgrp(pgrp_desc_t *);
pgrp_desc_t *create_pgrp_from_task(task_desc_t *);
task_desc_t *create_task_from_proc(proc_desc_t *, redir_llist_t *);

pgrp_desc_t *pipe_task_to_pgrp(task_desc_t *, pgrp_desc_t *);
job_desc_t *pipe_job_to_pgrp(job_desc_t *, pgrp_desc_t *);

#endif
