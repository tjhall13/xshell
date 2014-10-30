#include <xsh.h>
#include <xsh_cmd.h>
#include <xsh_process.h>

#include <dirent.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>

static boolean xsh_is_internal(char *cmd) {
    if(
        strcmp(cmd, "exit") == 0 ||
        strcmp(cmd, "fg") == 0   ||
        strcmp(cmd, "bg") == 0   ||
        strcmp(cmd, "jobs") == 0
    ) {
        return TRUE;
    } else {
        return FALSE;
    }
}

static int xsh_execute_internal(char *cmd, char *argv[]) {
    if(strcmp(cmd, "exit") == 0) {
        xsh_exit();
    } else if(strcmp(cmd, "jobs") == 0) {
        xsh_process_table *ptr = process_table;
        xsh_process_entry *entry;
        while(ptr != NULL) {
            printf("[%d]: %d %d", ptr->entry.entry_id, ptr->entry.pid, ptr->entry.state);
            ptr = ptr->next;
        }
    }
    return 0;
}

static int xsh_execute_external(struct str_llist * list, char *cmd, boolean fg, char *argv[]) {
	pid_t pid;
	int retval;
	struct str_llist * ptr = NULL;
	for(ptr = list; ptr != NULL; ptr = ptr->next){
		struct stat sb;
		struct dirent *pDirent;
		DIR *pDir;
		
		pDir = opendir(ptr->str);
		if(pDir == NULL){
			return -1;
		}
		
		char *path = ptr->str;
		char *buf = (char *) malloc(strlen(path) + strlen(cmd) + 2);
		buf[0] = '\0';
		buf = strcat(buf, path);
		if(buf[strlen(buf) - 1] != '/') {
		    buf = strcat(buf, "/");
		}
		
		while((pDirent = readdir(pDir)) != NULL) {
		    if(strcmp(pDirent->d_name, cmd) == 0) {
		        buf = strcat(buf, cmd);
			    if(stat(buf, &sb) == 0 && sb.st_mode & S_IXUSR) {
					pid = fork();
			        xsh_process_entry prc;
			        prc.fg = fg;
			        prc.pid = getpid();
			        
			        xsh_create_process_entry(&prc);
					if(pid <= 0) {
						execv(buf, argv);
						exit(0);
					} else {
					    if(fg) {
    						waitpid(pid, &retval, 0);
    						xsh_delete_process_entry(pid);
    				    }
					}
				}else{
					closedir (pDir);
					break;
				}
			}
		}
		
		free(buf);
	}
	return 0;
}

//linked list of paths, command, size of array, array of parameters
int xsh_execute_cmd(struct str_llist * list, char * cmd, boolean fg, char ** argv) {
    if(xsh_is_internal(cmd)) {
        return xsh_execute_internal(cmd, argv);
    } else {
        return xsh_execute_external(list, cmd, fg, argv);
    }
}
