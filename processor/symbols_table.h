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
#include <vector>
#include <stdio.h>

#include "tokens.h"

using namespace std;

void ts_tests(void);

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
struct c_base_class
{
  string text;
  int     access_specifier;

  c_base_class()
  {
    text = "";
    access_specifier = 0;
  }

  c_base_class(string t, int access)
  {
    text = t;
    access_specifier = access;
  }
};
typedef map<string, c_base_class> t_map_base_class;



/*
decl_specifier:
	storage_class_specifier
	| type_specifier
	| function_specifier
	| FRIEND
	| TYPEDEF
	;

## todo vector<c_decl_specifier> t_vector_decl_specifier
*/


struct c_symbol
{
  c_token token;
  int type;
  int class_key;

  t_map_base_class map_base_class;

  c_symbol()
  {
    type  = 0;
    class_key = 0;
    map_base_class.clear();
  }

  c_symbol(c_token token_1)
  {
      token = token_1;
      type  = 0;
      class_key = 0;
      map_base_class.clear();
  }

  ~c_symbol()
  {
      type  = 0;
      class_key = 0;
      map_base_class.clear();
  }
  void print(const char * tab);
};

typedef map<string, c_symbol> t_symbols;
typedef vector<t_symbols> stack_symbols;

class c_symbols_table
{
  private:
    stack_symbols stack;

  public:
    c_symbols_table();
    ~c_symbols_table();
    void set();
    void unset();

    void insert(c_symbol symbol);
    void print(void);

    c_symbol * search_symbol(string str);
};

extern c_symbols_table ts;

#endif

