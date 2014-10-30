#ifndef XSH_CMD_H_
#define XSH_CMD_H_

#include <xsh.h>
#include <sys/types.h>

int xsh_interpret(struct xsh_cntxt *);
int xsh_execute_cmd(struct str_llist *, char *, boolean, char **);



#endif
