/*------------------------------------------------------------------------------

    Proyecto            : 
    Codigo              : main.cpp
    Descripcion         : 
    Version             : 0.1
    Autor               : F. Manuel Hevia Garcia
    Fecha creacion      : 03/03/2011
    Fecha modificacion  : 

    Observaciones:
        ->

------------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>

#include "parser_descent.h"

void test_01(char file_name[])
{
  c_parser_descent parser;
  
  parser.yyparse(file_name);
}

int main(int argc, char* argv[])
{
  printf("abidos v0.0.01\n");
  if( argc < 2)
  {
    printf( "error file expected\n");
    exit(1);
  }
  
  test_01(argv[1]);
  return 0;
}