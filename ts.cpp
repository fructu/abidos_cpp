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
#include <string.h>
/*
struct c_cell
{
	string path
	string name
*/
/*----------------------------------------------------------------------------*/
c_cell::c_cell()
{
	path = "";
	name = "";
}
/*----------------------------------------------------------------------------*/
c_cell::~c_cell()
{
	path = "";
	name = "";
}
/*----------------------------------------------------------------------------*/
void c_cell::init(void)
{
	path = "";
	name = "";
}
/*----------------------------------------------------------------------------*/
void c_cell::print(void)
{
	printf(" path[%s] name[%s] ", path.c_str(), name.c_str() );
}
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
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
			printf("      i2.first ->[%s]\n", ((*i2).first).c_str());
			printf("      i2.second->");
				( ((*i2).second).print() );
			printf("\n");
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
/*
	prueba/p2/f.h
			 ^   ^
			 |   |
			 p2  p1
*/
void c_ts::file_included(char *f)
{
	char s_name[1024]={0};
	char s_path[1024]={0};

	char *p1 = NULL;
	char *p2 = NULL;

	p1 = f;
	p2 = f;

	files[file_name][f].init();

	p1=strchr(f,'/');

	while (p1!=NULL)
	{
		p2 = p1;
		p1=strchr(p1+1,'/');
	}

	if( '/' == p2[0] )
	{
		strncpy(s_path,f, p2-f+1);
		strcpy(s_name,p2+1);
	}
	else
	{
		sprintf(s_path,"./");
		sprintf(s_name,"%s",f);
	}

	files[file_name][f].path = s_path;
	files[file_name][f].name = s_name;
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
