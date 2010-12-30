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

/*
        void init(void);        
    protected:
    public:        
        c_includes_semantic();
        ~c_includes_semantic();

		void define_process(char * str);
*/
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
void c_includes_semantic::define_process(char * str)
{
	printf("c_includes_semantic::define_process(char * [%s])\n", str);
}
/*----------------------------------------------------------------------------*/
