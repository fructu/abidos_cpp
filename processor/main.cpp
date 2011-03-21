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

//included only to do tests
#include "symbols_table.h"

void test_01(void)
{
  c_parser_descent parser;
  
  parser.test_01();
}

void process_file(char file_name[])
{
  c_parser_descent parser;
  
  parser.yyparse(file_name);
}

int main(int argc, char* argv[])
{
  printf("abidos v0.0.02\n");

  ts_tests();

//  test_01();

  if( argc < 2)
  {
    printf( "error file expected\n");
    exit(1);
  }
  
  process_file(argv[1]);
  return 0;
}
