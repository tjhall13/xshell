%{

#include <xsh_parse.h>

int yyerror(char *);

%}

%option noyywrap

char        [a-zA-Z]
digit       [0-9]

string      {char}({char}|{digit})*

%%

{string}  { yylval.str = strdup(yytext); return STRING; }

[ ]+  { }

[\n\c]  { return NEWLINE; }

.       { yyerror("unknown char"); }
