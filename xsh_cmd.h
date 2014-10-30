#ifndef XSH_CMD_H_
#define XSH_CMD_H_

#include <xsh.h>
#include <sys/types.h>

int xsh_interpret(struct xsh_cntxt *);
int xsh_execute_cmd(struct str_llist *, char *, int, char **);

typedef enum {
	RUNNING,
	STOPPED,
	BLOCKED,
	STATE_NUM
} state_t;

typedef struct {
	int 		entry_id;
	pid_t 		pid;
	state_t 	state;
	boolean		fg;
} xsh_process_entry;

typedef struct xsh_process_table_t{
	xsh_process_entry entry;
	struct xsh_process_table_t* next;
} xsh_process_table;

extern xsh_process_table* process_table;
extern int entry_id_count;

#endif
