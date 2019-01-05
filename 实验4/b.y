%{
#include  <ctype.h>
#include  <stdio.h>
#include <string.h>
#include <stdlib.h>

#define YYSTYPE char*
%}
%token  MAIN IF ELSE WHILE ID NUM LE RE EQ NQ
%left  '+'  '-'
%left  '*'  '/'
%right  UMINUS
%nonassoc then
%nonassoc ELSE
%%
program		    : MAIN '(' ')' '{' statement_list '}' {printf("program->main(){statement_list}\n");}
		    | MAIN '(' ')' '{' '}' {printf("program->main(){}\n");}
		    ;
statement_list	    : statement_list statement {printf("statement_list->statement_list statement\n");}
		    | statement {printf("statement_list->statement\n");}
		    ;
statement	    : expression_stmt {printf("statement->expression_stmt\n");}
		    | if_stmt  {printf("statement->if_stmt\n");}
		    | while_stmt {printf("statement->while_stmt\n");}
		    ;
expression_stmt	    : expression ';' {printf("expression_stmt->expression;\n");}
		    | ';' {printf("expression_stmt->;\n");}
if_stmt		    : IF '(' expression ')' statement ELSE statement {printf("if_stmt-> if ( expression ) statement else statement\n");}
		    | IF '(' expression ')' statement %prec then{printf("if_stmt->if ( expression ) statement\n");}
		    ;
while_stmt	    : WHILE '(' expression ')' statement {printf("while_stmt->while ( expression ) statement\n");}
		    ;
expression	    : var '=' expression {printf("expression->var = expression\n");}
		    | simple_expression {printf("expression->simple_expression\n");}
		    ;
var		    : ID {printf("var->ID\n");}
		    | ID '[' expression ']' {printf("var->ID [ expression ]\n");}
		    ;
simple_expression   : additive_expression relop additive_expression {printf("simple_expression->additive_expression relop additive_expression\n");}
		    | additive_expression {printf("simple_expression->additive_expression\n");}
		    ;
relop		    : LE  {printf("relop-> <=\n");}
		    | '<' {printf("relop-> <\n");}
		    | '>' {printf("relop-> >\n");}
		    | RE  {printf("relop-> >=\n");}
		    | EQ  {printf("relop-> ==\n");}
		    | NQ  {printf("relop-> !=\n");}
		    ;
additive_expression : additive_expression addop term {printf("additive_expression->additive-expression addop term\n");}
		    | term {printf("additive_expression->term\n");}
		    ;
addop		    : '+' {printf("addop-> +\n");}
		    | '-' {printf("addop-> -\n");}
		    ;
term		    : term mulop factor {printf("term->term mulop factor\n");}
		    | factor {printf("term->factor\n");}
		    ;
mulop		    : '*' {printf("mulop-> *\n");}
		    | '/' {printf("mulop-> /\n");}
		    ;
factor		    : '(' expression ')' {printf("factor->( expression ) \n");}
		    | var {printf("factor->var\n");}
		    | NUM {printf("factor->NUM\n");}
		    ;
%%
#include "lex.yy.c"

int main(int argc,char** argv)
{
   printf("Yacc Starting...\n");
   ++argv,--argc;
   if(argc>0)yyin=fopen(argv[0],"r");
   else yyin=stdin;
   yyparse();
   return 0;
}
int yyerror(char *s)
{
   fprintf(stderr,"...%s\n",s);
   return 0;
}
