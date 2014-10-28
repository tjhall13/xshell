#include <xsh.h>
#include <xsh_cmd.h>
#include <dirent.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>

//linked list of paths, command, size of array, array of parameters
int xsh_execute_cmd(struct str_llist * list, char * cmd, int argc, char ** argv){
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
		
		while((pDirent = readdir(pDir)) != NULL){
			char *path = ptr->str;
			if(stat(path, &sb) == 0 && sb.st_mode & S_IXUSR){
				if(strcmp(pDirent->d_name, cmd) == 0){
					pid = fork();
					if(pid == 0){
						if(path[(strlen(path) - 1)] != '/'){
							path = strcat(path, "/");
							path = strcat(path, cmd);
						}else{
							path = strcat(path, cmd);
						}
						execv(path, argv);
					}else{
						waitpid(pid, &retval, 0);
					}
				}else{
					closedir (pDir);
					break;
				}
			}else{
				closedir (pDir);
				break;
			}
		closedir (pDir);
		}
	}
	return 0;
}

int main(void){
	char *teststr = "/mnt/hgfs/XShell";
	struct str_llist* list = (struct str_llist*) malloc(sizeof(struct str_llist));
	
	list->str = teststr;
	
	char * cmd = "hello.sh";
	
	if(xsh_execute_cmd(list, cmd, 0, NULL) == 0){
		printf("Great success!\n");
	}
	
	return 0;
}