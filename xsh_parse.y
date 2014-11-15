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
}

%start input

%token <str>    STRING
%token <dval>   DOUBLE
%token <ival>   INTEGER
%token <str>    VARIABLE

%token AMPER
%token NEWLINE
%token PIPE
%token REDIRR
%token REDIRL

%type <str> param
%type <job> job
%type <task> task;
%type <proc> proc;

%%

input:    cmd input
          | cmd
          ;

cmd:      job AMPER NEWLINE   { execute_job($1, FALSE); destroy_job($1); }
          | job NEWLINE       { execute_job($1, TRUE); destroy_job($1); }
          ;

job:      task                { $$ = create_job_from_task($1); }
          | task PIPE job     { $$ = pipe_task_to_job($1, $3); }
          | job REDIRL task   { $$ = pipe_task_to_job($3, $1); }
          ;

task:     proc                { $$ = create_task_from_proc($1); }
          | job REDIRR STRING { $$ = redr_job_to_file($1, $3); }
          ;

proc:     param proc          { $$ = new_str_llist($1, $2); }
          | param             { $$ = new_str_llist($1, NULL); }
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
