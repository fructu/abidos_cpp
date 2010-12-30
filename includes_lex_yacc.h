/*------------------------------------------------------------------------------

	Proyecto            : ClientSegmentation
	Codigo              : includes_lex_yacc.h
	Descripcion         : 
	Version             : 0.1
	Autor               : F. Manuel Hevia Garcia
	Fecha creacion      : 30/12/2010
	Fecha modificacion  : 

	Observaciones :

			
------------------------------------------------------------------------------*/
#ifndef includes_lex_yacc_h
#define includes_lex_yacc_h

#include "includes_yacc.h"

extern int yylineno;
extern FILE *yyin;

extern YYSTYPE yylval;

int  yylex(void);
void yyerror (char *s);
int  yyparse();

#endif
/*----------------------------------------------------------------------------*/

