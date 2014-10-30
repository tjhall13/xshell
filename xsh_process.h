#ifndef XSH_PROCESS_H_
#define XSH_PROCESS_H_

#include <xsh.h>
#include <sys/types.h>

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

boolean xsh_create_process_entry(xsh_process_entry *prc);
boolean xsh_delete_process_entry(pid_t id);
boolean xsh_update_process_entry(pid_t id, xsh_process_entry *prc);
xsh_process_entry* xsh_retrieve_process_entry_by_pid(pid_t id);
xsh_process_entry* xsh_retrieve_process_entry_by_id(int id);
xsh_process_entry* xsh_retrieve_foreground_process();

#endif
