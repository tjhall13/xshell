%{

#include <xsh.h>
#include <xsh_cmd.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

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
    pgrp_desc_t *pgrp;
    task_desc_t *task;
    proc_desc_t *proc;
    redir_desc_t *redir;
    redir_llist_t *redir_l;
    
    int         fd;
    double		fuckmath;
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
%token PLUS
%token MINUS
%token MULITPLY
%token DIVIDE
%token DOLLAR
%token DLBRACE
%token DRBRACE
%token LPAR
%token RPAR
%token POW

%type <str> expr
%type <fuckmath> math
%type <job> job
%type <pgrp> pgrp
%type <task> task
%type <proc> proc
%type <redir> redir
%type <redir_l> redir_l

%left PLUS
%left MINUS
%left MULTIPLY
%left DIVIDE
%right POW

%%

input:    cmd input
          |
          ;

cmd:      job AMPER NEWLINE   { execute_job($1, FALSE); destroy_job($1); }
          | job NEWLINE       { execute_job($1, TRUE); destroy_job($1); }
          | NEWLINE           { prompt(); }
          ;

redir:      REDIRR STRING     { $$ = new_redir_to_file(1, $2, FALSE); }
          | INTRED STRING     { $$ = new_redir_to_file($1, $2, FALSE); }
          | INTAPPEND STRING  { $$ = new_redir_to_file($1, $2, TRUE); }
          | INTRED AMPINT     { $$ = new_redir_to_fd($1, $2); }
          ;
          
redir_l:                      { $$ = NULL; }
          | redir redir_l     { $$ = new_redir_llist($1, $2); }
          ;

job:        pgrp REDIRL job   { $$ = pipe_job_to_pgrp($3, $1); }
          | pgrp              { $$ = create_job_from_pgrp($1); }
          ;

pgrp:       task PIPE pgrp    { $$ = pipe_task_to_pgrp($1, $3); }
          | task              { $$ = create_pgrp_from_task($1); }
          ;

task:       proc redir_l      { $$ = create_task_from_proc($1, $2); }
          ;

proc:     expr proc           { $$ = new_str_llist($1, $2); }
          | expr              { $$ = new_str_llist($1, NULL); }
          ;
          
math:	  math PLUS math			{$$ = $1 + $3;}
		 |math MINUS math			{$$ = $1 - $3;}
		 |math MULTIPLY math		{$$ = $1 * $3;}
		 |math DIVIDE math			{$$ = $1 / $3;}
		 |math POW math				{$$ = pow($1, $3);}
		 |MINUS math %prec MINUS	{$$ = -$2;}
		 |LPAR math RPAR			{$$ = $2;}
		 |expr						{double val; sscanf($1, "%lf", &val); $$ = val;} 
		 ;

expr:     STRING        { $$ = $1; }
          | DOUBLE      { $$ = strdup($1.str); }
          | INTEGER     { $$ = strdup($1.str); }
          | VARIABLE    { $$ = getenv($1); $$ = ( $$ ? $$ : "" ); $$ = strdup($$); }
          | DOLLAR DLBRACE math DRBRACE	{$$ = malloc(64); sprintf($$, "%lf", $3);}
          ;

%%

int yyerror(char const * str) {
    fprintf(stderr, "%s\n", str);
    return 0;
}
