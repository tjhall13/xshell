#include <xsh.h>
#include <xsh_cmd.h>
#include <xsh_process.h>

#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>

static int xsh_cd(char *argv[]);
static int xsh_jobs();

static boolean xsh_is_internal(char *cmd) {
    if(
        strcmp(cmd, "exit") == 0 ||
        strcmp(cmd, "fg") == 0   ||
        strcmp(cmd, "bg") == 0   ||
        strcmp(cmd, "jobs") == 0 ||
        strcmp(cmd, "cd") == 0
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
        xsh_jobs();
    } else if(strcmp(cmd, "cd") == 0) {
        xsh_cd(argv);
    }
}

static int xsh_execute_external(char *cmd, char *argv[]) {
    // execute cmd searching in $PATH with argv
    execvp(cmd, argv);
    
    // program is still continuing, an error occured
    switch(errno) {
    case EACCES:
        fprintf(stderr, "xsh: %s: Permission denied\n", cmd);
        break;
    case ENOEXEC:
        fprintf(stderr, "xsh: %s: Was not a recognizable executable\n", cmd);
        break;
    default:
        fprintf(stderr, "xsh: %s: Error executing file\n", cmd);
        break;
    }
    return -1;
}

// command, foreground, array of parameters
int xsh_execute_process(char * cmd, boolean fg, char ** argv, boolean refd, int proc_stdin[2], int proc_stdout[2], int proc_stderr[2]) {
    /* If is a foreground process, is an internal command and file descriptors do not need to be managed, execute internal command on this process
    */
    if(fg && xsh_is_internal(cmd) && !refd) {
        int retval = xsh_execute_internal(cmd, argv);
        
        return retval;
    } else {
        pid_t pid = fork();
        if(pid == 0) {
            
            if(refd) {
                // close writing end of pipe
                if(fcntl(proc_stdin[0], F_GETFD) != -1) {
                    close(proc_stdin[0]);
                }
                if(fcntl(proc_stdout[1], F_GETFD) != -1) {
                    close(proc_stdout[1]);
                }
                if(fcntl(proc_stderr[1], F_GETFD) != -1) {
                    close(proc_stderr[1]);
                }
                
                // set stdin/stdout/stderr to the new values
                dup2(proc_stdin[1], fileno(stdin));
                dup2(proc_stdout[0], fileno(stdout));
                dup2(proc_stderr[0], fileno(stderr));
            }
            
            if(xsh_is_internal(cmd)) {
                xsh_execute_internal(cmd, argv);
            } else {
                xsh_execute_external(cmd, argv);
            }
            exit(0);
        } else if(pid < 0) {
            fprintf(stderr, "xsh: Could not spawn a new process\n");
            return -1;
        } else {
            if(!fg) {
                // Add job entry
            }
            return pid;
        }
    }
}

int xsh_wait(pid_t pid, boolean fg) {
    int status = 0;
    pid_t state_pid = waitpid(pid, &status, WNOHANG | WUNTRACED | WCONTINUED);
    int retval, sigval;
    boolean fg_running = TRUE;
    while(state_pid != 0) {
        if(state_pid == pid) {
            fg_running = FALSE;
        } else if(state_pid < 0) {
            if(errno == ECHILD) {
                // Why are there no children?
                break;
            }
        } else if(WIFEXITED(status)) {
            retval = WEXITSTATUS(status);
            // Update Table
        } else if(WIFSIGNALED(status)) {
            sigval = WTERMSIG(status);
            // Update Table
        } else if(WIFSTOPPED(status)) {
            sigval = WSTOPSIG(status);
            switch(sigval) {
            case SIGSTOP:
                // Update Table
                break;
            case SIGTSTP:
                // Update Table
                break;
            default:
                break;
            }
        #ifdef WCOREDUMP
        } else if(WCOREDUMP(status)) {
            // Update Table
            // Print to user
            
        #endif
        }
    }
    
    if(fg && fg_running) {
        waitpid(pid, &status, 0);
        // Print job status changes
    }
    
    return 0;
}

int xsh_cd(char** argv) {
    char path[200];
    strcpy(path,argv[1]);

    char *token;

    char cwd[200];
    if(argv[1][0] != '/')
    {// true for the dir in cwd
        getcwd(cwd,sizeof(cwd));
        strcat(cwd,"/"); 
        strcat(cwd,path);
        chdir(cwd);
    } else {//true for dir w.r.t. /
        chdir(argv[1]);
    }

    return 0;
}
int xsh_jobs() {
    
    return 0;
}
