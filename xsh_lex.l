%{

#include <xsh.h>
#include <xsh_cmd.h>
#include <xsh_parse.h>

int yyerror(char const *);

%}

%option noyywrap

amper       [&]
lessthan    [<]
greaterthan [>]
verticalbar [|]
backslash   [\]
char        [\.\/a-zA-Z\-_]
digit       [0-9]

double      {digit}*\.{digit}+
integer     {digit}+

string      ({digit}|{char})*{char}({digit}|{char})*

dquote_str  \".*\"
squote_str  \'.*\'

variable    \${char}+

fd          [1-2]

append      {amper}{amper}
ampint      {amper}{fd}
intred      {fd}{greaterthan}
intappend   {fd}{append}

comment     ^\#.*

%%

{dquote_str} { yylval.str = strdup(yytext); return STRING; }

{squote_str} { yylval.str = strdup(yytext); return STRING; }

{intappend} { yytext[1] = 0; yylval.fd = atoi(yytext); return INTAPPEND; }
{intred}    { yytext[1] = 0; yylval.fd = atoi(yytext); return INTRED; }
{ampint}    { yylval.fd = atoi(yytext + 1); return AMPINT; }
{append}    { return APPEND; }

{amper}     { return AMPER; }
{verticalbar} { return PIPE; }
{greaterthan} { return REDIRR; }
{lessthan}  { return REDIRL; }

{double}    { sscanf(yytext, "%lf", &yylval.dval.dval); yylval.dval.str = strdup(yytext); return DOUBLE; }

{integer}   { sscanf(yytext, "%d", &yylval.ival.ival); yylval.ival.str = strdup(yytext); return INTEGER; }

{string}    { yylval.str = strdup(yytext); return STRING; }

{variable}  { yylval.str = yytext + 1; return VARIABLE; }

[ \t]       { }

[\n]      { return NEWLINE; }

{comment}   { }

.           { yyerror("unknown char"); }
