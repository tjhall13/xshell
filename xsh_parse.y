%{

#include <xsh.h>
#include <xsh_cmd.h>

extern FILE *yyin;

int yyerror(char *s);
int yylex(void);

%}

%defines "xsh_parse.h"

%union {
    char *      str;
    int         ival;
    double      dval;
    
    struct cmd_llist * cmd_arg_list;
}

%start input

%token <str>    STRING
%token NEWLINE
%type  <cmd_arg_list> cmd_args

%%

input:      cmd_args NEWLINE    { print_cmd_llist($1); }
            | NEWLINE
            ;

cmd_args:   STRING cmd_args     { $$ = new_cmd_llist($1, $2); }
            | STRING            { $$ = new_cmd_llist($1, NULL); }
            ;

%%

int yyerror(char *str) {
    fprintf(stderr, "%s\n", str);
    return 0;
}
