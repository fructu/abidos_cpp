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
struct c_decl_specifier
{
	c_token token;

	int storage_class_specifier;
	int type_specifier;
	int function_specifier;
	int friend_specifier;
	int typedef_specifier;

	c_decl_specifier(c_token t)
	{
		token = t;

		storage_class_specifier = 0;
		type_specifier = 0;
		function_specifier = 0;
		friend_specifier = 0;
		typedef_specifier = 0;
	}

	void clear(void)
	{
		storage_class_specifier = 0;
		type_specifier = 0;
		function_specifier = 0;
		friend_specifier = 0;
		typedef_specifier = 0;
	}
};

typedef vector<c_decl_specifier> t_vector_decl_specifier;

struct c_class_member
{
	c_token token;
	t_vector_decl_specifier vector_decl_specifier;

	c_class_member()
	{
	}

	c_class_member(c_token t, t_vector_decl_specifier v)
	{
		token = t;
		vector_decl_specifier = v;
	}
};

typedef map<string, c_class_member> t_map_class_member;

struct c_symbol
{
  c_token token;
  int type;
  int class_key;

  t_map_base_class map_base_class;
  t_map_class_member map_class_member;

  c_symbol()
  {
    type  = 0;
    class_key = 0;
    map_base_class.clear();
    map_class_member.clear();
  }

  c_symbol(c_token token_1)
  {
      token = token_1;
      type  = 0;
      class_key = 0;
      map_base_class.clear();
	  map_class_member.clear();
  }

  ~c_symbol()
  {
      type  = 0;
      class_key = 0;
      map_base_class.clear();
      map_class_member.clear();
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

