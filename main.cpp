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
#include "ts.h"

void file_process(char *f)
{
//FILE *yyin;
	yyin=fopen(f,"r");
	if( NULL==yyin )
	{
		printf("  error open [%s]\n",f);
	}
	ts.file_begin(f);
	yylineno=1;
	yyparse();
	ts.file_end();
}

void files_process(char * f_name)
{
//input.txt
	FILE *f = NULL;
	char line[4024]={0};
	int n_chars = 0;

	f = fopen(f_name,"r");

	if( NULL==f )
	{
		printf("  error open [%s]\n", f_name);
		return;
	}

	n_chars = fscanf(f,"%s",line);
	while( !feof(f) )
	{

//		printf(" files_process[%s]\n",line);
		if( n_chars > 0 )
		{
			file_process(line);
		}
		n_chars = fscanf(f,"%s",line);
	}

	fclose(f);
}

int main(int argc, char* argv[])
{
	printf("show_includes project v0.0.01\n");
	printf("{\n");
//	file_process("test/t1.cpp");
	files_process((char *)"input.txt");
	ts.print();
	ts.generate();
	printf("}\n");
}
