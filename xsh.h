#ifndef XSH_H_
#define XSH_H_

#include <stdio.h>

typedef enum {
    FALSE,
    TRUE
} boolean;

struct xsh_cntxt {
    FILE *input;
    int accept_cmd;
    boolean script;
};

extern FILE *yyin;

int xsh_init(struct xsh_cntxt *);
int xsh_init();

struct double_val {
    double dval;
    char *str;
};

struct int_val {
    int ival;
    char *str;
};

struct str_llist {
    char *str;
    struct str_llist *next;
};

struct str_llist *new_str_llist(char *, struct str_llist *);

void delete_str_llist(struct str_llist *);

int size_str_llist(struct str_llist *);

void print_str_llist(struct str_llist *);

#endif
