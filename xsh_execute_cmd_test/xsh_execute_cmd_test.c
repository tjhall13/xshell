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

boolean error1 = FALSE;
boolean error2 = FALSE;
boolean error3 = FALSE;

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
			error1 = TRUE;
			continue;
		}
		
		buf = (char *) malloc(strlen(path) + strlen(cmd) + 2);
		
		while((pDirent = readdir(pDir)) != NULL){
			if(strcmp(pDirent->d_name, cmd) == 0){
				error2 = FALSE;
				strcpy(buf, path);
				if(path[(strlen(path) - 1)] != '/'){
					strcat(buf, "/");
					strcat(buf, cmd);
				}else{
					strcat(buf, cmd);
				}

				pid = fork();
				if(pid == 0){
					execv(buf, argv);
					exit(-1);
				}else{
					if(background == FALSE){
						waitpid(pid, &retval, 0);
						if(WEXITSTATUS(retval) == 255){
							error3 = TRUE;
						}
					}
				}
				break;
			}else{
				error2 = TRUE;
			}
		}
		closedir(pDir);
		free(buf);
	}
	if(error1){
		return -1;
	}else if(error2){
		return -2;
	}else if(error3){
		return -3;
	}else{
		return 0;
	}
}

int test1(void){
	//Test 1 (normal execution)
	printf("Executing Test 1\n");
	char *teststr = "/mnt/hgfs/XShell/xsh_execute_cmd_test";
	struct str_llist* list = (struct str_llist*) malloc(sizeof(struct str_llist));
	list->str = teststr;
	list->next = NULL;
	char * cmd = "hello.sh";
	if(xsh_execute_cmd(list, cmd, FALSE, NULL) == 0){
		printf("Passed 1\n");
	}else{
		printf("Failed 1\n");
	}
	return 0;
}

int test2(void){	
	//Test 2 (no permission to execute)
	printf("Executing Test 2\n");
	char *teststr = "/mnt/hgfs/XShell/xsh_execute_cmd_test";
	struct str_llist* list = (struct str_llist*) malloc(sizeof(struct str_llist));
	list->str = teststr;
	list->next = NULL;
	char * cmd = "hello_x.sh";
	if(xsh_execute_cmd(list, cmd, FALSE, NULL) == -3){
		printf("Passed 2\n");
	}else{
		printf("Failed 2\n");
	}
	return 0;
}

int test3(void){	
	//Test 3 (file not found)
	printf("Executing Test 3\n");
	char *teststr = "/mnt/hgfs/XShell/xsh_execute_cmd_test";
	struct str_llist* list = (struct str_llist*) malloc(sizeof(struct str_llist));
	list->str = teststr;
	list->next = NULL;
	char * cmd = "nonexistent.sh";
	if(xsh_execute_cmd(list, cmd, FALSE, NULL) == -2){
		printf("Passed 3\n");
	}else{
		printf("Failed 3\n");
	}
	return 0;
}

int main(void){
	test1();
	printf("Finished Test 1\n\n");
	test2();
	printf("Finished Test 2\n\n");
	test3();
	printf("Finished Test 3\n\n");

	return 0;
}