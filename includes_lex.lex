/*------------------------------------------------------------------------------

	Proyecto			: show_includes
	Codigo				: includes_lex.lex
	Descripcion			: 
	Version				: 0.1
	Autor				: F. Manuel Hevia Garcia
	Fecha creacion		: 30/12/2010
	Fecha modificacion	: 

	Observaciones :

			
------------------------------------------------------------------------------*/
%{
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"includes_lex_yacc.h"    
    
//int yylineno = 1;
%}
/*
	if i want to continue scaning other file
	i must define yywrap and drop the next line
*/
%option noyywrap
%option yylineno 
stringtext				([^"])|(\\.)
%%
\r\n {
		yylineno++;
		//return '\n';
	}
\n {
		yylineno++;
		//return '\n';
	}

[\t\f\v\r ]+ { /* Ignore whitespace. */ 
	}

[#] {
		return '#';
	}

(include) {
		return TOKEN_INCLUDE;
	}

"\""{stringtext}*"\"" { 
		strcpy(yylval.id,yytext);
		
		return STRING; 
	}

"L\""{stringtext}*"\"" {
		strcpy(yylval.id,yytext); 
		return STRING; 
	}

"\<"{stringtext}*"\>" { 
		strcpy(yylval.id,yytext);
		return STRING; 
	}

. { 
		fprintf(stderr, "lex %d: unexpected character `%c'\n", yylineno, yytext[0]); 
	}
%%

int yyerror(const char *msg) 
{
  printf("%d: %s at '%s'\n",yylineno, msg, yytext);
  return -1;
}

