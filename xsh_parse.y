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

input:      cmd_args;

cmd_args:   STRING cmd_args
            | STRING
            ;

%%

int yyerror(char *str) {
    fprintf(stderr, "%s\n", str);
    return 0;
}
