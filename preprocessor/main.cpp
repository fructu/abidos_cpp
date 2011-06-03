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
#include <string.h>
#include "includes_lex_yacc.h"
#include "ts.h"

void file_process(char *f)
{
	if( 0 == lex_file_init(f) )
	{
		return;
	}

	if( 1 == config.print_)
	{
		printf("    file_process()[%s]\n",f);
	}

	ts_includes.file_begin(f);

	yyparse();
	ts_includes.file_end();

	lex_file_end();
	yylex_destroy();
}

void files_process_batch(char * f_name)
{
	FILE *f = NULL;
	char line[LONG_STR]={0};

	int n_chars = 0;

	printf("  files_process_batch()\n");
	printf("  {\n");

	f = fopen(f_name,"r");

	if( NULL==f )
	{
		printf("  files_process_batchprocess..error open [%s]\n", f_name);
		return;
	}

	n_chars = fscanf(f,"%s",line);
	while( !feof(f) )
	{
		if( n_chars > 0 )
		{
			if( '#' != line[0] )
			{
				files_to_process.push(line);
			}
		}
		n_chars = fscanf(f,"%s",line);
	}

	fclose(f);

	while(!files_to_process.empty())
	{
		strcpy(line, files_to_process.pop());
		file_process(line);
	}
	printf("  }\n");
}


void file_process_one_file(char * f_name)
{
	char line[LONG_STR]={0};

	printf("  file_process_one_file()\n");
	printf("  {\n");

	files_to_process.push(f_name);
	while(!files_to_process.empty())
	{
		strcpy(line, files_to_process.pop());
		file_process(line);
	}
	printf("  }\n");
}


void test_resolve(char *s1, char *s2, char *s_result)
{
	c_cell c1;
	c_cell c2;

	c1.fill(s1, (char *) "\"");

	if( '<' == s2[0] )
	{
		c2.fill(s2, (char *) "<");
	}
	else
	{
		c2.fill(s2, (char *) "\"");
	}

	printf("  {\n");
/*
	printf("   file[");
	c1.print();
	printf("\n");

	printf("   s2 include [%s]\n",s2);
	printf("   include[");
	c2.print();
	printf("\n");
*/
	printf("   file    [%s]\n",s1);
	printf("   include [%s]\n",s2);
	c1.path_resolve(c2);

	printf("   expected[%s]\n", s_result);
	printf("   resolved[%s]\n",c2.full());
/*
	printf("   resolved[");
	c2.print();
	printf("]\n");	
*/

	if( strcmp(c2.full(),s_result) == 0 )
	{
		printf("              [ok]\n");
	}
	else
	{
		printf("              [fail]\n");
	}
	printf("  }\n");

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
	char s1[] = "/d1/d2/p.cpp";
	char s2[] = "../h1.h";
	char s3[] = "/d1/h1.h";

	printf("  test_01\n");
	test_resolve(s1, s2, s3);
}

void test_02(void)
{
/*
	c_cell c1;
	c_cell c2;
*/
	char s1[] = "Xserver/Xorg/xc/extras/Mesa/src/mesa/drivers/dri/unichrome/via_context.h";
	char s2[] = "../../../../../include/extensions/Xinerama.h";
//	char s3[] = "Xserver/Xorg/xc/include/extensions/Xinerama.h";
	char s3[] = "Xserver/Xorg/xc/extras/Mesa/include/extensions/Xinerama.h";

	printf("  test_02\n");
	test_resolve(s1, s2, s3);
}

void test_03(void)
{
	char s1[] = "test/h2.h";
	char s2[] = "h3.h";

	char s3[] = "test/h3.h";

	printf("  test_03\n");
	test_resolve(s1, s2, s3);
}

void test_04(void)
{
	char s1[] = "test/d/h2.h";
	char s2[] = "../d/../h3.h";

	char s3[] = "test/h3.h";

	printf("  test_04\n");
	test_resolve(s1, s2, s3);
}


void test_05(void)
{
	char s1[] = "test/d/h2.h";
	char s2[] = "<stdio.h>";

	char s3[] = "<stdio.h>";

	printf("  test_05\n");
	test_resolve(s1, s2, s3);
}

void tests(void)
{
	test_01();
	test_02();
	test_03();
	test_04();
	test_05();
}


void help_print(void)
{
	printf("   -batch        #the file is a list of files to process\n");
	printf("   -callers file #only process files with include file\n");
	printf("   -help         #show help options\n");
	printf("   -no-sharp     #don't process #include<...>\n");
	printf("   -follow       #try to parse the files included in actual file\n");
	printf("   -test         #run tests to check the program\n");
	printf("   -print        #show the ts and other data\n");
}


void process_command(char * command)
{
/*
		sharp  = 1;
		follow = 0;
*/
	if( strcmp("-help", command) == 0)
	{
		help_print();
		config.help = 1;
	}
	else if( strcmp("-batch", command) == 0)
	{
		config.batch = 1;
	}
	else if( strcmp("-no-sharp", command) == 0)
	{
		config.sharp = 0;
	}
	else if( strcmp("-follow", command) == 0)
	{
		config.follow = 1;
	}
	else if( strcmp("-test", command) == 0)
	{
		config.test = 1;
	}
	else if( strcmp("-print", command) == 0)
	{
		config.print_ = 1;
	}
	else if( strcmp("-callers", command) == 0)
	{
		config.callers = 2;
	}
	else
	{
		printf(" error comand unknow \n");
	}
}

void process_arg(int argc, char* argv[], char * file_name)
{
	int i = 0;
	while(i < argc)
	{
		if( '-' == argv[i][0])
		{
			process_command(argv[i]);
		}
		else
		{
			if( 2 == config.callers )
			{
				config.callers = 1;
				strcpy( config.callers_file, argv[i] );

				printf("  callers of [%s]\n",config.callers_file);
			}	
			else
			{
				strcpy( file_name, argv[i] );
			}
		}

		i++;
	}
}

int main(int argc, char* argv[])
{
	char file_name[LONG_STR]={0};

	printf("show_includes project v0.0.01\n");
	printf("{\n");

	process_arg(argc, argv, file_name);

	config.print();
	
	if( 0 == config.help)
	{
		if( 1 == config.test)
		{
			tests();
		}
		else
		{
			//     file_process("MVTVc/source/engine/mozilla/firefox-2.0.0.18/mailnews/addrbook/src/nsAbCardProperty.cpp");
			//     file_process("MVTVc/source/engine/mozilla/firefox-2.0.0.18/xpfe/components/filepicker/src/nsWildCard.cpp");
			if( 1 == config.batch )
			{
//			    files_process_batch((char *)"input.txt");
				files_process_batch(file_name);
			}
			else
			{
				file_process_one_file(file_name);
			}
			if( 1 == config.print_)
			{
			   config.print();
			   ts_includes.print();
			}
		    ts_includes.generate();
		}
	}

	printf("}\n");
}

