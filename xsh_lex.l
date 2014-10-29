%{

#include <xsh_parse.h>

int yyerror(char const *);

%}

%option noyywrap

amper       [&]
char        [a-zA-Z_]
digit       [0-9]

double      {digit}*.{digit}+
integer     {digit}+

string      {char}({char}|{digit})*

%%

{string}    { yylval.str = strdup(yytext); return STRING; }

{double}    { sscanf(yytext, "%lf", &yylval.dval); return DOUBLE; }

{integer}   { sscanf(yytext, "%d", &yylval.ival); return INTEGER; }

{amper}     { yylval.cntrl = '&'; return AMPER; }

[ \t]       { }

[\n\c]      { return NEWLINE; }

.           { yyerror("unknown char"); }
