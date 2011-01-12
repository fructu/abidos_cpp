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
/*----------------------------------------------------------------------------*/
c_cell::c_cell()
{
	path = "";
	name = "";
	delimiter = "";
}
/*----------------------------------------------------------------------------*/
c_cell::~c_cell()
{
	path = "";
	name = "";
	delimiter = "";
}
/*----------------------------------------------------------------------------*/
void c_cell::init(void)
{
	path = "";
	name = "";
	delimiter = "";
}
/*----------------------------------------------------------------------------*/
void c_cell::print(void)
{
	printf(" path[%s] name[%s] delimiter[%s]", path.c_str(), name.c_str(), delimiter.c_str() );
}
/*----------------------------------------------------------------------------*/
void c_cell::fill(char *f1)
/*
	prueba/p2/f.h
			 ^   ^
			 |   |
			 p2  p1
*/
{

	char s_name[1024]={0};
	char s_path[1024]={0};

	char f[LONG_STR]={0};

	char *p1 = NULL;
	char *p2 = NULL;

	strcpy(f,f1);

	p1 = f;
	p2 = f;

	p1=strchr(f,'\0');

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
		sprintf(s_path,PATH_ROOT);
		sprintf(s_name,"%s",f);
	}

	path = s_path;
	name = s_name;
}
/*----------------------------------------------------------------------------*/
char * c_cell::full(void)
{
	static char s[LONG_STR] = {};

	if( PATH_ROOT == path )
	{
		sprintf(s,"%s",name.c_str());
	}
	else
	{
		sprintf(s,"%s%s",path.c_str(),name.c_str());
	}

	return s;
}
/*----------------------------------------------------------------------------*/
int c_cell::get_number_dirs(void)
{
	char str[LONG_STR]={0};
	int n_dirs = 0;

	strcpy(str, path.c_str());

	char * pch;

	pch = strtok (str,"/");
	while (pch != NULL)
	{
		++n_dirs;
		
		pch = strtok (NULL, "/");
	}

	return n_dirs;
}
/*----------------------------------------------------------------------------*/
void c_cell::push_dirs(t_dir_vector & dir_vector)
{
	char str[LONG_STR]={0};
	strcpy(str, path.c_str());

	char * pch;

	pch = strtok (str,"/");
	while (pch != NULL)
	{
		dir_vector.push_back(pch);
		
		pch = strtok (NULL, "/");
	}
}
/*----------------------------------------------------------------------------*/
void c_cell::pop_dirs(t_dir_vector & dir_vector, char * str)
{
	sprintf(str,"");

	t_dir_vector::iterator i = dir_vector.begin();

	while( i != dir_vector.end() ) 
	{
		string s = *i;
		strcat(str,s.c_str());
		strcat(str,"/");

		++i;
	}
	printf(" -------------**->str[%s]\n",str);
}
/*----------------------------------------------------------------------------*/
/*
		actual file: /d1/d2/p.cpp
		actual path: /d1/d2/

		#include "../h1.h"
		path_resolve("../h1.h");
			->/d1/h1.h
*/
char * c_cell::path_resolve(c_cell cell)
{
	char str[LONG_STR]={0};
	int n_dirs     = get_number_dirs();
	int n_resolved = n_dirs;

	t_dir_vector dir_vector;

	push_dirs(dir_vector);

	strcpy(str, cell.path.c_str());
	char * pch;


	printf(" ### path_resolve [%s]\n",str);

	pch = strtok (str,"/\"");
	while (pch != NULL)
	{
		printf("  ######### pch[%s] ####", pch);
		if( strcmp(pch,"..") == 0 )
		{
			if( !dir_vector.empty() )
			{
				printf("  #########pop_back() ####");
				dir_vector.pop_back();
			}
		}
		
		pch = strtok (NULL, "/");
	}

	pop_dirs( dir_vector, str );

	return str;
}
/*
  vector<int> myvector;
  int sum (0);
  myvector.push_back (100);
  myvector.push_back (200);
  myvector.push_back (300);

  while (!myvector.empty())
  {
    sum+=myvector.back();
    myvector.pop_back();
  }

  cout << "The elements of myvector summed " << sum << endl;

  return 0;
*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
c_ts ts;
/*----------------------------------------------------------------------------*/
c_ts::c_ts()
{
	file.init();
}
/*----------------------------------------------------------------------------*/
c_ts::~c_ts()
{
	files.clear();
	all_files.clear();
}
/*----------------------------------------------------------------------------*/
void c_ts::print(void)
{
	printf("  void c_ts::print()\n");
	printf("  {\n");
	t_files_all::iterator i = all_files.begin();
	printf("    all files\n");
	printf("    {\n");
	while( i != all_files.end())
	{
		printf("      [%s][%d]\n", ((*i).first).c_str(), ((*i).second) );
		++i;
	}
	printf("    }\n");

	printf("    -----------------------\n");
	t_files::iterator i1 = files.begin();
	while( i1 != files.end())
	{
		printf("    file[%s] includes:\n", ((*i1).first).c_str());
		printf("    {\n");

		t_files_included::iterator i2 = (*i1).second.begin();
		while( i2 != (*i1).second.end())
		{
			printf("      i2.first ->[%s]", ((*i2).first).c_str());
			printf(", second->[ ");
				( ((*i2).second).print() );
			printf(" ]\n");
			++i2;
		}
		printf("    }\n");
		++i1;
	}
	printf("  }\n");
}
/*----------------------------------------------------------------------------*/
void str_drop_char(char *p_str, char c)
{
	int i1 = 0;
	int i2 = 0;
	char str[LONG_STR] = {0};

	while( '\0' != p_str[i1] )
	{
		if( c != p_str[i1] )
		{
			str[i2] = p_str[i1];
			++i2;
		}

		++i1;
	}

	strcpy(p_str, str);
}
/*----------------------------------------------------------------------------*/
void c_ts::generate(void)
{
	FILE *f_out = NULL;

	printf("  void c_ts::generate()\n");
	printf("  {\n");
	
	f_out = fopen("out.gv","w");
	if( NULL == f_out )
	{
		printf("  error c_ts::generate() cant fopen()\n");
		return;
	}

/*
	fprintf(f_out,"digraph defines {\n");
	fprintf(f_out,"  size=\"6,6\";\n");
	fprintf(f_out,"  node [color=lightblue2, style=filled];\n");
*/
	fprintf(f_out,"digraph defines {\n");
	fprintf(f_out,"  edge [label=0];\n");
	fprintf(f_out,"  graph [ranksep=0];\n");

	t_files_all::iterator i = all_files.begin();
	while( i != all_files.end() )
	{
		c_cell cell;

		string s = ((*i).first).c_str();
		char f[LONG_STR]={0};

		strcpy(f,s.c_str());

		cell.fill(f);

		char str[LONG_STR] = {0};
		char str_path[LONG_STR] = {0};
		char str_name[LONG_STR] = {0};

		strcpy(str,s.c_str());
		strcpy(str_path,cell.path.c_str());
		strcpy(str_name,cell.name.c_str());
/*
		str_drop_char(str, '"');
		str_drop_char(str_path, '"');
		str_drop_char(str_name, '"');
*/
		fprintf(f_out,"  \"%s\" [label=\"%s\\n%s\\n%s\"];\n", str, str, str_path, str_name);

		++i;
	}

	t_files::iterator i1 = files.begin();
	while( i1 != files.end())
	{
//		printf("    file[%s] includes:\n", ((*i1).first).c_str());

		t_files_included::iterator i2 = (*i1).second.begin();
		while( i2 != (*i1).second.end())
		{
			//printf("      ->[%s]\n", ((*i2).first).c_str());
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
/*
				fprintf(f_out,"\"state2\"  [ style = \"filled\" penwidth = 1 fillcolor = \"white\" fontname = \"Courier New\" shape = \"Mrecord\" label =<<table border=\"0\" cellborder=\"0\" cellpadding=\"3\" bgcolor=\"white\"><tr><td bgcolor=\"black\" align=\"center\" colspan=\"2\"><font color=\"white\">State #2</font></td></tr><tr><td align=\"left\" port=\"r4\">&#40;4&#41; l -&gt; 'n' &bull;<\"/td><td bgcolor=\"grey\" align=\"right\">=$</td></tr></table>> ];");
*/

		++i1;
	}
	printf("  }\n");
	fprintf(f_out,"}\n");
	fclose(f_out);
}
/*----------------------------------------------------------------------------*/
void c_ts::file_begin(char *f)
{
	if( "" != file.name) 
	{
		printf("error c_ts::file_begin(char *f) file_name [%s] not empty\n"
			, file.name.c_str()
		);
		exit(-1);
	}

	file.fill(f);
	printf(" file_begin() file_name [%s]\n",file.name.c_str());

	all_files[file.full()]++;
}
/*----------------------------------------------------------------------------*/
void c_ts::file_included(char *f, char c_type)
{
	c_cell cell;
	cell.init();
	cell.fill(f);
	cell.delimiter = c_type;

	if( file.path != "./")
	{
		cell.path = file.path_resolve(cell);

		printf("   file_included f[%s] resolve with [",f);
		file.print();
		printf("] -> [");
		cell.print();
		printf("]\n");
	}


	files[file.full()][cell.full()] = cell;
	all_files[cell.full()]++;


//	files[file.full()][f] = cell;

/*
	files[file.full()][f].init();
	files[file.full()][f].fill(f);
*/

//	files[file.full()][f].path = file.path_resolve(files[file.full()][f]);

//	all_files[files[file.full()][f].full()]++;

}
/*----------------------------------------------------------------------------*/
void c_ts::file_end(void)
{
	if( "" == file.name) 
	{
		printf("warning c_ts::file_end() file_name empty\n");
	}
	file.init();
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


digraph unix {
    edge [label=0];
    graph [ranksep=0];
    node [shape=record];

        "D" [label="{{D|6}|0000}"];
        "1" -> "2";
        "D" -> "3";
}

*/
