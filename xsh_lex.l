%{

#include <xsh.h>
#include <xsh_cmd.h>
#include <xsh_parse.h>

int yyerror(char const *);

%}

%option noyywrap

amper       [&]
<<<<<<< Updated upstream
char        [a-zA-Z\-_]
=======
lessthan    [<]
greaterthan [>]
verticalbar [|]
backslash   [\]
char        [\.\/a-zA-Z\-_]
>>>>>>> Stashed changes
digit       [0-9]

double      {digit}*\.{digit}+
integer     {digit}+

<<<<<<< Updated upstream
string      {char}({char}|{digit})*
=======
string      ({digit}|{char})*{char}({digit}|{char})*

dquote_str  \".*\"
squote_str  \'.*\'
>>>>>>> Stashed changes

variable    \${char}+

comment     ^\#.*

%%

{string}    { yylval.str = strdup(yytext); return STRING; }

<<<<<<< Updated upstream
{amper}     { yylval.cntrl = '&'; return AMPER; }
=======
{dquote_str} { yylval.str = strdup(yytext); return STRING; }

{squote_str} { yylval.str = strdup(yytext); return STRING; }

{amper}     { return AMPER; }
{verticalbar} { return PIPE; }
{greaterthan} { return REDIRR; }
{lessthan}  { return REDIRL; }
>>>>>>> Stashed changes

{double}    { sscanf(yytext, "%lf", &yylval.dval.dval); yylval.dval.str = yytext; return DOUBLE; }

{integer}   { sscanf(yytext, "%d", &yylval.ival.ival); yylval.ival.str = yytext; return INTEGER; }

{variable}  { yylval.str = yytext + 1; return VARIABLE; }

[ \t]       { }

[\n\c]      { return NEWLINE; }

{comment}   { printf("comment"); }

.           { yyerror("unknown char"); }
