%{

#include <xsh_parse.h>

int yyerror(char *);

%}

%option noyywrap

character   [a-zA-Z_]
digit       [0-9]

string      character{character|digit}+

%%

string  { yylval.str = strdup(yytext); return STRING; }

[ \t]*  { }

.       { yyerror("unknown char"); }
