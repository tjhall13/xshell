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
};

extern FILE *yyin;

int xsh_init(struct xsh_cntxt *);
int xsh_init(struct xsh_cntxt *);

#endif
