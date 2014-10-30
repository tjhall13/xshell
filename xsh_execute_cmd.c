#include <xsh.h>
#include <xsh_cmd.h>
#include <dirent.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdlib.h>

int xsh_execute_cmd(struct str_llist * list, char * cmd, boolean background, char ** argv){
	
	pid_t pid;
	int retval;
	struct str_llist * ptr = NULL;
	struct stat sb;
	struct dirent *pDirent;
	DIR *pDir;
	char *path = NULL;
	char *buf = NULL;
	
	for(ptr = list; ptr != NULL; ptr = ptr->next){
		
		pDir = opendir(ptr->str);
		if(pDir == NULL){
			return -1;
		}
		
		path = ptr->str;
		
		buf = (char *) malloc(strlen(path) + strlen(cmd) + 2);
		
		while((pDirent = readdir(pDir)) != NULL){
			if(strcmp(pDirent->d_name, cmd) == 0){
				strcpy(buf, path);
				if(path[(strlen(path) - 1)] != '/'){
					strcat(buf, "/");
					strcat(buf, cmd);
				}else{
					strcat(buf, cmd);
				}
				if(stat(path, &sb) == 0 && sb.st_mode & S_IXUSR){
				
					xsh_process_entry prc;
						prc.pid_t = getpid;
						prc.fg = !background;
						
					pid = fork();
					if(pid == 0){
						xsh_create_process_entry(prc);
						execv(buf, argv);
					}else{
						if(background == FALSE){
							waitpid(pid, &retval, 0);
							xsh_delete_process_entry(getpid);
						}
					}
				}
				break;
			}
		}
		closedir(pDir);
		free(buf);
	}
	
	return 0;
}