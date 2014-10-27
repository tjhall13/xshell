#ifndef XSH_H_
#define XSH_H_

#include <stdio.h>

struct xsh_cntxt {
    FILE *input;
    int accept_cmd;
};

struct str_llist {
	char * str;
	struct str_llist * next;
};

int xsh_init(struct xsh_cntxt *);
int xsh_init(struct xsh_cntxt *);

#endif
