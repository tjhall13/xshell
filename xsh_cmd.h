#ifndef XSH_CMD_H_
#define XSH_CMD_H_

#include <xsh.h>

int xsh_interpret(struct xsh_cntxt *);

<<<<<<< Updated upstream
struct str_llist {
    char *str;
    struct str_llist *next;
};

extern struct str_llist *gbl_PATH;

struct str_llist *new_str_llist(char *, struct str_llist *);

void delete_str_llist(struct str_llist *);

int size_str_llist(struct str_llist *);

void exec_str_llist(boolean, struct str_llist *);

void print_str_llist(struct str_llist *);
=======
int xsh_execute_process(char *cmd, boolean fg, char *argv[], boolean refd, int proc_stdin, int proc_stdout, int proc_stderr);

typedef enum {
    PROC_T,
    TASK_T,
    PIPE_T,
    REDIRR_T
    
} job_type;

typedef struct str_llist proc_desc_t;

struct job_desc;

typedef struct job_desc job_desc_t;

typedef struct task_desc {
    job_type type;
    union {
        proc_desc_t *proc;
        struct {
            job_desc_t *job;
            char       *filename;
        } redirr;
    } task;
} task_desc_t;

typedef struct job_desc {
    job_type type;
    union {
        task_desc_t *task;
        struct {
            task_desc_t *task;
            job_desc_t  *job;
        } pipe;
    } job;
} job_desc_t;

void execute_job(job_desc_t *, boolean);

void destroy_job(job_desc_t *);

job_desc_t *create_job_from_task(task_desc_t *);
task_desc_t *create_task_from_proc(proc_desc_t *);

job_desc_t *pipe_task_to_job(task_desc_t *, job_desc_t *);
task_desc_t *redr_job_to_file(job_desc_t *, char *);
>>>>>>> Stashed changes


#endif
