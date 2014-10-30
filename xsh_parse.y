%{

#include <xsh.h>
#include <xsh_cmd.h>
#include <stdlib.h>
#include <string.h>

extern FILE *yyin;

int yyerror(char const * s);
int yylex(void);

%}

%defines "xsh_parse.h"

%define parse.error verbose

%union {
    char *      str;
    char        cntrl;
    
    struct double_val  dval;
    struct int_val     ival;
    
    struct str_llist * cmd_arg_list;
}

%start input

%token <str>    STRING
%token <dval>   DOUBLE
%token <ival>   INTEGER
%token <str>    VARIABLE

%token AMPER
%token NEWLINE

%type <str> param
%type <cmd_arg_list> cmd_args

%%

input:    cmd input
          | cmd
          ;

cmd:      cmd_args NEWLINE
                         { exec_str_llist(TRUE, $1); }
          | cmd_args AMPER NEWLINE
                         { exec_str_llist(FALSE, $1); }
          | NEWLINE      { exec_str_llist(TRUE, NULL); }
          ;

cmd_args: param cmd_args { $$ = new_str_llist($1, $2); }
          | param        { $$ = new_str_llist($1, NULL); }
          ;

param:    STRING        { $$ = $1; }
          | DOUBLE      { $$ = strdup($1.str); }
          | INTEGER     { $$ = strdup($1.str); }
          | VARIABLE    { $$ = getenv($1); $$ = ( $$ ? $$ : "" ); $$ = strdup($$); }
          ;

%%

int yyerror(char const * str) {
    fprintf(stderr, "%s\n", str);
    return 0;
}
