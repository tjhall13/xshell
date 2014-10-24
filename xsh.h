#ifndef XSH_H_
#define XSH_H_

#include <stdio.h>

struct xsh_cntxt {
    FILE *input;
    int accept_cmd;
};

int xsh_init(struct xsh_cntxt *);
int xsh_init(struct xsh_cntxt *);

#endif
