/*------------------------------------------------------------------------------

    Proyecto            : abidos
    Codigo              : symbols_table.h
    Descripcion         :
    Version             : 0.1
    Autor               : F. Manuel Hevia Garcia
    Fecha creacion      : 21/03/2011
    Fecha modificacion  :

    Observaciones :


------------------------------------------------------------------------------*/
#ifndef symbols_table_h
#define symbols_table_h

#include <string>
#include <map>
//#include <vector>

using namespace std;

struct c_token
{
    int     id;
    string  text;

    c_token()
    {
        id = 0;
        text  = "";
    }

    c_token(int t, char * s)
    {
        id = t;
        text  = s;
    }
};

/*
  when a token is analised it can become to symbols_table

  class A

  class -> does not become a symbol
  A -> become a symbol
*/
struct c_symbol
{
  c_token token;

  c_symbol()
  {
  }

  c_symbol(c_token token_1)
  {
      token = token_1;
  }
};

typedef map<string, c_symbol> t_symbols;

#endif