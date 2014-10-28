#ifndef XSH_CMD_H_
#define XSH_CMD_H_

#include <xsh.h>

int xsh_interpret(struct xsh_cntxt *);

struct cmd_llist {
    char *str;
    struct cmd_llist *next;
};

struct cmd_llist *new_cmd_llist(char *, struct cmd_llist *);

void print_cmd_llist(struct cmd_llist *);

#endif
