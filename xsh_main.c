#include <stdio.h>
#include <xsh.h>
#include <xsh_cmd.h>

struct xsh_cntxt sh_ctx;

int main(int argc, char **argv) {
    if(argc > 1) {
        char *filename = argv[1];
        FILE *file = fopen(filename, "r");
        sh_ctx.input = file;
        sh_ctx.script = TRUE;
    } else {
        sh_ctx.input = stdin;
    }
    
    xsh_init(&sh_ctx);
    
    xsh_interpret(&sh_ctx);
    
    return xsh_exit(&sh_ctx);
}
