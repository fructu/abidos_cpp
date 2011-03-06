/*------------------------------------------------------------------------------

    Proyecto            : abidos
    Codigo              : parser_descent.h
    Descripcion         :
    Version             : 0.1
    Autor               : F. Manuel Hevia Garcia
    Fecha creacion      : 03/03/2011
    Fecha modificacion  :

    Observaciones :


------------------------------------------------------------------------------*/
#ifndef parser_descent_h
#define parser_descent_h

#include "lex_yacc.h"

#include <string>
#include <vector>

using namespace std;

struct c_token
{
	int 	token;
	string  text;

	c_token()
	{
		token = 0;
		text  = "";
	}

	c_token(int t, char * s)
	{
		token = t;
		text  = s;
	}
};

typedef vector<c_token> t_tokens;

class c_parser_descent
{
  private:
	int just_reloaded;
	t_tokens::iterator i_token_actual;
//	c_token  token_actual;
	t_tokens tokens_vector;
  
	void tokens_vector_print(void);
	void tokens_vector_reload(void);
	void tokens_vector_clear(void);
  
    void token_print(void);
    int  token_get(void);
    void token_next(void);
/*
	rules from yacc
*/
	//Translation unit.
	//this is the start rule in yacc
	int translation_unit(void);

	// Lexical elements.
	int identifier(c_token & token_identifier);

	//declarations
	int declaration_seq(void);
	int declaration(void);
	int block_declaration(void);
	int simple_declaration(void);
	int decl_specifier_seq(void);
	int decl_specifier(void);
	int type_specifier(void);
	int class_specifier(void);
	int class_head(void);
	int class_key(void);

 	//Epsilon (optional) definitions.
	int declaration_seq_opt(void);
	int decl_specifier_seq_opt(void);
	int init_declarator_list_opt(void);

  public:    
    c_parser_descent();
    ~c_parser_descent();
    void yyparse(char * file_name);

/*
	tests
*/
	int test_01(void);
};

#endif
