%{

#include <xsh.h>
#include <xsh_cmd.h>

extern FILE *yyin;

int yyerror(char const * s);
int yylex(void);

%}

%defines "xsh_parse.h"

%define parse.error verbose

%union {
    char *      str;
    char        cntrl;
    int         ival;
    double      dval;
    
    struct cmd_llist * cmd_arg_list;
}

%start input

%token <str>    STRING
%token <dval>   DOUBLE
%token <ival>   INTEGER
%token AMPER
%token NEWLINE
%type  <cmd_arg_list> cmd

%%

input:      cmd NEWLINE         { print_cmd_llist($1); YYACCEPT; }
            | cmd AMPER NEWLINE { printf("bg:\n"); print_cmd_llist($1); YYACCEPT; }
            | NEWLINE           { YYACCEPT; }
            ;

cmd:        STRING cmd          { $$ = new_cmd_llist($1, $2); }
            | STRING            { $$ = new_cmd_llist($1, NULL); }
            ;

%%

int yyerror(char const * str) {
    fprintf(stderr, "%s\n", str);
    return 0;
}
