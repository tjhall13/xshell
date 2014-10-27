#include <xsh.h>
#include <xsh_cmd.h>
#include <dirent.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

//linked list of paths, command, size of array, array of parameters
int xsh_execute_cmd(struct str_llist * list, char * cmd, int argc, char ** argv){
	pid_t pid;
	int retval;
	struct str_llist * ptr = NULL;
	for(ptr = list; ptr != NULL; ptr = ptr->next){
		struct dirent *pDirent;
		DIR *pDir;
		
		pDir = opendir(ptr->str);
		if(pDIr == NULL){
			return -1;
		}
		
		while((pDirent = readdir(pDir)) != NULL){
			//check if executable
			if(strcmp(pDirent->d_name, cmd) == 0){
				pid = fork();
				if(pid == 0){
					char *path = ptr->str; //ptr->str gives current string
					if(path[(strlen(ptr->str) - 1)] != '/'){
						path = strcat(path, "/");
						path = strcat(path, cmd);
					}else{
						path = strcat(path, cmd);
					}
					execv(path, argv);
				}else{
					waitpid(pid, $retval, 0);
				}
			}
		}
		closedir (pDir);
	}
};