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

#include "web_filter_sintactico.h"

#define K_ERROR_UNKNOWN_TOKEN -1
#define STRING 1024

extern int yylineno;
extern FILE *yyin;

extern YYSTYPE yylval;

int  yylex(void);
void yyerror (char *s);
int  yyparse();

void yy_coger_cadena(const char* entrada);

#endif
/*----------------------------------------------------------------------------*/

