#include <stdio.h>
#include <xsh.h>
#include <xsh_cmd.h>

struct xsh_cntxt sh_ctx;

int main(int argc, char **argv) {
    xsh_init(&sh_ctx);
    
    while(sh_ctx.accept_cmd) {
        xsh_interpret(&sh_ctx);
    }
    
    return xsh_exit(&sh_ctx);
}
