/*------------------------------------------------------------------------------

	Proyecto            : 
	Codigo              : main.cpp
	Descripcion         : 
	Version             : 0.1
	Autor               : F. Manuel Hevia Garcia
	Fecha creacion      : 30/12/2010
	Fecha modificacion  : 

	Observaciones:
     	->

------------------------------------------------------------------------------*/
#include <stdio.h>
#include "includes_lex_yacc.h"

void file_process(char *f)
{
//FILE *yyin;
	yyin=fopen(f,"r");
	if( NULL==yyin )
	{
		printf("  error open [%s]\n",f);
	}
	yyparse();
}

int main(int argc, char* argv[])
{
	printf("show_includes project v0.0.01\n");
	printf("{\n");
	file_process("test/t1.cpp");
	printf("}\n");
}
