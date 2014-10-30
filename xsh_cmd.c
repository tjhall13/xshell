#include <xsh.h>
#include <xsh_cmd.h>
#include <xsh_parse.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define PROMPT() printf("xsh :-) ");

int xsh_interpret(struct xsh_cntxt *ctx) {
    PROMPT();
    yyparse();
    return 0;
}

struct str_llist *gbl_PATH = NULL;

struct str_llist *new_str_llist(char *str, struct str_llist *list) {
    struct str_llist *current = (struct str_llist *) malloc(sizeof(*list));
    current->str = str;
    current->next = list;
    return current;
}

void delete_str_llist(struct str_llist *list) {
    if(list == NULL) {
        return;
    } else {
        free(list->str);
        delete_str_llist(list->next);
        free(list);
    }
}

int size_str_llist(struct str_llist *list) {
    struct str_llist *ptr = list;
    int size = 0;
    while(ptr != NULL) {
        size++;
        ptr = ptr->next;
    }
    return size;
}

void exec_str_llist(boolean fg, struct str_llist *list) {
    if(list == NULL) {
        PROMPT();
        return;
    }
    int size = size_str_llist(list);
    char *argv[size + 1];
    
    struct str_llist *ptr = list;
    int n = 0;
    
    while(ptr != NULL) {
        argv[n++] = strdup(ptr->str);
        ptr = ptr->next;
    }
    argv[n] = (char *) 0;
    delete_str_llist(list);
    
    int err = xsh_execute_cmd(gbl_PATH, argv[0], fg, argv);
    PROMPT();
}

void print_str_llist(struct str_llist *list) {
    printf("%s", list->str);
    if(list->next == NULL) {
        puts("\n");
    } else {
        puts(" ");
        print_str_llist(list->next);
    }
}
