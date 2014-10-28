#include <xsh.h>
#include <xsh_cmd.h>
#include <xsh_parse.h>

#include <stdlib.h>
#include <stdio.h>

int xsh_interpret(struct xsh_cntxt *ctx) {
    yyparse();
    return 0;
}

struct cmd_llist *new_cmd_llist(char *str, struct cmd_llist *list) {
    struct cmd_llist *current = (struct cmd_llist *) malloc(sizeof(*list));
    current->str = str;
    current->next = list;
    return current;
}

void print_cmd_llist(struct cmd_llist *list) {
    printf("%s", list->str);
    if(list->next == NULL) {
        puts("\n");
    } else {
        puts(" ");
        print_cmd_llist(list->next);
    }
}
