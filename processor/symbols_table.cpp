/*------------------------------------------------------------------------------

    Proyecto            : abidos
    Codigo              : symbols_table.cpp
    Descripcion         :
    Version             : 0.1
    Autor               : F. Manuel Hevia Garcia
    Fecha creacion      : 21/03/2011
    Fecha modificacion  :

    Observaciones :


------------------------------------------------------------------------------*/
#include "symbols_table.h"
#include <stdio.h>
#include <stdlib.h>
/*----------------------------------------------------------------------------*/
c_symbols_table::c_symbols_table()
{
  stack.clear();
}
/*----------------------------------------------------------------------------*/  
c_symbols_table::~c_symbols_table()
{
  stack.clear();
}
/*----------------------------------------------------------------------------*/
void c_symbols_table::set()
{
  t_symbols t;
  stack.push_back(t);
}
/*----------------------------------------------------------------------------*/
void c_symbols_table::unset()
{
  long i = stack.size() - 1;

  if( i < 0)
  {
    printf("error void c_symbols_table::unset(c_token token) -> ( last < 0)\n");
    exit(-1);
  }

  unsigned last = stack.size() - 1;

  stack[last].clear();

  stack.pop_back();
}
/*----------------------------------------------------------------------------*/
void c_symbols_table::insert(c_token token)
{
  long i = stack.size() - 1;

  if( i < 0)
  {
    printf("error void c_symbols_table::unset(c_token token) -> ( last < 0)\n");
    exit(-1);
  }

  unsigned last = stack.size() - 1;

  if( i < 0)
  {
    printf("error void c_symbols_table::insert(c_token token) -> ( last < 0)\n");
    exit(-1);
  }

  c_symbol symbol(token);
  stack[last][token.text] = symbol;
}
/*----------------------------------------------------------------------------*/
void c_symbols_table::print(void)
{
  unsigned i_stack = 0;

  t_symbols::iterator i_vector;

  printf("ts::print\n");
  printf("{\n");
  if( stack.size() <= 0)
  {
    printf("  empty\n");
  }

  for( i_stack = 0; i_stack < stack.size(); ++i_stack )
  {
    printf("  stack level[%d]\n",i_stack);
    printf("  {\n");
    for( i_vector = stack[i_stack].begin(); i_vector != stack[i_stack].end(); ++i_vector )
    {
      printf("    first[%s] id[%d] text[%s]\n"
        , ((*i_vector).first).c_str()
        , ((*i_vector).second).token.id
        , ((*i_vector).second).token.text.c_str()
      );
    }
    printf("  }\n");
  }
  printf("}\n");
}
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
void ts_tests(void)
{
  printf("symbols_table testing\n");
  c_symbols_table ts;
  c_token t1(1,(char *)"level 0 ->1");
//  c_symbol s1(t1);

  ts.set();

  ts.insert(t1);

  ts.set();

  c_token t2(2,(char *)"level 1 -> 2");
  ts.insert(t2);

  c_token t3(3,(char *)"level 1 -> 3");
  ts.insert(t3);

  c_token t4(4,(char *)"level 1 -> 4");
  ts.insert(t4);

  ts.set();
  c_token t5(5,(char *)"level 2 -> 5");
  ts.insert(t5);

  printf("print [0-2]\n");
  ts.print();

  printf("print [0-1]\n");
  ts.unset();
  ts.print();

  printf("print [0-0]\n");
  ts.unset();
  ts.print();

  printf("print [empty]\n");
  ts.unset();
  ts.print();

// this is useful to lock if stack detect empty state
//  ts.unset();
  printf("symbols_table testing end\n");
}
/*----------------------------------------------------------------------------*/