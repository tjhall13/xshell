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
					if(pid <= 0) {
						execv(buf, argv);
						exit(0);
					} else {
						waitpid(pid, &retval, 0);
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

int main(void){
	char *teststr = "/home/trevor/classwork/software/xshell";
	struct str_llist* list = (struct str_llist*) malloc(sizeof(struct str_llist));
	
	list->str = teststr;
	list->next = NULL;
	
	char * cmd = "hello";
	
	if(xsh_execute_cmd(list, cmd, 0, NULL) != 0) {
	    printf("Failed\n");
	}
	
	return 0;
}
