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
    
    job_desc_t  *job;
    task_desc_t *task;
    proc_desc_t *proc;
    redir_desc_t *redir;
    redirr_llist_t *redir_l;
    
    int         fd;
}

%start input

%token <str>    STRING
%token <dval>   DOUBLE
%token <ival>   INTEGER
%token <str>    VARIABLE

%token <fd>   AMPINT
%token <fd>   INTRED
%token <fd>   INTAPPEND


%token APPEND
%token AMPER
%token AMPRED
%token NEWLINE
%token PIPE
%token REDIRR
%token REDIRL

%type <str> expr
%type <job> job
%type <task> task
%type <proc> proc
%type <redir> redirr
%type <redir_l> redirr_l

%%

input:    cmd input
          | cmd
          ;

cmd:      job AMPER NEWLINE   { execute_job($1, FALSE); destroy_job($1); }
          | job NEWLINE       { execute_job($1, TRUE); destroy_job($1); }
          ;

redirr:     REDIRR STRING     { $$ = new_redir_to_file(1, $2, FALSE); }
          | INTRED STRING     { $$ = new_redir_to_file($1, $2, FALSE); }
          | INTAPPEND STRING  { $$ = new_redir_to_file($1, $2, TRUE); }
          | INTRED AMPINT     { $$ = new_redir_to_fd($1, $2); }
          ;
          
redirr_l:                     { $$ = NULL; }
          | redirr redirr_l   { $$ = new_redirr_llist($1, $2); }
          ;
          
job:      task                { $$ = create_job_from_task($1); }
          | task REDIRL job   { $$ = pipe_job_to_task($3, $1); }
          ;

task:     proc redirr_l         { $$ = create_task_from_proc($1, $2); }
          | task PIPE proc redirr_l  { $$ = pipe_task_to_proc($1, $3, $4); }
          ;

proc:     expr proc          { $$ = new_str_llist($1, $2); }
          | expr             { $$ = new_str_llist($1, NULL); }
          ;

expr:     STRING        { $$ = $1; }
          | DOUBLE      { $$ = strdup($1.str); }
          | INTEGER     { $$ = strdup($1.str); }
          | VARIABLE    { $$ = getenv($1); $$ = ( $$ ? $$ : "" ); $$ = strdup($$); }
          ;

%%

int yyerror(char const * str) {
    fprintf(stderr, "%s\n", str);
    return 0;
}
