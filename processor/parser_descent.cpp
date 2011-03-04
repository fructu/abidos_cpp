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
#include "tokens.h"
/*----------------------------------------------------------------------------*/
c_parser_descent::c_parser_descent()
{
  yy_actual = 0;
}
/*----------------------------------------------------------------------------*/
c_parser_descent::~c_parser_descent()
{
}

void yytname_print(void)
{
  int i = 0;
  while(yytname[i] != 0)
  {
     printf(" i=[%d] -> [%s]\n", i, yytokens[i]);
     i++;
  }
};
/*----------------------------------------------------------------------------*/
void c_parser_descent::token_print(void)
{
  printf(" yy_actual=[%d] -> [%s]\n", yy_actual, yytokens[yy_actual]);
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
  token_print();
}
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
void c_parser_descent::yyparse(char * file_name)
{
  if (!(yyin=fopen(file_name,"r")))
  {
    printf("\nERROR: yyparse did not can open [%s]\n",file_name);
    return;
  }

  //yytname_print();
  
  token_next();
  while( yy_actual != 0 )
  {
    
    token_next();
  }

  yylex_destroy();  
}
/*----------------------------------------------------------------------------*/
