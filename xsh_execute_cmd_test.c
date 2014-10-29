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
	char buff[50];
	int execute = 0;
	
	for(ptr = list; ptr != NULL; ptr = ptr->next){
		
		pDir = opendir(ptr->str);
		if(pDir == NULL){
			return -1;
		}
		
		while((pDirent = readdir(pDir)) != NULL){
			path = ptr->str;
			if(strcmp(pDirent->d_name, cmd) == 0){
				strcpy(buff, path);
				if(path[(strlen(path) - 1)] != '/'){
					strcat(buff, "/");
					strcat(buff, cmd);
				}else{
					strcat(buff, cmd);
				}
				if(stat(buff, &sb) == 0 && ((sb.st_mode & S_IXUSR) != 0)){
					execute = 1;
					pid = fork();
					if(pid == 0){
						execv(buff, argv);
					}else{
						waitpid(pid, &retval, 0);
					}
					break;
				}else{
					break;
				}
			}
		}
		closedir(pDir);
	}
	if(execute == 0){
		printf("unable to execute\n");
	}
	return 0;
}

int main(void){
	char *teststr = "/home/ugrad/jkenney/xshell";
	struct str_llist* list = (struct str_llist*) malloc(sizeof(struct str_llist));
	
	list->str = teststr;
	list->next = NULL;
	
	char * cmd = "hello.sh";
	
	xsh_execute_cmd(list, cmd, 0, NULL);
	
	return 0;
}