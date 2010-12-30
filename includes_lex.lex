/*------------------------------------------------------------------------------

	Proyecto			: ClientSegmentation
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
#include"web_filter_lex_yacc.h"    
    
int yylineno = 1;
%}

stringtext				([^"])|(\\.)
%%
\r\n {
		yylineno++;
		return '\n';
	}
\n {
		yylineno++;
		return '\n';
	}

[\t\f\v\r ]+ { /* Ignore whitespace. */ 
	}


"\""{stringtext}*"\"" { 
		return STRING; 
	}
"L\""{stringtext}*"\"" { 
		return STRING; 
	}
%%

int yyerror(const char *msg) 
{
  printf("%d: %s at '%s'\n",yylineno, msg, yytext);
  yydied = 1;
  return -1;
}

