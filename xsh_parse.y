%{

#include <xsh.h>
#include <xsh_cmd.h>
#include <stdlib.h>

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
    
    struct str_llist * cmd_arg_list;
}

%start input

%token <str>    STRING
%token <dval>   DOUBLE
%token <ival>   INTEGER
%token AMPER
%token NEWLINE
%type  <cmd_arg_list> cmd_args

%%

input:    cmd input
          | cmd
          ;

cmd:      cmd_args NEWLINE
                            { exec_str_llist(TRUE, $1); }
          | cmd_args AMPER NEWLINE
                            { exec_str_llist(FALSE, $1); }
          ;

cmd_args: STRING cmd_args   { $$ = new_str_llist($1, $2); }
          | STRING          { $$ = new_str_llist($1, NULL); }
          ;

%%

int yyerror(char const * str) {
    fprintf(stderr, "%s\n", str);
    return 0;
}
