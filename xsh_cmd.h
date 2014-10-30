#ifndef XSH_CMD_H_
#define XSH_CMD_H_

#include <xsh.h>

int xsh_interpret(struct xsh_cntxt *);

struct str_llist {
    char *str;
    struct str_llist *next;
};

extern struct str_llist *gbl_PATH;

struct str_llist *new_str_llist(char *, struct str_llist *);

void print_str_llist(struct str_llist *);

int xsh_execute_cmd(struct str_llist * list, char * cmd, boolean bg, char ** argv);

#endif
