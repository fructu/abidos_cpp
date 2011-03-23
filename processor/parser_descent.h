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

#include "symbols_table.h"

typedef vector<c_token> t_tokens;

/*
  tokens_vector =[CLASS A { } ;]
  when we are parsing A
  context will have class_key == CLASS
  to know what is a
*/
enum
{
  BASE_CLASS_DECLARATION = 1
};

struct c_context
{
  unsigned i_token;
  int      class_key; //the rule when CLASS STRUCT UNION are set
  int      access_specifier;
  int      class_declaration;
  string   class_name_declaration; //class class_name_declaration
  int just_reloaded;

  c_context()
  {
    i_token = 0;
    class_key = 0;
    access_specifier = 0;
    class_declaration = 0;
    class_name_declaration = "";
    just_reloaded = 0;
  }
  void clear(void)
  {
    i_token = 0;
    class_key = 0;
    access_specifier = 0;
    class_declaration = 0;
    class_name_declaration = "";
    just_reloaded = 0;
  }
};

/*
  this class save the context and restore it
  to support parsers' backtraking
*/
struct c_context_tokens
{
    // index of tokens
//	unsigned i_token;    
    c_context context;

	c_context_tokens(c_context context_param);
	void save(c_context context_param);
	c_context restore(void);
};

class c_parser_descent
{
  private:
	//int just_reloaded;
//	unsigned i_token_actual;
//	c_token  token_actual;
    c_context context;

	t_tokens tokens_vector;
  
	void tokens_vector_print(void);
	void tokens_vector_print_from_actual(void);

	void tokens_vector_reload(void);
	void tokens_vector_clear(void);
  
    void token_print(void);
    int  token_get(void);
    c_token c_token_get(void);
//	void token_previous(void);
    void token_next(void);
/*
	rules from yacc
*/
	int error_recover(void);

	//Translation unit.
	//this is the start rule in yacc
	int translation_unit(void);

    //Expressions
    int id_expression(void);
    int unqualified_id(void);
    int qualified_id(void);

	 //Context-dependent identifiers.
	int class_name(void);

	// Lexical elements.
	int identifier(void);

	//declarations
	int declaration_seq(void);
	int declaration(void);
	int block_declaration(void);
	int simple_declaration(void);
	int decl_specifier_seq(void);
	int decl_specifier(void);
	int type_specifier(void);
    int simple_type_specifier(void);
	int class_specifier(void);
	int class_head(void);
	int class_key(void);
    int member_specification(void);
    int member_declaration(void);

    //Declarators
    int declarator(void);
    int direct_declarator(void);
    int declarator_id(void);
    int function_definition(void);

	// Derived classes.
	int base_clause(void);
	int base_specifier_list(void);
	int base_specifier(void);
	int access_specifier(void);

 	//Epsilon (optional) definitions.
	int declaration_seq_opt(void);
	int decl_specifier_seq_opt(void);
	int init_declarator_list_opt(void);
	int identifier_opt(void);
    int member_specification_opt(void);
	int base_clause_opt(void);

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
