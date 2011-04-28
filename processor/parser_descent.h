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

#define NO_IDENTIFIER "@IDENTIFIER#"

# define TAB " "
void trace(string & tab, string s);

#include "symbols_table.h"

typedef vector < c_token > t_tokens;

/*
 * tokens_vector =[CLASS A { } ;] when we are parsing A context will have
 * class_key == CLASS to know what is a
 */
enum t_class_specifier_status
{
  NO_CLASS_STATUS = 0,
  CLASS_SPECIFIER_STATUS_IDENTIFIER,
  CLASS_SPECIFIER_STATUS_BASE_DECLARATION,
  CLASS_SPECIFIER_STATUS_MEMBER_SPECIFIER,
  CLASS_SPECIFIER_STATUS_MEMBER_DECLARATOR
};
/*
 * if you adde more members pls actualize restore_but_not_i_token()
 */
struct c_context
{
  unsigned i_token;
  enum t_class_specifier_status class_specifier_status;	// the
  // semantic
  // will
  // know is
  // inside
  // a
  // class/struct/union
  // declaration

  int class_key;		// the rule when CLASS STRUCT UNION are
  // set
  int access_specifier;

  string class_name_declaration;	// class
  // class_name_declaration

  int i_am_in_member;
  string member_declaration;	// member of class
  int member_definition_outside; // it must be a first declaration inside

  string declaration;

  int i_am_in_parameter_declaration;

  int just_reloaded;

  t_vector_decl_specifier param_vector_decl_specifier;

  c_class_member class_member;

  c_declarator declarator;

  c_context()
  {
    i_token = 0;
    class_specifier_status = NO_CLASS_STATUS;
    class_key = 0;
    access_specifier = 0;

    class_name_declaration = "";

    i_am_in_member = 0;
    member_declaration = "";
    member_definition_outside = 0;

    declaration = "";

    i_am_in_parameter_declaration = 0;
    just_reloaded = 0;

    param_vector_decl_specifier.clear();
    class_member.clear();
    declarator.clear();
  }
  void clear(void)
  {
    i_token = 0;
    class_specifier_status = NO_CLASS_STATUS;
    class_key = 0;
    access_specifier = 0;

    class_name_declaration = "";

    i_am_in_member = 0;
    member_declaration = "";
    member_definition_outside = 0;

    declaration = "";

    i_am_in_parameter_declaration = 0;
    just_reloaded = 1;

    param_vector_decl_specifier.clear();

    class_member.clear();
    declarator.clear();
  }
  void restore_but_not_i_token(c_context & context_param)
  {
    context_param.class_specifier_status = class_specifier_status;
    context_param.class_key = class_key;
    context_param.access_specifier = access_specifier;
    context_param.i_am_in_member = i_am_in_member;
    context_param.member_declaration = member_declaration;
    context_param.member_definition_outside = member_definition_outside;
    context_param.i_am_in_parameter_declaration =
      i_am_in_parameter_declaration;

    context_param.class_name_declaration = class_name_declaration;
    context_param.param_vector_decl_specifier =
      param_vector_decl_specifier;
    context_param.class_member = class_member;
    context_param.declarator = declarator;
    context_param.declaration = declaration;
  }
};

/*
 * this class save the context and restore it to support parsers'
 * backtraking
 */
struct c_context_tokens
{
  // index of tokens
  // unsigned i_token;
  c_context context;

  c_context_tokens(c_context context_param);
  void save(c_context context_param);
  c_context restore(void);

  void restore_but_not_i_token(c_context & context_param);
};

class c_parser_descent
{
private:
  // int just_reloaded;
  // unsigned i_token_actual;
  // c_token token_actual;
  c_context context;

  t_tokens tokens_vector;

  void tokens_vector_print(void);
  void tokens_vector_print_from_actual(void);

  void tokens_vector_reload(void);
  void tokens_vector_clear(void);

  void token_print(void);
  int token_get(void);
  c_token c_token_get(void);
  // void token_previous(void);
  void token_next(string tab);
  /*
   * rules from yacc
   */
  int error_recover(string tab);

  // Translation unit.
  // this is the start rule in yacc
  int translation_unit(void);

  // Expressions
  int id_expression(string tab);
  int unqualified_id(string tab);
  int qualified_id(string tab);

  // Context-dependent identifiers.
  int class_name(string tab);

  // Lexical elements.
  int identifier(string tab);

  // Statements
  int statement(string tab);
  int compound_statement(string tab);
  int statement_seq(string tab);

  // declarations
  int declaration_seq(string tab);
  int declaration(string tab);
  int block_declaration(string tab);
  int simple_declaration(string tab);
  int decl_specifier_seq(string tab);
  int decl_specifier(string tab);
  int storage_class_specifier(string tab);
  int type_specifier(string tab);
  int function_specifier(string tab);
  int simple_type_specifier(string tab);
  int ptr_specifier(string tab);    //rule mine
  int type_name(string tab);
  int class_specifier(string tab);
  int class_head(string tab);
  int class_key(string tab);
  int member_specification(string tab);
  int member_declaration(string tab);
  int member_declarator_list(string tab);
  int member_declarator(string tab);

  // Declarators
  int init_declarator_list(string tab);
  int init_declarator(string tab);
  int declarator(string tab);
  int direct_declarator(string tab);
  int ptr_operator(string tab);
  int declarator_id(string tab);
  int parameter_declaration_clause(string tab);
  int parameter_declaration_list(string tab);
  int parameter_declaration(string tab);
  int function_definition(string tab);
  int function_body(string tab);

  // Derived classes.
  int base_clause(string tab);
  int base_specifier_list(string tab);
  int base_specifier(string tab);
  int access_specifier(string tab);

  // Epsilon (optional) definitions.
  int declaration_seq_opt(string tab);
  int COLONCOLON_opt(string tab);
  int statement_seq_opt(string tab);
  int decl_specifier_seq_opt(string tab);
  int init_declarator_list_opt(string tab);
  int identifier_opt(string tab);
  int parameter_declaration_list_opt(string tab);
  int ELLIPSIS_opt(string tab);
  int member_specification_opt(string tab);
  int base_clause_opt(string tab);
  int member_declarator_list_opt(string tab);

public:
  c_parser_descent();
  ~c_parser_descent();
  void yyparse(char *file_name);

  /*
   * tests
   */
  int test_01(void);
};

#endif
