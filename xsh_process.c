#include <xsh.h>
#include <xsh_process.h>
#include <stdlib.h>

int xsh_init_job_table(xsh_job_table_t *table) {
    table->job_entries = NULL;
    table->num_jobs = 0;
    return 0;
}

static jobs_num = 1;

xsh_job_t *xsh_create_job_entry(char *cmd, pid_t pgid) {
    xsh_job_t *ptr = (xsh_job_t *) malloc(sizeof(xsh_job_t));
    ptr->cmd = cmd;
    ptr->pgid = pgid;
    ptr->jobid = jobs_num++;
    ptr->next = NULL;
    
    return ptr;
}

int xsh_insert_job_entry(xsh_job_table_t *table, xsh_job_t *job) {
    xsh_job_t *ptr, *current;
    for(ptr = table->job_entries; ptr != NULL; ptr = ptr->next) {
        current = ptr;
    }
    current->next = job;
    return 0;
}

xsh_job_t *xsh_remove_job_entry_by_jobid(xsh_job_table_t *table, int jobid) {
    xsh_job_t *ptr, *current;
    for(ptr = table->job_entries; ptr != NULL; ptr = ptr->next) {
        if(ptr->jobid == jobid) {
            current->next = ptr->next;
            return ptr;
        }
        current = ptr;
    }
    return NULL;
}

xsh_job_t *xsh_remove_job_entry_by_pgid(xsh_job_table_t *table, int pgid) {
    xsh_job_t *ptr, *current;
    for(ptr = table->job_entries; ptr != NULL; ptr = ptr->next) {
        if(ptr->pgid == pgid) {
            current->next = ptr->next;
            return ptr;
        }
        current = ptr;
    }
    return NULL;
}

xsh_job_t *xsh_get_job_entry_by_jobid(xsh_job_table_t *table, int jobid) {
    xsh_job_t *ptr, *current;
    for(ptr = table->job_entries; ptr != NULL; ptr = ptr->next) {
        if(ptr->jobid == jobid) {
            return ptr;
        }
        current = ptr;
    }
    return NULL;
}

xsh_job_t *xsh_get_job_entry_by_pgid(xsh_job_table_t *table, int pgid) {
    xsh_job_t *ptr, *current;
    for(ptr = table->job_entries; ptr != NULL; ptr = ptr->next) {
        if(ptr->pgid == pgid) {
            return ptr;
        }
        current = ptr;
    }
    return NULL;
}
