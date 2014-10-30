#include <xsh.h>
#include <xsh_cmd.h>
#include <dirent.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdlib.h>

boolean errOpeningDir = FALSE;
boolean errFindingCmd = FALSE;
boolean errExec = FALSE

int xsh_execute_cmd(struct str_llist * list, char * cmd, boolean background, char ** argv){
	
	pid_t pid;
	int retval;
	struct str_llist * ptr = NULL;
	struct dirent *pDirent;
	DIR *pDir;
	char *path = NULL;
	char *buf = NULL;
	
	for(ptr = list; ptr != NULL; ptr = ptr->next){
		path = ptr->str;
		pDir = opendir(path);
		if(pDir == NULL){
			errOpeningDir = TRUE;
			continue;
		}
		
		buf = (char *) malloc(strlen(path) + strlen(cmd) + 2);
		
		while((pDirent = readdir(pDir)) != NULL){
			if(strcmp(pDirent->d_name, cmd) == 0){
				errFindingCmd = FALSE;
				strcpy(buf, path);
				if(path[(strlen(path) - 1)] != '/'){
					strcat(buf, "/");
					strcat(buf, cmd);
				}else{
					strcat(buf, cmd);
				}
	
				pid = fork();
					
				xsh_process_entry prc;
					prc.pid = getpid;
					prc.fg = !background;
						
				if(pid == 0){
					xsh_create_process_entry(prc);
					execv(buf, argv);
					exit(-1);
				}else{
					if(background == FALSE){
						waitpid(pid, &retval, 0);
						if(WEXITSTATUS(retval) == 255){
							errExec = TRUE;
						}
						xsh_delete_process_entry(getpid);
					}
				}
				break;
			}else{
				errFindingCmd = TRUE
			}
		}
		closedir(pDir);
		free(buf);
	}
	if(errOpeningDir){
		return -1;
	}else if(errFindingCmd){
		return -2;
	}else if(errExec){
		return -3;
	}else{
		return 0;
	}
}