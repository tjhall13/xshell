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
	struct stat sb;
	struct dirent *pDirent;
	DIR *pDir;
	char *path = NULL;
	
	for(ptr = list; ptr != NULL; ptr = ptr->next){
		
		pDir = opendir(ptr->str);
		if(pDir == NULL){
			return -1;
		}
		
		while((pDirent = readdir(pDir)) != NULL){
			path = ptr->str;
			printf("path: %s\n", path);
			if(stat(path, &sb) == 0 && sb.st_mode & S_IXUSR){
				printf("I'm executable\n");
				printf("directory name: %s\n", pDirent->d_name);
				printf("cmd: %s\n", cmd);
				if(strcmp(pDirent->d_name, cmd) == 0){
					printf("I matched the string\n");
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
				}
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
	list->next = NULL;
	
	char * cmd = "hello.sh";
	
	if(xsh_execute_cmd(list, cmd, 0, NULL) == 1){
		printf("Great success!\n");
	}
	
	return 0;
}