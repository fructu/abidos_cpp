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
/*----------------------------------------------------------------------------*/c_parser_descent::c_parser_descent()
{
}
/*----------------------------------------------------------------------------*/
c_parser_descent::~c_parser_descent()
{
}
/*----------------------------------------------------------------------------*/
void yytname_print(void)
{
  int i = 0;
  while(yytokens[i] != 0)
  {
     printf(" i=[%d] -> [%s]\n", i, yytokens[i]);
     i++;
  }
};
/*----------------------------------------------------------------------------*/
void c_parser_descent::token_print(void)
{
  printf(" yy_actual=[%3d] -> yytokens[%s] yytext[%s]\n"
	, token_get()
	, yytokens[token_get()]
	, yytext
  );
}
/*----------------------------------------------------------------------------*/
int c_parser_descent::token_get(void)
{
//  return yy_actual;

	return 1;
}
/*----------------------------------------------------------------------------*/
void c_parser_descent::token_next(void)
{
  int t = 0;

  t = yylex();
  c_token token(t, yytext);
  tokens_vector.push_back(token);
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
  while( token_get() != 0 )
  {
    token_next();
  }

  yylex_destroy();  
}
/*----------------------------------------------------------------------------*/
