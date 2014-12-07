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

void execute_job(job_desc_t *job, boolean fg) {
    if(job->type == PROC_T) {
        struct str_llist *ptr = job->job.proc;
        int size = size_str_llist(ptr);
        char *cmd[size];
        int i;
        for(i = 0; i < size; i++) {
            cmd[i] = strdup(ptr->str);
            ptr = ptr->next;
        }
        int _stdin[2];
        int _stdout[2];
        int _stderr[2];
        _stdin[0] = fileno(stdin);
        _stdin[1] = fileno(stdin);
        _stdout[0] = fileno(stdout);
        _stdout[1] = fileno(stdout);
        _stderr[0] = fileno(stderr);
        _stderr[1] = fileno(stderr);
        
        pid_t pid = xsh_execute_process(cmd[0], fg, cmd, FALSE, _stdin, _stdout, _stderr);
        xsh_wait(pid, fg);
    }
}

void destroy_job(job_desc_t *job) {
    fflush(stdout);
}

redirr_llist_t *new_redirr_llist(redir_desc_t *redir, redirr_llist_t *next) {
    redirr_llist_t *ptr = (redirr_llist_t *) malloc(sizeof(redirr_llist_t));
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

job_desc_t *create_job_from_task(task_desc_t *task) {
    job_desc_t *ptr = (job_desc_t *) malloc(sizeof(job_desc_t));
    ptr->type = TASK_T;
    ptr->job.task = task;
    ptr->io._stderr = task->io._stderr;
    ptr->io._stdout = task->io._stdout;
    ptr->io._stdin = task->io._stdin;
    return ptr;
}

task_desc_t *create_task_from_proc(proc_desc_t *proc, redirr_llist_t *redir_l) {
    task_desc_t *ptr = (task_desc_t *) malloc(sizeof(task_desc_t));
    ptr->type = PROC_T;
    ptr->task.proc = proc;
    
    redirr_llist_t *redir_ptr;
    redir_desc_t *redir;
    for(redir_ptr = redir_l; redir_ptr != NULL; redir_ptr = redir_ptr->next) {
        redir = redir_ptr->redir;
        
        int fromfd;
        switch(redir->fromfd) {
            case 0:
                fromfd = fileno(ptr->io._stdin);
                break;
            case 1:
                fromfd = fileno(ptr->io._stdout);
                break;
            case 2:
                fromfd = fileno(ptr->io._stderr);
                break;
            default:
                fromfd = -1;
                break;
        }
        
        int tofd = redir->tofd;
        if(fromfd != -1 && tofd != -1) {
            dup2(tofd, fromfd);
        }
    }
    
    return ptr;
}

job_desc_t *pipe_job_to_task(job_desc_t *job, task_desc_t *task) {
    job_desc_t *ptr = (job_desc_t *) malloc(sizeof(job_desc_t));
    ptr->type = PIPE_T;
    ptr->job.pipe.job  = job;
    ptr->job.pipe.task = task;
    
    int fd[2];
    pipe(fd);
    dup2(fd[0], task->io._stdin);
    int temp = fileno(job->io._stdout);
    dup2(fd[1], job->io._stdout);
    close(temp);
    
    ptr->io._stderr = stderr;
    ptr->io._stdout = stdout;
    ptr->io._stdin = stdin;
    return ptr;
}

task_desc_t *pipe_task_to_proc(task_desc_t *task, proc_desc_t *proc, redirr_llist_t *redir_l) {
    task_desc_t *ptr = (task_desc_t *) malloc(sizeof(task_desc_t));
    ptr->type = PIPE;
    ptr->task.pipe.task = task;
    ptr->task.pipe.proc = proc;
    
    redirr_llist_t *redir_ptr;
    redir_desc_t *redir;
    for(redir_ptr = redir_l; redir_ptr != NULL; redir_ptr = redir_ptr->next) {
        redir = redir_ptr->redir;
        
        int fromfd;
        switch(redir->fromfd) {
            case 0:
                fromfd = fileno(ptr->io._stdin);
                break;
            case 1:
                fromfd = fileno(ptr->io._stdout);
                break;
            case 2:
                fromfd = fileno(ptr->io._stderr);
                break;
            default:
                fromfd = -1;
                break;
        }
        
        int tofd = redir->tofd;
        if(fromfd != -1 && tofd != -1) {
            dup2(tofd, fromfd);
        }
    }
    
    return ptr;
}

job_desc_t *create_job_from_proc(proc_desc_t *proc) {
    job_desc_t *ptr = (job_desc_t *) malloc(sizeof(job_desc_t));
    ptr->job.proc = proc;
    ptr->type = PROC_T;
    return ptr;
}
