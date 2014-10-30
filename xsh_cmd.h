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

void delete_str_llist(struct str_llist *);

int size_str_llist(struct str_llist *);

void exec_str_llist(boolean, struct str_llist *);

void print_str_llist(struct str_llist *);


#endif
