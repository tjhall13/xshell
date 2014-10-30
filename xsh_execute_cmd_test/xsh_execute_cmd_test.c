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

int can_errno;

int xsh_execute_cmd(struct str_llist * list, char * cmd, boolean background, char ** argv){
	
	pid_t pid;
	int retval;
	struct str_llist * ptr = NULL;
	struct stat sb;
	struct dirent *pDirent;
	DIR *pDir;
	char *path = NULL;
	char *buf = NULL;
	can_errno = 0;
	
	for(ptr = list; ptr != NULL; ptr = ptr->next){
		path = ptr->str;
		pDir = opendir(path);
		if(pDir == NULL){
			can_errno = -1;
			continue;
		}
		
		buf = (char *) malloc(strlen(path) + strlen(cmd) + 2);
		
		while((pDirent = readdir(pDir)) != NULL){
			if(strcmp(pDirent->d_name, cmd) == 0){
				can_errno = 0;
				strcpy(buf, path);
				if(path[(strlen(path) - 1)] != '/'){
					strcat(buf, "/");
					strcat(buf, cmd);
				}else{
					strcat(buf, cmd);
				}
				if(stat(buf, &sb) == 0 && ((sb.st_mode & S_IXUSR) != 0)){
					pid = fork();
					if(pid == 0){
						execv(buf, argv);
					}else{
						if(background == FALSE){
							waitpid(pid, &retval, 0);
						}
					}
				}else{
					can_errno = -3;
				}
				break;
			}else{
				can_errno = -2;
			}
		}
		closedir(pDir);
		free(buf);
	}
	if(can_errno < 0){
		return -1;
	}else{
		return 0;
	}
}

int main(void){
	//Test 1 (normal execution)
	printf("Executing Test 1\n");
	char *teststr = "/mnt/hgfs/XShell/xsh_execute_cmd_test";
	struct str_llist* list = (struct str_llist*) malloc(sizeof(struct str_llist));
	list->str = teststr;
	list->next = NULL;
	char * cmd = "hello.sh";
	if(xsh_execute_cmd(list, cmd, FALSE, NULL) == 0){
		printf("Passed 1\n\n");
	}else{
		printf("Failed 1: %d\n\n", can_errno);
	}
	
	//Test 2 (no permission to execute)
	printf("Executing Test 2\n");
	cmd = "hello_x.sh";
	if(xsh_execute_cmd(list, cmd, FALSE, NULL) == -1 && can_errno == -3){
		printf("Passed 2\n\n");
	}else{
		printf("Failed 2: %d\n\n", can_errno);
	}
	
	//Test 3 (file not found)
	printf("Executing Test 3\n");
	cmd = "nonexistent.sh";
	if(xsh_execute_cmd(list, cmd, FALSE, NULL) == -1 && can_errno == -2){
		printf("Passed 3\n\n");
	}else{
		printf("Failed 3: %d\n\n", can_errno);
	}

	return 0;
}