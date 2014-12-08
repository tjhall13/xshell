#include <xsh.h>
#include <xsh_cmd.h>
#include <xsh_parse.h>

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

int xsh_init(struct xsh_cntxt *ctx) {
    sh_ctx = ctx;
    
    ctx->accept_cmd = 1;
    
    yyin = ctx->input;
    
    signal(SIGINT, signal_handler);
    
    return 0;
}

int xsh_exit() {
    exit(0);
}
