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
	char line[LONG_STR]={0};

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

/*
		actual file: /d1/d2/p.cpp
		actual path: /d1/d2/

		#include "../h1.h"
		path_resolve("../h1.h");
			->/d1/h1.h
*/
void test_01(void)
{
	c_cell c1;
	c_cell c2;
	char s1[] = "/d1/d2/p.cpp";
	char s2[] = "../h1.h";

	c1.fill(s1);
	c2.fill(s2);

	printf("test_01\n");
	printf("{\n");
	printf("   file[");
	c1.print();
	printf("\n");


	printf("s2 include [%s]\n",s2);
	printf("   include[");
	c2.print();
	printf("\n");

	c1.path_resolve(c2);

	printf("   resolved[");
	c2.print();
	printf("]\n");
	

//	/d1/d2/p.cpp
	printf("}\n");
}

void tests(void)
{
	test_01();
}

int main(int argc, char* argv[])
{
	printf("show_includes project v0.0.01\n");
	printf("{\n");
//	tests();

	files_process((char *)"input.txt");
	ts.print();
	ts.generate();

	printf("}\n");
}
