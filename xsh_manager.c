#include <xsh.h>
#include <xsh_parse.h>
#include <xsh_cmd.h>

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>

static struct xsh_cntxt *sh_ctx;

static void signal_handler(int signum) {
    switch(signum) {
    case SIGINT:
        sh_ctx->accept_cmd = 0;
        break;
    default:
        break;
    }
}

static struct str_llist *parse_path() {
    struct str_llist *list = NULL;
    
    char *path = getenv("PATH");
    char *ptr = strtok(path, ":");
    while(ptr != NULL) {
        list = new_str_llist(strdup(ptr), list);
        ptr = strtok(NULL, ":");
    }
    
    return list;
}

int xsh_init(struct xsh_cntxt *ctx) {
    sh_ctx = ctx;
    
    ctx->accept_cmd = 1;
    ctx->input = stdin;
    
    yyin = ctx->input;
    
    signal(SIGINT, signal_handler);
    
    gbl_PATH = parse_path();
    
    return 0;
}

int xsh_exit(struct xsh_cntxt *ctx) {
    
    exit(0);
}
