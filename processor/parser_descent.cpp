/*------------------------------------------------------------------------------

    Proyecto            : show_includes
    Codigo              : parser_descent.cpp
    Descripcion         :
    Version             : 0.1
    Autor               : F. Manuel Hevia Garcia
    Fecha creacion      : 03/03/2011
    Fecha modificacion  :

    Observaciones :


------------------------------------------------------------------------------*/
#include "parser_descent.h"
/*----------------------------------------------------------------------------*/
c_parser_descent::c_parser_descent()
{
  yy_actual = 0;
}
/*----------------------------------------------------------------------------*/
c_parser_descent::~c_parser_descent()
{
}
/*----------------------------------------------------------------------------*/
int c_parser_descent::token_get(void)
{
  return yy_actual;
}
/*----------------------------------------------------------------------------*/
void c_parser_descent::token_next(void)
{
  yy_actual = yylex();
}
/*----------------------------------------------------------------------------*/
void c_parser_descent::yyparse(char * file_name)
{
  if (!(yyin=fopen(file_name,"r")))
  {
    printf("\nERROR: yyparse [%s] erroneo\n",file_name);
    return;
  }

  yylex_destroy();  
}
/*----------------------------------------------------------------------------*/