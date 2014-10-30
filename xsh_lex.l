%{

#include <xsh.h>
#include <xsh_parse.h>

int yyerror(char const *);

%}

%option noyywrap

amper       [&]
char        [\.\/a-zA-Z\-_]
digit       [0-9]

double      ({digit}*\.{digit}+)|({digit}+\.{digit}*)
integer     {digit}+

string      ({digit}|{char})*{char}({digit}|{char})*

dquote_str   \".*\"
squote_str   \'.*\'


variable    \${char}+

comment     ^\#.*

%%

{string}    { yylval.str = strdup(yytext); return STRING; }

{dquote_str} { yylval.str = strdup(yytext); return STRING; }

{squote_str} { yylval.str = strdup(yytext); return STRING; }

{amper}     { yylval.cntrl = '&'; return AMPER; }

{double}    { sscanf(yytext, "%lf", &yylval.dval.dval); yylval.dval.str = yytext; return DOUBLE; }

{integer}   { sscanf(yytext, "%d", &yylval.ival.ival); yylval.ival.str = yytext; return INTEGER; }

{variable}  { yylval.str = yytext + 1; return VARIABLE; }

[ \t]       { }

[\n\c]      { return NEWLINE; }

{comment}   { printf("comment"); }

.           { yyerror("unknown char"); }
