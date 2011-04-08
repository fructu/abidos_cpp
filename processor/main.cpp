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
#include "options.h"

//included only to do tests
//#include "symbols_table.h"

void test_01(void)
{
  c_parser_descent parser;
  
  parser.test_01();
}

void process_file(char file_name[])
{
  c_parser_descent parser;

  parser.yyparse(file_name);
//  in this place there are some important thinks destroyed
//  like  lex_file_name
//  generator.run((char *)"out.gv");
}

int main(int argc, char* argv[])
{
  printf("abidos v0.0.04\n");
  options.proccess(argc, argv);

//  ts_tests();

//  test_01();

  if( argc < 2)
  {
    printf( "error file expected\n");
    exit(1);
  }

  process_file(options.file);
  return 0;
}
