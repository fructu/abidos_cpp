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
/*
typedef map<string, int> t_files_included;
typedef map<string, t_files_included> t_files;
*/
/*----------------------------------------------------------------------------*/
void c_ts::print(void)
{
	printf("  void c_ts::print()\n");
	printf("  {\n");
	printf("  }\n");
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
void c_ts::file_end(void)
{
	if( "" == file_name) 
	{
		printf("warning c_ts::file_end() file_name empty\n");
	}
	file_name = "";
}
/*----------------------------------------------------------------------------*/
