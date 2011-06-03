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
//#include "includes_lex_yacc.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*----------------------------------------------------------------------------*/
c_config config;
c_files_to_process files_to_process;
/*
class c_files_to_process
{
	private:
		t_files_to_process files_to_process;
		t_files_included   all_files

	public:
		void push(char *);
		char * pop(void);
};
*/
/*----------------------------------------------------------------------------*/
bool c_files_to_process::empty(void)
{
	return files_to_process.empty();
}
/*----------------------------------------------------------------------------*/
void c_files_to_process::push(char * file)
{
	if(all_files.find(file) != all_files.end())
	{
		return;
	}

	c_cell cell = ts_includes.resolve(file, (char *)"\"");

	files_to_process.push_back(cell.full());
	all_files[file]=1;

/*
	files_to_process.push_back(file);
	all_files[file]=1;
*/
}
/*----------------------------------------------------------------------------*/
char * c_files_to_process::pop(void)
{
	if( files_to_process.empty() )
	{
		return 0;
	}
	else
	{
		strcpy(str, files_to_process.back().c_str());
		files_to_process.pop_back();
	}
	return str;
}
/*----------------------------------------------------------------------------*/
c_cell::c_cell()
{
	path = "";
	name = "";
	delimiter = "";
	str_full[0]='\0';
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
	show = 1;

}
/*----------------------------------------------------------------------------*/
char * c_cell::get_path()
{
	strcpy(str_path, path.c_str());

	return str_path;
}
/*----------------------------------------------------------------------------*/
char * c_cell::get_delimiter()
{
	strcpy(str_delimiter, delimiter.c_str());

	return str_delimiter;
}
/*----------------------------------------------------------------------------*/
void c_cell::path_set(char str[])
{
	path=str;
}
/*----------------------------------------------------------------------------*/
char * c_cell::get_name()
{
	strcpy(str_name, name.c_str());

	return str_name;
}
/*----------------------------------------------------------------------------*/
void c_cell::print(void)
{
	printf(" path[%s] name[%s] delimiter[%s]", path.c_str(), name.c_str(), delimiter.c_str() );
}
/*----------------------------------------------------------------------------*/
void c_cell::fill(const char *f1, char * c_type)
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

	delimiter = c_type;

	p1 = f;
	p2 = f;

	p1=strchr(p1,'/');

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
	if( PATH_ROOT == path )
	{
		sprintf(str_full,"%s",name.c_str());
	}
	else
	{
		sprintf(str_full,"%s%s",path.c_str(),name.c_str());
	}

	return str_full;
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

	if( '/' == str[0])
	{
		dir_vector.push_back("/");
	}

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
	str[0]='\0';

	t_dir_vector::iterator i = dir_vector.begin();

	while( i != dir_vector.end() )
	{
		string s = *i;
		strcat(str,s.c_str());

		if( strcmp("/", s.c_str()) != 0)
		{
			strcat(str,"/");
		}

		++i;
	}
}
/*----------------------------------------------------------------------------*/
int c_cell::is_sharp(void)
{
	if( "<" == delimiter)
	{
		return 1;
	}

	return 0;
}
/*----------------------------------------------------------------------------*/
/*
	answerd to quiestion
		resolve or not?
*/
int c_cell::is_resolve(c_cell cell)
{
	 char str[LONG_STR]={0};

	int resolve = 0;

	if( 1 == cell.is_sharp() )
	{
		printf("  ##[");
		cell.print();
		printf("] -> no resolved");
		return 0;
	}

	strcpy(str, cell.path.c_str());
	char * pch;

	pch = strtok (str,"/");
	while (pch != NULL)
	{
		if( strcmp(pch,"..") == 0 )
		{
			return 1;
		}

		pch = strtok (NULL, "/");
	}

	return resolve;
}
/*----------------------------------------------------------------------------*/
/*
		actual file: /d1/d2/p.cpp
		actual path: /d1/d2/

		#include "../h1.h"
		path_resolve("../h1.h");
			->/d1/h1.h
*/
void c_cell::path_resolve(c_cell & cell)
{
//	int n_dirs     = get_number_dirs();
    char str[LONG_STR]={0};
	int n_resolved = 0;
	c_cell cell_old = cell;


	if( 1 == cell.is_sharp() )
	{
		return;
	}

	t_dir_vector dir_vector;

	push_dirs(dir_vector);

	strcpy(str, cell.path.c_str());
	char * pch;

	pch = strtok (str,"/");
	while (pch != NULL)
	{
		if( strcmp(pch,"..") == 0 )
		{
			if( !dir_vector.empty() )
			{
				n_resolved = 1;
				dir_vector.pop_back();
			}
		}
		else
		{
			if( strcmp(pch,".") != 0 )
			{
				dir_vector.push_back(pch);
			}
		}

		pch = strtok (NULL, "/");
	}

	pop_dirs( dir_vector, str );

	if( 1 == n_resolved)
	{
		/*
		printf("    resolved host[%s]: [%s]->[%s]\n"  , full(), cell.path.c_str(), str);
		printf("    resolved host[%s]: [%s]->[%s%s]\n", full(), cell.full()      , str, cell.get_name());
		*/
		printf("    resolved host[%s]:", full());
		printf(" [%s]->[%s%s]\n", cell.full(), str, cell.get_name());
	}

	cell.path_set(str);
}
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
c_ts ts_includes;
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
		printf("      [%s][%s]\n", ((*i).first).c_str(), ((*i).second).get_delimiter() );
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
//	fprintf(f_out,"  size=\"20,6\";\n");
//	fprintf(f_out,"  node [color=lightblue2, style=filled, nodesep=100];\n");
//	fprintf(f_out,"  edge [label=0];\n");
	fprintf(f_out,"  graph [ranksep=0];\n");


	t_files_all::iterator i = all_files.begin();
	while( i != all_files.end() )
	{
		c_cell cell;
		cell = (*i).second;

		string s = ((*i).first).c_str();
		char f[LONG_STR]={0};

		strcpy(f,s.c_str());

//		cell.fill(f);

		char str[LONG_STR] = {0};

		char str_path[LONG_STR] = {0};
/*
		char str_name[LONG_STR] = {0};
*/

		if( 0 == strcmp("./",cell.get_path()) )
		{
			strcpy(str_path,"");
		}
		else
		{
			strcpy(str_path,cell.get_path());
		}

		strcpy(str,s.c_str());

		if( 1 == cell.show)
		{
			if( 0 == strcmp("<", cell.get_delimiter()) )
			{
	//			fprintf(f_out,"  \"%s\" [label=\"<%s%s>\"];\n",str, str_path, cell.get_name());
			fprintf(f_out,"  \"%s\" [label=\"<%s>\"];\n",str, cell.get_name());
			}
			else
			{
	//			fprintf(f_out,"  \"%s\" [label=\"%s%s\"];\n",str, str_path, cell.get_name());
				fprintf(f_out,"  \"%s\" [URL=\"%s\"];\n",str, cell.full());
			}
		}

		++i;
	}

	t_files::iterator i1 = files.begin();
	while( i1 != files.end())
	{
//		printf("    file[%s] includes:\n", ((*i1).first).c_str());
		c_cell cell_1 ;

//		if( 1 == cell_1.show )
//		{
			t_files_included::iterator i2 = (*i1).second.begin();
			while( i2 != (*i1).second.end())
			{
				c_cell cell_2 = (*i2).second;
				//printf("      ->[%s]\n", ((*i2).first).c_str());
				string s = ((*i2).first).c_str();
				if( 1 == cell_2.show )
				{
					fprintf(f_out,"  \"%s\" -> \"%s\";\n", ((*i1).first).c_str(), ((*i2).first).c_str() );
				}

				++i2;
			}
//		}

		++i1;
	}
	printf("  }\n");
	fprintf(f_out,"}\n");
	fclose(f_out);
}
/*----------------------------------------------------------------------------*/
int file_name_good(const char * f)
{
	int i = 0;

	while( f[i] != '\0' )
	{
		if( f[i] == '(' )
		{
			return 0;
		}
		else if( f[i] == ')' )
		{
			return 0;
		}
		else if( f[i] == '"' )
		{
			return 0;
		}
		else if( f[i] == '[' )
		{
			return 0;
		}
		else if( f[i] == ']' )
		{
			return 0;
		}
		else if( f[i] == '{' )
		{
			return 0;
		}
		else if( f[i] == '}' )
		{
			return 0;
		}
		else if( f[i] == '=' )
		{
			return 0;
		}
		else if( f[i] == '<' )
		{
			return 0;
		}
		else if( f[i] == '>' )
		{
			return 0;
		}

		++i;
	}

	return 1;
}
/*----------------------------------------------------------------------------*/
void c_ts::file_begin(char *f)
{
	if( strcmp("",file.get_name()) != 0 )
	{
		printf("error c_ts::file_begin(char *f) file_name [%s] not empty\n"
			, file.get_name()
		);
		exit(-1);
	}

	if( file_name_good(f) != 1  )
	{
		printf(" c_ts::file_begin() chars arent all good [%s]\n",f);
		return;
	}

	file.fill(f,(char *) "\"");

	if( 1 == config.callers )
	{
		file.show = 0;
	}
	else
	{
		file.show = 1;
	}

	all_files[file.full()]=file;
}
/*----------------------------------------------------------------------------*/
c_cell c_ts::resolve(const char *f, char * c_type)
{
	c_cell cell;
	cell.init();
	cell.fill(f, c_type);

	if( file_name_good(f) != 1  )
	{
		printf("    c_ts::resolve()\n");
		printf("    {\n");
		printf("      processing file [%s]\n",file.full());
		printf("      chars arent all good [%s]\n",f);
		printf("    }\n");
		return cell;
	}

	if( strcmp(file.get_path(), "./") != 0)
	{
		file.path_resolve(cell);

		printf("   file_included f[%s] resolve with [",f);
		file.print();
		printf("] -> [");
		cell.print();
		printf("]\n");

	}

	return cell;
}
/*----------------------------------------------------------------------------*/
void c_ts::file_included(char *f, char * c_type)
{
	c_cell cell = resolve(f, c_type);

	if( 1 == config.callers )
	{
		if( 0 == strcmp( config.callers_file, cell.full() ) )
		{
			cell.show = 1;
			file.show = 1;
			all_files[file.full()].show = 1;
		}
		else
		{
			cell.show = 0;
		}
	}
	else
	{
		file.show = 1;
		cell.show = 1;
	}

//    printf("    ## file[%s:%d] -> include[%s]\n",file.full(), yylineno, cell.full() );

	files[file.full()][cell.full()] = cell;
	all_files[cell.full()]=cell;
}
/*----------------------------------------------------------------------------*/
void c_ts::file_end(void)
{
	if( 0 == strcmp("",file.get_name()) )
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

