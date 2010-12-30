/*------------------------------------------------------------------------------

	Proyecto			: show_includes
	Codigo				: ts.cpp
	Descripcion			: 
	Version				: 0.1
	Autor				: F. Manuel Hevia Garcia
	Fecha creacion		: 30/12/2010
	Fecha modificacion	: 

	Observaciones :

			
------------------------------------------------------------------------------*/
#include "ts.h"
#include <stdio.h>
#include <stdlib.h>
/*----------------------------------------------------------------------------*/
c_ts ts;
/*----------------------------------------------------------------------------*/
c_ts::c_ts()
{
	file_name = "";
}
/*----------------------------------------------------------------------------*/
c_ts::~c_ts()
{
}
/*----------------------------------------------------------------------------*/
void c_ts::print(void)
{
	printf("  void c_ts::print()\n");
	printf("  {\n");
	t_files::iterator i1 = files.begin();
	while( i1 != files.end())
	{
		printf("    file[%s] includes:\n", ((*i1).first).c_str());

		t_files_included::iterator i2 = (*i1).second.begin();
		while( i2 != (*i1).second.end())
		{
			printf("      ->[%s]\n", ((*i2).first).c_str());
			++i2;
		}

		++i1;
	}
	printf("  }\n");
}
/*----------------------------------------------------------------------------*/
void c_ts::generate(void)
{
	FILE *f_out = NULL;
	

	f_out = fopen("out.gv","w");
	if( NULL == f_out )
	{
		printf("  error c_ts::generate() cant fopen()\n");
		return;
	}
	fprintf(f_out,"digraph defines {\n");
	fprintf(f_out,"  size=\"6,6\";\n");
	fprintf(f_out,"  node [color=lightblue2, style=filled];\n");


	printf("  void c_ts::generate()\n");
	printf("  {\n");
	t_files::iterator i1 = files.begin();
	while( i1 != files.end())
	{
//		printf("    file[%s] includes:\n", ((*i1).first).c_str());

		t_files_included::iterator i2 = (*i1).second.begin();
		while( i2 != (*i1).second.end())
		{
			printf("      ->[%s]\n", ((*i2).first).c_str());
			string s = ((*i2).first).c_str();
			if( '<' == s[0] )
			{
				fprintf(f_out,"  \"%s\" -> \"%s\";\n", ((*i1).first).c_str(), ((*i2).first).c_str() );
			}
			else
			{
				fprintf(f_out,"  \"%s\" -> %s;\n", ((*i1).first).c_str(), ((*i2).first).c_str() );
			}
			++i2;
		}

		++i1;
	}
	printf("  }\n");
	fprintf(f_out,"}\n");
	fclose(f_out);
}
/*----------------------------------------------------------------------------*/
void c_ts::file_begin(char *f)
{
	if( "" != file_name) 
	{
		printf("error c_ts::file_begin(char *f) file_name [%s] not empty\n"
			, file_name.c_str()
		);
		exit(-1);
	}

	file_name = f;
	printf(" file_begin() file_name [%s]\n",file_name.c_str());

}
/*----------------------------------------------------------------------------*/
void c_ts::file_included(char *f)
{
	files[file_name][f]=0;
}
/*----------------------------------------------------------------------------*/
void c_ts::file_end(void)
{
	if( "" == file_name) 
	{
		printf("warning c_ts::file_end() file_name empty\n");
	}
	file_name = "";
}
/*----------------------------------------------------------------------------*/

/*
dot out.gv -Tpng -o out.png

digraph unix {
	size="6,6";
	node [color=lightblue2, style=filled];
	"5th Edition" -> "6th Edition";
	"5th Edition" -> "PWB 1.0";
}
*/
