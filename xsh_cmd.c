#include <xsh.h>
#include <xsh_cmd.h>
#include <xsh_parse.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define PROMPT() printf("xsh :-) ");

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

void execute_task(task_desc_t *task, boolean fg, boolean refd, int write_fd, int read_fd) {
    if(task->type == PROC_T) {
        execute_proc(task->task.proc, refd, write_fd, read_fd);
    } else if(task->type == REDIRR_T) {
        execute_job(task->task.redirr.job, write_fd, read_fd);
    }
}

void execute_job(job_desc_t *job, boolean fg, boolean refd, int write_fd, int read_fd) {
    if(job->type == PIPE_T) {
        int pipe_fd[2];
        pipe(pipe_fd);
        execute_task(job->job.pipe.task, fg, TRUE, pipe_fd[0], pipe_fd[1]);
        execute_job(job->job.pipe.job, fg);
    } else if(job->type == TASK_T) {
        execute_task(job->job.task, fg, refd, write_fd, read_fd);
    }
}

static void destroy_proc(proc_desc_t *proc) {
    delete_str_llist(proc);
}

static void destroy_task(task_desc_t *task) {
    if(task == NULL) {
        return;
    }
    switch(task->type) {
    case PROC_T:
        destroy_proc(task->task.proc);
        break;
    case REDIRR_T:
        destroy_job(task->task.redirr.job);
        free(task->task.redirr.filename);
        break;
    default:
        break;
    }
    free(task);
}


void destroy_job(job_desc_t *job) {
    if(job == NULL) {
        return;
    }
    switch(job->type) {
    case TASK_T:
        destroy_task(job->job.task);
        break;
    case PIPE_T:
        destroy_task(job->job.pipe.task);
        destroy_job(job->job.pipe.job);
        break;
    default:
        break;
    }
    free(job);
}

job_desc_t *create_job_from_task(task_desc_t *task) {
    job_desc_t *ptr = (job_desc_t *) malloc(sizeof(job_desc_t));
    ptr->type = TASK_T;
    ptr->job.task = task;
    return ptr;
}

task_desc_t *create_task_from_proc(proc_desc_t *proc) {
    task_desc_t *ptr = (task_desc_t *) malloc(sizeof(task_desc_t));
    ptr->type = PROC_T;
    ptr->task.proc = proc;
    return ptr;
}

job_desc_t *pipe_task_to_job(task_desc_t *task, job_desc_t *job) {
    job_desc_t *ptr = (job_desc_t *) malloc(sizeof(job_desc_t));
    ptr->type = PIPE_T;
    ptr->job.pipe.job  = job;
    ptr->job.pipe.task = task;
    return ptr;
}

task_desc_t *redr_job_to_file(job_desc_t *job, char *filename) {
    task_desc_t *ptr = (task_desc_t *) malloc(sizeof(task_desc_t));
    ptr->type = REDIRR_T;
    ptr->task.redirr.job      = job;
    ptr->task.redirr.filename = filename;
    return ptr;
}
