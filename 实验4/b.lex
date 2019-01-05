/* ¸¨Öú¶¨Òå */
%{
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
extern  char*  yylval;
#define N 100000 
char stack[N];
int top=0;
%}
chars [A-Za-z\_]
numbers ([0-9])+
delim [" "\r\t]
whitespace {delim}+
words {chars}({chars}|{numbers})*
kexuejishu  -?((([0-9]+)((\.[0-9]+)*[Ee](\+|\-)?([0-9]+))*)|(\.([0-9]+)[Ee](\+|\-)?[0-9]+))
%%
main         {yylval=strdup(yytext); return MAIN;}
if	     {yylval=strdup(yytext); return IF;}
else	     {yylval=strdup(yytext); return ELSE;}
while	     {yylval=strdup(yytext); return WHILE;}
"<="	     {yylval=strdup(yytext); return LE;}
">="	     {yylval=strdup(yytext); return RE;}
"=="	     {yylval=strdup(yytext); return EQ;}
"!="	     {yylval=strdup(yytext); return NQ;}
[-+*/=.,\[\]><(){};] {yylval=strdup(yytext); return *yytext;}
{words}      {yylval=strdup(yytext); return ID;  }
{whitespace} {}
{kexuejishu} {yylval=strdup(yytext); return NUM;}
.|\n         {}
%%
int yywrap()
{
  return 1;
}
