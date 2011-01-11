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
	lex_file_init(f);

	ts.file_begin(f);
	yyparse();
	ts.file_end();
}

void files_process(char * f_name)
{
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
		if( n_chars > 0 )
		{
			if( '#' != line[0] )
			{
				file_process(line);
			}
		}
		n_chars = fscanf(f,"%s",line);
	}

	fclose(f);
}

int main(int argc, char* argv[])
{
	printf("show_includes project v0.0.01\n");
	printf("{\n");
	files_process((char *)"input.txt");
	ts.print();
	ts.generate();
	printf("}\n");
}
