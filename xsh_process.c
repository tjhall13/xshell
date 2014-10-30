#include <xsh.h>
#include <xsh_process.h>
#include <stdlib.h>

xsh_process_table* process_table = NULL;
int entry_id_count = 0;

boolean xsh_create_process_entry(xsh_process_entry *prc) {
	prc->entry_id = entry_id_count++;
	xsh_process_table *tbl = (xsh_process_table*) malloc(sizeof (xsh_process_table));
	tbl->entry =  *prc;
	if(process_table == NULL) {
		process_table = tbl;
	} else {
		xsh_process_table* current = process_table;
		while(current->next != NULL) {
			current = current->next;
		}
		current->next = tbl;
	}
	return TRUE;
}

boolean xsh_delete_process_entry(pid_t id) {
	boolean exists = FALSE;
	boolean is_head = FALSE;
	xsh_process_table* current = process_table;
	xsh_process_table* node_to_delete;
	xsh_process_table* prev_node;
	if(current->entry.pid == id) {
		exists = TRUE;
		node_to_delete = current;
		is_head = TRUE;
	} else {
		xsh_process_table* prev_node;
		while(current->next != NULL) {
		if(current->next->entry.pid == id) {
				exists = TRUE;
				prev_node = current;
				node_to_delete = current->next;
			}
			current = current->next;
		}
	}	exists = TRUE;
	if(exists) {
		if(is_head) {
			process_table = node_to_delete->next;
		} else {
			prev_node->next = node_to_delete->next;
		}
		free(node_to_delete);
	}
	return exists;
}

boolean xsh_update_process_entry(pid_t id, xsh_process_entry *prc) {
	boolean exists = FALSE;
	xsh_process_table* current = process_table;
	xsh_process_entry* process_to_update;
	while(current->next != NULL) {
		if(current->entry.pid == id) {
			process_to_update = &current->entry;
			exists = TRUE;
			break;
		}
	}

	if(exists) {
		process_to_update->state = prc->state;
		process_to_update->fg = prc->fg;
	}
	return exists;
}

xsh_process_entry* xsh_retrieve_process_entry_by_pid(pid_t id) {
	boolean exists = FALSE;
	xsh_process_table* current = process_table;
	xsh_process_entry* process_to_retrieve;
	while(current->next != NULL) {
		if(current->entry.pid == id) {
			process_to_retrieve = &current->entry;
			exists = TRUE;
			break;
		}
		current = current->next;
	}
	if(exists) {
		return process_to_retrieve;
	} else {
		return NULL;
	}
}

xsh_process_entry* xsh_retrieve_process_entry_by_id(int id) {
	boolean exists = FALSE;
	xsh_process_table* current = process_table;
	xsh_process_entry* process_to_retrieve;
	while(current->next != NULL) {
		printf("%d\n",current->next->entry.entry_id);
		if(current->entry.entry_id == id) {
			process_to_retrieve = &current->entry;
			exists = TRUE;
			break;
		}
		current = current->next;
	}
	if(exists) {
		return process_to_retrieve;
	} else {
		return NULL;
	}
}

xsh_process_entry* xsh_retrieve_foreground_process() {
	boolean exists = FALSE;
	xsh_process_table* current = process_table;
	xsh_process_entry* process_to_retrieve;
	while(current->next != NULL) {
		if(current->entry.fg) {
			process_to_retrieve = &current->entry;
			exists = TRUE;
			break;
		}
	}
	if(exists) {
		return process_to_retrieve;
	} else {
		return NULL;
	}
}
