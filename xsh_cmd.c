#include <xsh.h>
#include <xsh_cmd.h>
#include <xsh_parse.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int xsh_interpret(struct xsh_cntxt *ctx) {
    PROMPT();
    yyparse();
    return 0;
}

struct str_llist *new_str_llist(char *str, struct str_llist *list) {
    struct str_llist *current = (struct str_llist *) malloc(sizeof(*list));
    current->str = str;
    current->next = list;
    return current;
}

void delete_str_llist(struct str_llist *list) {
    if(list == NULL) {
        return;
    } else {
        free(list->str);
        delete_str_llist(list->next);
        free(list);
    }
}

int size_str_llist(struct str_llist *list) {
    struct str_llist *ptr = list;
    int size = 0;
    while(ptr != NULL) {
        size++;
        ptr = ptr->next;
    }
    return size;
}

void print_str_llist(struct str_llist *list) {
    printf("%s", list->str);
    if(list->next == NULL) {
        puts("\n");
    } else {
        puts(" ");
        print_str_llist(list->next);
    }
}

pid_t execute_proc(proc_desc_t *proc, io_t *io, boolean fg) {
    struct str_llist *list = (struct str_llist *) proc;
    int size = size_str_llist(list);
    char *argv[size+1];
    struct str_llist *ptr;
    int i = 0;
    for(ptr = list; ptr != NULL; ptr = ptr->next) {
        argv[i++] = ptr->str;
    }
    argv[size] = NULL;
    int proc_stdout[2] = {io->_stdout, 0};
    int proc_stderr[2] = {io->_stderr, 0};
    int proc_stdin[2] = {0, io->_stdin};
    return xsh_execute_process(argv[0], fg, argv, proc_stdin, proc_stdout, proc_stderr);
}

pid_t execute_task(task_desc_t *task, io_t *io, boolean fg) {
    return execute_proc(task->proc, io, fg);
}

pid_t execute_pgrp(pgrp_desc_t *pgrp, boolean fg) {
    if(pgrp->type == TASK_T) {
        return execute_task(pgrp->pgrp.task, &pgrp->io, fg);
    } else if(pgrp->type == PIPE_T) {
        return 0;
    }
}

void execute_job(job_desc_t *job, boolean fg) {
    pid_t pid; 
    if(job->type == PGRP_T) {
        pid = execute_pgrp(job->job.pgrp, fg);
    } else if(job->type == PIPE_T) {
        
    }
    xsh_wait(pid, fg);
    
    PROMPT();
}

void destroy_job(job_desc_t *job) {
    
}

redir_llist_t *new_redir_llist(redir_desc_t *redir, redir_llist_t *next) {
    redir_llist_t *ptr = (redir_llist_t *) malloc(sizeof(redir_llist_t));
    ptr->redir = redir;
    ptr->next = next;
    return ptr;
}

redir_desc_t *new_redir_to_file(int fd, char *file, boolean append) {
    FILE *f = fopen(file, append ? "a" : "w");
    return new_redir_to_fd(fd, fileno(f));
}

redir_desc_t *new_redir_to_fd(int fromfd, int tofd) {
    redir_desc_t *redir = (redir_desc_t *) malloc(sizeof(redir_desc_t));
    redir->fromfd = fromfd;
    redir->tofd = tofd;
    return redir;
}

job_desc_t *create_job_from_pgrp(pgrp_desc_t *pgrp) {
    job_desc_t *ptr = (job_desc_t *) malloc(sizeof(job_desc_t));
    ptr->type = PGRP_T;
    ptr->job.pgrp = pgrp;
    
    ptr->io._stderr = pgrp->io._stderr;
    ptr->io._stdout = pgrp->io._stdout;
    ptr->io._stdin = pgrp->io._stdin;
    return ptr;
}

pgrp_desc_t *create_pgrp_from_task(task_desc_t *task) {
    pgrp_desc_t *ptr = (pgrp_desc_t *) malloc(sizeof(task_desc_t));
    ptr->type = TASK_T;
    ptr->pgrp.task = task;
    
    ptr->io._stdin = fileno(stdin);
    ptr->io._stdout = fileno(stdout);
    ptr->io._stderr = fileno(stderr);
    
    
    return ptr;
}

task_desc_t *create_task_from_proc(proc_desc_t *proc, redir_llist_t *redir_l) {
    task_desc_t *task = (task_desc_t *) malloc(sizeof(task_desc_t));
    task->proc = proc;
    task->redir_l = redir_l;
    return task;
}

job_desc_t *pipe_job_to_pgrp(job_desc_t *job, pgrp_desc_t *pgrp) {
    job_desc_t *ptr = (job_desc_t *) malloc(sizeof(job_desc_t));
    ptr->type = PIPE_T;
    ptr->job.pipe.job  = job;
    ptr->job.pipe.pgrp = pgrp;
    
    ptr->io._stderr = fileno(stderr);
    ptr->io._stdout = fileno(stdout);
    ptr->io._stdin = fileno(stdin);
    return ptr;
}

pgrp_desc_t *pipe_task_to_pgrp(task_desc_t *task, pgrp_desc_t *pgrp) {
    pgrp_desc_t *ptr = (pgrp_desc_t *) malloc(sizeof(pgrp_desc_t));
    ptr->type = PIPE;
    ptr->pgrp.pipe.pgrp = pgrp;
    ptr->pgrp.pipe.task = task;
    
    ptr->io._stderr = fileno(stderr);
    ptr->io._stdout = fileno(stdout);
    ptr->io._stdin = fileno(stdin);
    
    return ptr;
}
