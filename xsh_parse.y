%{

#include <xsh.h>

int yyerror(char *s);
int yylex(void);

%}

%defines "xsh_parse.h"

%union {
    char *  str;
    int     ival;
    double  dval;
}

%start input

%token <str>    STRING

%%

input:      cmd_args            { print_cmd_llsit($1); }
            ;

cmd_args:   STRING cmd_args     { $$ = new_cmd_llsit($1, $2); }
            | STRING            { $$ = new_cmd_llsit($1, NULL); }
            ;

%%

int yyerror(char *str) {
    fprintf(stderr, "%s\n", str);
    return 0;
}
