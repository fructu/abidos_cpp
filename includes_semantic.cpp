/*------------------------------------------------------------------------------

	Proyecto            : 
	Codigo              : includes_semantic.cpp
	Descripcion         : 
	Version             : 0.1
	Autor               : F. Manuel Hevia Garcia
	Fecha creacion      : 30/12/2010
	Fecha modificacion  : 

	Observaciones:
     	->

------------------------------------------------------------------------------*/
#include <stdio.h>
#include "includes_semantic.h"
#include "includes_lex_yacc.h"
#include "ts.h"
/*----------------------------------------------------------------------------*/
c_includes_semantic includes_semantic;
/*----------------------------------------------------------------------------*/
c_includes_semantic::c_includes_semantic()
{
}
/*----------------------------------------------------------------------------*/
c_includes_semantic::~c_includes_semantic()
{
}
/*----------------------------------------------------------------------------*/
void c_includes_semantic::init(void)
{
}
/*----------------------------------------------------------------------------*/
void c_includes_semantic::include_process(char * f, char c_type)
{
	ts.file_included(f, c_type);
}
/*----------------------------------------------------------------------------*/
