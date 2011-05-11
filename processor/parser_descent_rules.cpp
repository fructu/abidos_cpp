/*------------------------------------------------------------------------------

    Proyecto            : abidos
    Codigo              : parser_descent_rules.cpp
    Descripcion         :
    Version             : 0.1
    Autor               : F. Manuel Hevia Garcia
    Fecha creacion      : 03/03/2011
    Fecha modificacion  :

    Observaciones :


------------------------------------------------------------------------------*/
#include <stdlib.h>
#include "parser_descent.h"
#include "tokens.h"

#include "semantic.h"
/*----------------------------------------------------------------------
 * error_recover.
 *----------------------------------------------------------------------*/
int
c_parser_descent::error_recover(string tab)
{
  trace(tab, "## error_recover");
  while (token_get() != 0)
    {
      token_next(tab);
      if (';' == token_get())
        {
          return 1;
        }

      if ('}' == token_get())
        {
          return 1;
        }
      if (')' == token_get())
        {
          return 1;
        }
    }

  printf
  ("\nc_parser_descent::error_recover() -> token_get() == 0 !! \n");
  return 0;
}

/*----------------------------------------------------------------------
 * Translation unit.
 *----------------------------------------------------------------------*/
/*
 * translation_unit: declaration_seq_opt ;
 */
int c_parser_descent::translation_unit(void)
{
  string tab = "";
  trace(tab, "## translation_unit");
  tokens_vector_clear();

  if (1 == declaration_seq_opt(TAB))
    {
      return 1;
    }
  /*
   * if( 1 == error_recover(tab) ) { return 1; }
   */
  c_context_tokens context_tokens(context);
  token_next(tab);
  if (0 == token_get())
    {
      printf("translation_unit() -> EOF\n");
      return 1;
    }
  context = context_tokens.restore();

  printf("c_parser_descent::translation_unit() error sintax\n");
  return 0;
}

/*----------------------------------------------------------------------
 * Expressions.
 *----------------------------------------------------------------------*/
/*
 * id_expression: unqualified_id | qualified_id ;
 */
int c_parser_descent::id_expression(string tab)
{
  trace(tab, "## id_expression");

  if (1 == unqualified_id(tab))
    {
      return 1;
    }

  if (1 == qualified_id(tab))
    {
      return 1;
    }

  return 0;
}

/*
 * unqualified_id: identifier | operator_function_id |
 * conversion_function_id | '~' class_name | template_id ;
 */
int c_parser_descent::unqualified_id(string tab)
{
  trace(tab, "## unqualified_id");

  if (1 == identifier(tab))
    {
      return 1;
    }

  //## todo

  // destructor
  // | '~' class_name
  c_context_tokens context_tokens(context);
  token_next(tab);
  if ( '~' == token_get() )
    {
      context.class_member.is_destructor = 1;
      if (1 == class_name(tab))
        {
          return 1;
        }
      context.class_member.is_destructor = 0;
    }
  context = context_tokens.restore();

  return 0;
}

/*
 * qualified_id: nested_name_specifier TEMPLATE_opt unqualified_id ;
 */
int c_parser_descent::qualified_id(string tab)
{
  trace(tab, "## qualified_id");
  printf("##: mark_7 qualified_id_context\n");

  tokens_vector_print_from_actual();
//  context = qualified_id_context;
  tokens_vector_print_from_actual();


  if (0 == nested_name_specifier(tab))
    {
      return 0;
    }

  //## todo TEMPLATE_opt

  if (0 == unqualified_id(tab))
    {
      return 0;
    }

  return 1;
}

/*
nested_name_specifier:
	class_or_namespace_name COLONCOLON nested_name_specifier_opt
	;

  class_or_namespace_name : puted code inside this rule
  nested_name_specifier_opt : puted code inside this rule
class A
{
  class A_1
  {
    int A_1_i;
  };
};

A::A_1
in ts there is not [A_1] we need [A::A_1] A_1 would be
IDENTIFIER

A has been consumed in this point but :: not
*/

int c_parser_descent::nested_name_specifier(string tab)
{
  trace(tab, "## nested_name_specifier");

  printf("##: mark_-03 class_name_declaration [%s]\n", context.class_name_declaration.c_str());

  c_context_tokens context_tokens(context);

  int result = 0;
  string chain = "";

  /*
    c_token token;
    c_decl_specifier dcl(token);
    dcl = semantic.get_last_decl_specifier();
    if( 1 == dcl.has_colon_colon_after )
      {
        chain = dcl.token.text + "::";
        printf("##: yupi mark_-02 class_name_declaration [%s]\n", chain.c_str());
      }


    if ( 0 != context.class_name_declaration.size() )
      {
        chain = chain + context.class_name_declaration;
        printf("##: mark_-01 class_name_declaration [%s]\n", chain.c_str());
      }
  */
  while ( 1 )
    {
      context_tokens.save(context);
      token_next(tab);
      if ( CLASS_NAME != token_get() )
        {
          if ( IDENTIFIER == token_get() )
            {
              if ( 0 != chain.size() )
                {
                  chain = chain + c_token_get().text;
                }
              chain = chain + c_token_get().text;
              c_symbol *p_symbol = ts.search_symbol(yytext);
              printf("##: mark_00 chain [%s]\n", chain.c_str());
              if (0 == p_symbol)
                {
                  context = context_tokens.restore();
                  return result;
                }
              if (p_symbol->type != CLASS_NAME)
                {
                  context = context_tokens.restore();
                  return result;
                }
              printf("##: mark_00 chain [%s] is a class :-)\n", chain.c_str());
            }
          else
            {
              context = context_tokens.restore();
              return result;
            }
        }
      else
        {
          chain = chain + c_token_get().text;
        }

      printf("##: mark_01 [%s]\n", c_token_get().text.c_str());

      token_next(tab);
      if ( COLONCOLON != token_get() )
        {
          context = context_tokens.restore();
          return 0;
        }
      if ( 1 == result)
        {
//          chain = chain + c_token_get().text;
        }
      printf("##: mark_02 [%s]\n", c_token_get().text.c_str());
      printf("##: mark_03 chain [%s]\n", chain.c_str());
      result = 1;
    }

  context = context_tokens.restore();

  return result;
}

/*
class_or_namespace_name:
	class_name
	| namespace_name
	;

	only used in previous rule -> i inserted the code in the prev.
*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------
 * Context-dependent identifiers.
 *----------------------------------------------------------------------*/
/*
 *typedef_name:
 *	// identifier
 *  TYPEDEF_NAME
 *	;
*/
int c_parser_descent::typedef_name(string tab)
{
  trace(tab, "## typedef_name");

  c_context_tokens context_tokens(context);

  token_next(tab);

  if (TYPEDEF_NAME == token_get())
    {
      c_decl_specifier decl(c_token_get());
      decl.type_specifier = 1;

      if (1 == context.i_am_in_parameter_declaration)
        {
          context.param_vector_decl_specifier.push_back(decl);
        }
      else
        {
          semantic.push_back_vector_decl_specifier(decl);
        }
      return 1;
    }

  context = context_tokens.restore();
  return 0;
}
/*----------------------------------------------------------------------------*/
/*
 * class_name: CLASS_NAME | template_id ;
 */
int c_parser_descent::class_name(string tab)
{
  trace(tab, "## class_name");

  c_context_tokens context_tokens(context);

  token_next(tab);

  if (CLASS_NAME == token_get())
    {
      // the context must be specified more up in rules tree
      // context.class_specifier_status =
      // CLASS_SPECIFIER_STATUS_IDENTIFIER;
      // -> this rule is used in serveral others rules.

      semantic.class_name(context, c_token_get());
      return 1;
    }
  // ##todo | template_id

  context = context_tokens.restore();
  return 0;
}


/*----------------------------------------------------------------------
 * Lexical elements.
 *----------------------------------------------------------------------*/
/*
 * identifier: IDENTIFIER ;
 */
int c_parser_descent::identifier(string tab)
{
  trace(tab, "## identifier");
  c_context_tokens context_tokens(context);

  token_next(tab);

  if (IDENTIFIER == token_get())
    {
      semantic.identifier(context, c_token_get());
      return 1;
    }

  context = context_tokens.restore();
  return 0;
}

/*----------------------------------------------------------------------
 * Statements.
 *----------------------------------------------------------------------*/
/*
statement:
	labeled_statement
	| expression_statement
	| compound_statement
	| selection_statement
	| iteration_statement
	| jump_statement
	| declaration_statement
	| try_block
	;
*/
int c_parser_descent::statement(string tab)
{
  trace(tab, "## statement dummy version");
  return 1;
}

/*
compound_statement:
	'{' statement_seq_opt '}'
	;
*/
int c_parser_descent::compound_statement(string tab)
{
  trace(tab, "## compound_statement");

  c_context_tokens context_tokens(context);

  token_next(tab);
  if ( '{' != token_get())
    {
      context = context_tokens.restore();
      return 0;
    }

  context.declarator.has_body = 1;

  if ( 0 == statement_seq_opt(tab) )
    {
//      context = context_tokens.restore();
      return 0;
    }

  token_next(tab);
  if ( '}' != token_get())
    {
      context = context_tokens.restore();
      return 0;
    }

  return 1;
}
/*----------------------------------------------------------------------------*/
/*
statement_seq:
	statement
	| statement_seq statement
	;
*/
int c_parser_descent::statement_seq(string tab)
{
  trace(tab, "## statement_seq");

  int n_open_braket = 0;

  c_context_tokens context_good_way(context);

  while (token_get() != 0)
    {
      //## dummy
      statement(tab);

      context_good_way.save(context);
      token_next(tab);
      if ('{' == token_get())
        {
          ++n_open_braket;
        }

      if ('}' == token_get())
        {
          if ( 0 == n_open_braket )
            {
              context = context_good_way.restore();
              return 1;
            }
          --n_open_braket;
        }
    }

  return 1;
}

/*----------------------------------------------------------------------
 * Declarations.
 *----------------------------------------------------------------------*/
/*
 * declaration_seq: declaration | declaration_seq declaration ;
 */
int c_parser_descent::declaration_seq(string tab)
{
  trace(tab, "## declaration_seq");

  int result = 0;

  while (1 == declaration(tab))
    {
      printf("## while declaration [ok]\n\n");
      tokens_vector_clear();
      result = 1;
    }

  return result;
}

/*----------------------------------------------------------------------------*/
/*
 * declaration: block_declaration | function_definition |
 * template_declaration | explicit_instantiation | explicit_specialization
 * | linkage_specification | namespace_definition ;
 */
// ## todo rest of | ...
int c_parser_descent::declaration(string tab)
{
  trace(tab, "## declaration");

  if (1 == block_declaration(tab))
    {
      return 1;
    }

  if (1 == function_definition(tab))
    {
      semantic.declarator_insert(tab, context);
      return 1;
    }

  return 0;
}

/*----------------------------------------------------------------------------*/
/*
 * block_declaration: simple_declaration | asm_definition |
 * namespace_alias_definition | using_declaration | using_directive ;
 */
// ## todo rest of | ...
int c_parser_descent::block_declaration(string tab)
{
  trace(tab, "## block_declaration");

  if (1 == simple_declaration(tab))
    {
      return 1;
    }
  // tokens_vector_reload();


  return 0;
}

/*----------------------------------------------------------------------------*/
/*
 * simple_declaration: decl_specifier_seq_opt init_declarator_list_opt ';'
 * ;
 */
int c_parser_descent::simple_declaration(string tab)
{
  trace(tab, "## simple_declaration");

  if ( CLASS_SPECIFIER_STATUS_MEMBER_SPECIFIER != context.class_specifier_status )
    {
      semantic.clear_decl_specifier();
    }

  decl_specifier_seq_opt(tab);	// long int a = 0; this is 'long
  // int'
  init_declarator_list_opt(tab);	// long int a = 0; this is 'a = 0'

  c_context_tokens context_tokens(context);

  token_next(tab);

  // functions with body does not have ; in the end
  printf("\n\n#### context.declarator.has_body[%d]\n\n",context.declarator.has_body);

  if ( 1 == context.declarator.has_body )
    {
      context.declarator.has_body = 0;
      tokens_vector_clear();
      tokens_vector_print();
      return 1;
    }
  else
    {
      tokens_vector_print();
      if (';' == token_get())
        {
          tokens_vector_clear();
          return 1;
        }
    }

  context = context_tokens.restore();
  return 0;
}

/*----------------------------------------------------------------------------*/
/*
 * decl_specifier_seq: decl_specifier_seq_opt decl_specifier ;
 */
int c_parser_descent::decl_specifier_seq(string tab)
{
  trace(tab, "## decl_specifier_seq");
  // decl_specifier_seq_opt->decl_specifier_seq->decl_specifier_seq_opt...INFINITE
  // decl_specifier_seq_opt(tab);

  int result = 0;
  c_context_tokens context_tokens(context);

  while (1 == decl_specifier(tab))
    {
      result = 1;
    }

  if (1 == result)
    {
      // tokens_vector_print_from_actual();
    }
  else
    {
      context = context_tokens.restore();
    }

  return result;
}

/*----------------------------------------------------------------------------*/
/*
 * decl_specifier: storage_class_specifier | type_specifier |
 * function_specifier | FRIEND | TYPEDEF ;
 */
// ## todo rest of | ...
int c_parser_descent::decl_specifier(string tab)
{
  trace(tab, "## decl_specifier");

  if (1 == storage_class_specifier(tab))
    {
      return 1;
    }

  if (1 == type_specifier(tab))
    {
      return 1;
    }

  if (1 == function_specifier(tab))
    {
      return 1;
    }

  if ( 1 == friend_specifier(tab))
    {
      return 1;
    }

  if ( 1 == typedef_specifier(tab))
    {
      return 1;
    }

  if ( 1 == ptr_specifier(tab))
    {
      return 1;
    }

  return 0;
}

int c_parser_descent::friend_specifier(string tab)
{
  trace(tab, "## friend_specifier");
  int result = 0;
  c_context_tokens context_tokens(context);
  token_next(tab);

  if ( FRIEND == token_get())
    {
      result = 1;
    }

// friend class c_generator_original;
  if (1 == result)
    {
      c_decl_specifier decl(c_token_get());
      decl.type_specifier = 1;

      if (1 == context.i_am_in_parameter_declaration)
        {
          context.param_vector_decl_specifier.push_back(decl);
        }
      else
        {
          semantic.push_back_vector_decl_specifier(decl);
        }
    }
  else
    {
      context = context_tokens.restore();
      return 0;
    }

  c_context_tokens context_tokens_2(context);
  token_next(tab);

  if ( CLASS == token_get())
    {
      c_decl_specifier decl(c_token_get());
      decl.type_specifier = 1;

      if (1 == context.i_am_in_parameter_declaration)
        {
          context.param_vector_decl_specifier.push_back(decl);
        }
      else
        {
          semantic.push_back_vector_decl_specifier(decl);
        }

      token_next(tab);
      context.class_specifier_status = CLASS_SPECIFIER_STATUS_FRIEND_DECLARATOR;
      //## this need a improve to parse thinks like A::B...
      if (CLASS_NAME == token_get())
        {
          semantic.class_name_friend(context, c_token_get());
          return 1;
        }
      return 1;
    }

  context = context_tokens_2.restore();
  return 1;
}

int c_parser_descent::typedef_specifier(string tab)
{
  trace(tab, "## typedef_specifier");
  int result = 0;
  c_context_tokens context_tokens(context);
  token_next(tab);

  if ( TYPEDEF == token_get())
    {
      result = 1;
    }

// friend class c_generator_original;
  if (1 == result)
    {
      //this should pass to c_symbol and in context = 0;
      context.is_typedef = 1;

      c_decl_specifier decl(c_token_get());
      decl.type_specifier = 1;

      if (1 == context.i_am_in_parameter_declaration)
        {
          context.param_vector_decl_specifier.push_back(decl);
        }
      else
        {
          semantic.push_back_vector_decl_specifier(decl);
        }

      //## testing
      return 1;
    }
  else
    {
      context = context_tokens.restore();
      return 0;
    }

  c_context_tokens context_tokens_2(context);
  token_next(tab);

  if ( CLASS  == token_get() ||
       STRUCT == token_get()
     )
    {
      c_decl_specifier decl(c_token_get());
      decl.type_specifier = 1;

      if (1 == context.i_am_in_parameter_declaration)
        {
          context.param_vector_decl_specifier.push_back(decl);
        }
      else
        {
          semantic.push_back_vector_decl_specifier(decl);
        }

      token_next(tab);
//      context.class_specifier_status = CLASS_SPECIFIER_STATUS_FRIEND_DECLARATOR;
      //## this need a improve to parse thinks like A::B...
      if (CLASS_NAME == token_get())
        {
//          semantic.class_name_friend(context, c_token_get());
          return 1;
        }
      return 1;
    }

  context = context_tokens_2.restore();
  return 1;
}

/*
storage_class_specifier:
	AUTO
	| REGISTER
	| STATIC
	| EXTERN
	| MUTABLE
	;
*/
int c_parser_descent::storage_class_specifier(string tab)
{
  trace(tab, "## storage_class_specifier");

  int result = 0;
  c_context_tokens context_tokens(context);
  token_next(tab);

  if ( AUTO == token_get())
    {
      result = 1;
    }

  if ( REGISTER == token_get())
    {
      result = 1;
    }

  if ( STATIC == token_get())
    {
      result = 1;
    }

  if ( EXTERN == token_get())
    {
      result = 1;
    }

  if ( MUTABLE == token_get())
    {
      result = 1;
    }


  if (1 == result)
    {
      c_decl_specifier decl(c_token_get());
      decl.type_specifier = 1;

      if (1 == context.i_am_in_parameter_declaration)
        {
          context.param_vector_decl_specifier.push_back(decl);
        }
      else
        {
          semantic.push_back_vector_decl_specifier(decl);
        }

      return 1;
    }

  context = context_tokens.restore();
  return 0;
}
/*----------------------------------------------------------------------------*/
/*
 * type_specifier: simple_type_specifier | class_specifier |
 * enum_specifier | elaborated_type_specifier | cv_qualifier ;
 */
// ## todo rest of | ...
int c_parser_descent::type_specifier(string tab)
{
  trace(tab, "## type_specifier");

  c_context_tokens context_tokens(context);

  if (1 == simple_type_specifier(tab))
    {
      return 1;
    }

  if (1 == class_specifier(tab))
    {
      context_tokens.restore_but_not_i_token(context);
      return 1;
    }

  if (1 == cv_qualifier(tab))
    {
      return 1;
    }

  context = context_tokens.restore();

  return 0;
}

/*----------------------------------------------------------------------------*/
/*
 * simple_type_specifier: COLONCOLON_opt nested_name_specifier_opt
 * type_name | CHAR | WCHAR_T | BOOL | SHORT | INT | LONG | SIGNED |
 * UNSIGNED | FLOAT | DOUBLE | VOID ;
 */
int c_parser_descent::simple_type_specifier(string tab)
{
  trace(tab, "## simple_type_specifier");

  // ## todo COLONCOLON_opt nested_name_specifier_opt type_name

  string class_name = context.class_name_declaration;

  int result = 0;
  int has_colon_colon_after = 0;
  c_context_tokens context_tokens(context);

  /*
    i think this is to consume for example

      void A::f1(void){... <- A

      A get_A(void){... <- A

    but not constructors (this must be consumed in declarator:)
      A();
  */
  if (1 == type_name(tab))
    {
      c_context_tokens context_tokens_0(context);
      //pre-analisys
      token_next(tab);
      if ( '(' != token_get())
        {
          result = 1;
        }

      if ( COLONCOLON == token_get())
        {
          has_colon_colon_after = 1;
//          context = context_tokens_0.restore();
//          return 1;
        }

      context = context_tokens_0.restore();

    }

  if ( 0 == result )
    {
      token_next(tab);
      context.class_name_declaration = class_name;
    }

  if (CHAR == token_get())
    {
      result = 1;
    }

  if (WCHAR_T == token_get())
    {
      result = 1;
    }

  if (BOOL == token_get())
    {
      result = 1;
    }

  if (SHORT == token_get())
    {
      result = 1;
    }

  if (INT == token_get())
    {
      result = 1;
    }

  if (LONG == token_get())
    {
      result = 1;
    }

  if (SIGNED == token_get())
    {
      result = 1;
    }

  if (UNSIGNED == token_get())
    {
      result = 1;
    }

  if (FLOAT == token_get())
    {
      result = 1;
    }

  if (DOUBLE == token_get())
    {
      result = 1;
    }

  if (VOID == token_get())
    {
      result = 1;
    }

  if (1 == result)
    {
      c_decl_specifier decl(c_token_get());
      decl.type_specifier = 1;
      decl.has_colon_colon_after = has_colon_colon_after;

      if (1 == context.i_am_in_parameter_declaration)
        {
          context.param_vector_decl_specifier.push_back(decl);
        }
      else
        {
          semantic.push_back_vector_decl_specifier(decl);
        }

      return 1;
    }

  context = context_tokens.restore();

  return 0;
}

/*
function_specifier:
	INLINE
	| VIRTUAL
	| EXPLICIT
	;
*/
int c_parser_descent::function_specifier(string tab)
{
  trace(tab, "## function_specifier");

  int result = 0;
  c_context_tokens context_tokens(context);
  token_next(tab);

  if ( INLINE == token_get())
    {
      result = 1;
    }

  if ( VIRTUAL == token_get())
    {
      result = 1;
    }

  if ( EXPLICIT == token_get())
    {
      result = 1;
    }

  if (1 == result)
    {
      c_decl_specifier decl(c_token_get());
      decl.type_specifier = 1;

      if (1 == context.i_am_in_parameter_declaration)
        {
          context.param_vector_decl_specifier.push_back(decl);
        }
      else
        {
          semantic.push_back_vector_decl_specifier(decl);
        }

      return 1;
    }

  context = context_tokens.restore();
  return 0;
}

/*
  rule mine

*/

int c_parser_descent::ptr_specifier(string tab)
{
  trace(tab, "## ptr_specifier");

  int result = 0;
  c_context_tokens context_tokens(context);
  token_next(tab);

  if ('*' == token_get())
    {
      result = 1;
    }

  if ('&' == token_get())
    {
      result = 1;
    }

  if (1 == result)
    {
      c_decl_specifier decl(c_token_get());
      decl.type_specifier = 1;

      if (1 == context.i_am_in_parameter_declaration)
        {
          context.param_vector_decl_specifier.push_back(decl);
        }
      else
        {
          semantic.push_back_vector_decl_specifier(decl);
        }

      return 1;
    }

  context = context_tokens.restore();
  return 0;
}

/*
type_name:
	class_name
	| enum_name
	| typedef_name
	;
*/
int c_parser_descent::type_name(string tab)
{
  trace(tab, "## type_name");

  if (1 == class_name(tab))
    {
      return 1;
    }

  if (1 == typedef_name(tab))
    {
      return 1;
    }

  //## todo rest

  return 0;
}

/*----------------------------------------------------------------------------*/
/*
 * class_specifier: class_head '{' member_specification_opt '}' ;
 *
 * this rule was expanded to process:
 *   typedef class {};
 * and
 *   typedef class A t_A;
 */
// ## todo rest of rule
int c_parser_descent::class_specifier(string tab)
{
  trace(tab, "## class_specifier");

  int was_typedef = 0;
  string class_name_previous = context.class_name_declaration;

  was_typedef = context.is_typedef;
  context.is_typedef = 0;

  //maybe will need read class other time
  c_context_tokens context_tokens_1(context);

  if (0 == class_head(tab))
    {
      context.class_name_declaration = class_name_previous;
      return 0;
    }

  //we can get this point wihout identifer of class
  // typedef class A t_A; in this clase
  // A is not a IDENTIFIER is CLASS_NAME -> enter in next if

  if ( 0 == context.class_name_declaration.size() )
    {
      if ( 1 == was_typedef )
        {
          context = context_tokens_1.restore();
          if (0 == class_key(tab))
            {
              context.class_name_declaration = class_name_previous;
              return 0;
            }

          context.class_specifier_status = CLASS_SPECIFIER_STATUS_IDENTIFIER;

          c_context_tokens context_tokens_2(context);
          token_next(tab);
          // typedef class {};
          if ( '{' == token_get())
            {
              c_token no_identifier(IDENTIFIER,(char *)NO_CLASS_NAME);
              semantic.identifier(context, no_identifier);
            }
          else
            {
              //typedef class A t_A;
              if ( CLASS_NAME == token_get() )
                {
                  context.class_name_declaration = c_token_get().text;
                }

              context.is_typedef = was_typedef;
              if ( 1 == identifier(tab))
                {
                  context.is_typedef = 0;
                  context.class_name_declaration = class_name_previous;
                  return 1;
                }
            }
          /*
                    context.is_typedef = 0;

                    return 1;
          */

          // '{' should be in the buffer
          context = context_tokens_2.restore();
          context.class_name_declaration = NO_CLASS_NAME;
        }
      else
        {
          printf("error c_parser_descent::class_specifier() class without name\n");
          context.class_name_declaration = class_name_previous;
          return 0;
        }
    }

  c_context_tokens context_tokens(context);

  // context.class_specifier = 1;

  token_next(tab);
  if ('{' != token_get())
    {
      context = context_tokens.restore();
      context.class_name_declaration = class_name_previous;
      return 0;
    }

  printf("%s## class_specifier {\n", tab.c_str());

  // we need to know what class is processing
  string class_name = context.class_name_declaration;
  tokens_vector_clear();
  context.class_name_declaration = class_name;

  context_tokens.save(context);

  member_specification_opt(tab);

  token_next(tab);
  if ('}' == token_get())
    {

      printf("%s## class_specifier }\n", tab.c_str());

      context.is_typedef = was_typedef;
      if ( 1 == context.is_typedef)
        {
          if ( 1 != identifier(tab))
            {
              context = context_tokens.restore();
              context.class_name_declaration = class_name_previous;
              return 0;
            }
        }

      context.is_typedef = 0;

      tokens_vector_clear();
      context.class_name_declaration = class_name_previous;
      return 1;
    }

  context.is_typedef = was_typedef;

  context = context_tokens.restore();
  context.class_name_declaration = class_name_previous;
  return 0;
}

/*----------------------------------------------------------------------------*/
/*
 * class_head: class_key identifier_opt base_clause_opt | class_key
 * nested_name_specifier identifier base_clause_opt ;
 */
// ## todo next |
int c_parser_descent::class_head(string tab)
{
  trace(tab, "## class_head");

  if (0 == class_key(tab))
    {
      return 0;
    }

  context.class_specifier_status = CLASS_SPECIFIER_STATUS_IDENTIFIER;
  // ## what happend if have not name ?
  identifier_opt(tab);

  context.class_specifier_status =
    CLASS_SPECIFIER_STATUS_BASE_DECLARATION;

  base_clause_opt(tab);

  // printf( " #### class_head-> [%s]\n",token_identifier.text.c_str()
  // );
  return 1;
}

/*----------------------------------------------------------------------------*/
/*
 * class_key: CLASS | STRUCT | UNION ;
 */
int c_parser_descent::class_key(string tab)
{
  trace(tab, "## class_key");

  // #### here is the error
  // token_next() pass the class token
  c_context_tokens context_tokens(context);
  token_next(tab);

  if (CLASS == token_get())
    {
      trace(tab, "## class_key -> CLASS [ok]");
      context.class_key = CLASS;
      context.access_specifier = PRIVATE;
      return 1;
    }

  if (STRUCT == token_get())
    {
      context.class_key = STRUCT;
      context.access_specifier = PUBLIC;
      return 1;
    }

  if (UNION == token_get())
    {
      context.class_key = UNION;
      context.access_specifier = PUBLIC;
      return 1;
    }

  context = context_tokens.restore();

  return 0;
}

/*----------------------------------------------------------------------------*/
/*
 * member_specification: member_declaration member_specification_opt |
 *                       access_specifier ':' member_specification_opt ;
 *
 * member_specification_opt have while to drop the recursion
 */
int c_parser_descent::member_specification(string tab)
{
  trace(tab, "## member_specification");

  if (0 == member_declaration(tab))
    {
      //## working to define classes inside classes
      if ( 1 == simple_declaration(tab) )
        {
          return 1;
        }
      // | access_specifier ':'
      if (0 == access_specifier(tab))
        {
          return 0;
        }
      // this information
      // private public protected
      // -> context.access_specifier

      c_context_tokens context_tokens(context);
      token_next(tab);

      if (':' != token_get())
        {
          context = context_tokens.restore();
          return 0;
        }
    }

  return 1;
}

/*----------------------------------------------------------------------------*/
/*
 * member_declaration: decl_specifier_seq_opt member_declarator_list_opt
 * ';' | function_definition SEMICOLON_opt | qualified_id ';' |
 * using_declaration | template_declaration ;
 */
int c_parser_descent::member_declaration(string tab)
{
  trace(tab, "## member_declaration");
  c_context_tokens context_tokens(context);

  context.class_specifier_status =
    CLASS_SPECIFIER_STATUS_MEMBER_SPECIFIER;
  semantic.clear_decl_specifier();
  decl_specifier_seq_opt(tab);

  //friend class A;
  //have not member_declarator_list_opt
  if ( CLASS_SPECIFIER_STATUS_FRIEND_DECLARATOR == context.class_specifier_status )
    {
      token_next(tab);

      if (';' == token_get())
        {
          tokens_vector_clear();
          return 1;
        }
      tokens_vector_clear();
      return 1;
    }

  context.class_specifier_status =
    CLASS_SPECIFIER_STATUS_MEMBER_DECLARATOR;

//  c_context_tokens context_tokens_2(context);

  if (1 == member_declarator_list_opt(tab))
    {

      token_next(tab);

      if (';' == token_get())
        {
          tokens_vector_clear();
          return 1;
        }
    }

  context = context_tokens.restore();

  context.class_specifier_status =
    CLASS_SPECIFIER_STATUS_MEMBER_SPECIFIER;

  context.i_am_in_member = 1;
  semantic.clear_decl_specifier();

  if ( 1 == function_definition(tab) )
    {
      //SEMICOLON_opt(tab);
      semantic.declarator_insert(tab, context);
      context.i_am_in_member = 0;
      tokens_vector_clear();
      return 1;
    }

  context = context_tokens.restore();
  return 0;
}

/*----------------------------------------------------------------------------*/
/*
 * member_declarator_list: member_declarator | member_declarator_list ','
 * member_declarator ;
 *
 * example in int a, b, c, d consume a, b, c, d
 */
int c_parser_descent::member_declarator_list(string tab)
{
  trace(tab, "## member_declarator_list");

  c_context_tokens context_tokens(context);
  c_context_tokens context_good_way(context);

  if (0 == member_declarator(tab))
    {
      return 0;
    }

  for (;;)
    {
      context_good_way.save(context);
      token_next(tab);

      if (';' == token_get())
        {
          // yes i restore here to consume ';' more up in the tree
          context = context_good_way.restore();
          return 1;
        }

      if (',' != token_get())
        {
          context = context_tokens.restore();
          return 1;
        }

      if (0 == member_declarator(tab))
        {
          context = context_tokens.restore();
          return 0;
        }
    }


  return 1;
}

/*----------------------------------------------------------------------------*/
/*
 * member_declarator: declarator pure_specifier_opt | declarator
 * constant_initializer_opt | identifier_opt ':' constant_expression ;
 */
int c_parser_descent::member_declarator(string tab)
{
  trace(tab, "## member_declarator");

  // declarator pure_specifier_opt
  context.i_am_in_member = 1;
  if (1 == declarator(tab))
    {
      // if( 1 == pure_specifier_opt(tab) ) //## todo
      {
        context.i_am_in_member = 0;
        return 1;
      }
    }

  context.i_am_in_member = 0;
  context.member_declaration = "";
  // ## todo rest of | ...

  return 0;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------
 * Derived classes.
 *----------------------------------------------------------------------*/
/*
 * base_clause: ':' base_specifier_list ;
 */
int c_parser_descent::base_clause(string tab)
{
  trace(tab, "## base_clause");

  c_context_tokens context_tokens(context);
  token_next(tab);

  if (':' != token_get())
    {
      context = context_tokens.restore();
      return 0;
    }

  if (0 == base_specifier_list(tab))
    {
      context = context_tokens.restore();
      return 0;
    }

  return 1;
}

/*----------------------------------------------------------------------------*/
/*
 * base_specifier_list: base_specifier | base_specifier_list ','
 * base_specifier ;
 */
int c_parser_descent::base_specifier_list(string tab)
{
  trace(tab, "## base_specifier_list");

  if (0 == base_specifier(tab))
    {
      return 0;
    }

  c_context_tokens context_tokens(context);

  for (;;)
    {
      context_tokens.save(context);
      token_next(tab);
      if (',' != token_get())
        {
          context = context_tokens.restore();
          return 1;
        }

      if (0 == base_specifier(tab))
        {
          context = context_tokens.restore();
          return 0;
        }
    }

  context = context_tokens.restore();
  return 0;
}

/*----------------------------------------------------------------------------*/
/*
 * base_specifier: COLONCOLON_opt nested_name_specifier_opt class_name |
 * VIRTUAL access_specifier_opt COLONCOLON_opt nested_name_specifier_opt
 * class_name | access_specifier VIRTUAL_opt COLONCOLON_opt
 * nested_name_specifier_opt class_name ;
 */
int c_parser_descent::base_specifier(string tab)
{
  trace(tab, "## base_specifier");


  // ##todo rest of rules


  // | access_specifier VIRTUAL_opt COLONCOLON_opt
  // nested_name_specifier_opt class_name

  if (0 == access_specifier(tab))
    {
      // ## this dont must break the down
      // return 0;
    }
  // ## todo midle

  context.class_specifier_status =
    CLASS_SPECIFIER_STATUS_BASE_DECLARATION;

  if (1 == class_name(tab))
    {
      return 1;
    }


  return 0;
}

/*----------------------------------------------------------------------------*/
/*
 * access_specifier: PRIVATE | PROTECTED | PUBLIC ;
 */
int c_parser_descent::access_specifier(string tab)
{
  trace(tab, "## access_specifier");

  c_context_tokens context_tokens(context);
  token_next(tab);

  if (PRIVATE == token_get())
    {
      context.access_specifier = PRIVATE;
      return 1;
    }

  if (PROTECTED == token_get())
    {
      context.access_specifier = PROTECTED;
      return 1;
    }

  if (PUBLIC == token_get())
    {
      context.access_specifier = PUBLIC;
      return 1;
    }

  context = context_tokens.restore();

  return 0;
}

/*----------------------------------------------------------------------
 * Epsilon (optional) definitions.
 *----------------------------------------------------------------------*/
/*
 * declaration_seq_opt: //epsilon | declaration_seq ;
 */
// todo epsilon
int c_parser_descent::declaration_seq_opt(string tab)
{
  trace(tab, "## declaration_seq_opt");
  if (1 == declaration_seq(tab))
    {
      return 1;
    }

  return 1;
}

/*----------------------------------------------------------------------------*/
/*
COLONCOLON_opt:
	// epsilon
	| COLONCOLON
	;
*/
int c_parser_descent::COLONCOLON_opt(string tab)
{
  trace(tab, "## COLONCOLON_opt");

  c_context_tokens context_tokens(context);

  token_next(tab);
  if (COLONCOLON == token_get())
    {
      return 1;
    }

  context = context_tokens.restore();
  return 0;
}

/*----------------------------------------------------------------------------*/
/*
 * decl_specifier_seq_opt: //epsilon | decl_specifier_seq ;
 */
int c_parser_descent::decl_specifier_seq_opt(string tab)
{
  trace(tab, "## decl_specifier_seq_opt");

  return decl_specifier_seq(tab);
}
/*----------------------------------------------------------------------------*/
/*
statement_seq_opt:
	// epsilon
	| statement_seq
	;
*/
int c_parser_descent::statement_seq_opt(string tab)
{
  trace(tab, "## statement_seq_opt");

  return statement_seq(tab);
}
/*----------------------------------------------------------------------------*/
/*
 * init_declarator_list_opt: //epsilon | init_declarator_list ;
 */
int c_parser_descent::init_declarator_list_opt(string tab)
{
  trace(tab, "## init_declarator_list_opt");

  return init_declarator_list(tab);
}

/*----------------------------------------------------------------------------*/
/*
 * identifier_opt: // epsilon | identifier ;
 */
int c_parser_descent::identifier_opt(string tab)
{
  trace(tab, "## identifier_opt");

  return identifier(tab);
}

/*
 * parameter_declaration_list_opt: // epsilon | parameter_declaration_list
 * ;
 */
int



c_parser_descent::parameter_declaration_list_opt(string tab)
{
  trace(tab, "## parameter_declaration_list_opt");

  return parameter_declaration_list(tab);
}

/*
 * ELLIPSIS_opt: // epsilon | ELLIPSIS ;
 */
int c_parser_descent::ELLIPSIS_opt(string tab)
{
  trace(tab, "## ELLIPSIS_opt");

  c_context_tokens context_tokens(context);

  token_next(tab);
  if (ELLIPSIS == token_get())
    {
      c_decl_specifier decl(c_token_get());
      decl.type_specifier = 1;

      if (1 == context.i_am_in_parameter_declaration)
        {
          context.param_vector_decl_specifier.push_back(decl);
        }

      c_token token(IDENTIFIER, (char *) "...");
      semantic.identifier(context, token);

      return 1;
    }

  context = context_tokens.restore();
  return 0;
}

/*----------------------------------------------------------------------------*/
/*
 * member_specification_opt: epsilon | member_specification ;
 *
 * i try to drop indirect recursion member_specification_opt ->
 * member_declaration -> member_specification_opt ...
 */
int c_parser_descent::member_specification_opt(string tab)
{
  trace(tab, "## member_specification_opt");

  int result = 0;

  while (1 == member_specification(tab))
    {
      result = 1;
    }

  return result;
}

/*----------------------------------------------------------------------------*/
/*
 * base_clause_opt: // epsilon | base_clause ;
 */
int c_parser_descent::base_clause_opt(string tab)
{
  trace(tab, "## base_clause_opt");

  return base_clause(tab);
}

/*----------------------------------------------------------------------------*/
/*
 * member_declarator_list_opt: epsilon | member_declarator_list ;
 */
int c_parser_descent::member_declarator_list_opt(string tab)
{
  trace(tab, "## member_declarator_list_opt");

  return member_declarator_list(tab);
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------
 * Declarators.
 *----------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*
 * init_declarator_list: init_declarator | init_declarator_list ','
 * init_declarator ;
 */
int c_parser_descent::init_declarator_list(string tab)
{
  trace(tab, "## init_declarator_list");
  c_context_tokens context_tokens(context);
  c_context_tokens context_good_way(context);

  if (0 == init_declarator(tab))
    {
      return 0;
    }

  for (;;)
    {
      context_good_way.save(context);
      token_next(tab);

      if (';' == token_get())
        {
          // yes i restore here to consume ';' more up in the tree
          context = context_good_way.restore();
          return 1;
        }

      if (',' != token_get())
        {
          context = context_tokens.restore();
          return 1;
        }

      if (0 == init_declarator(tab))
        {
          context = context_tokens.restore();
          return 0;
        }
    }

  return 1;
}

/*----------------------------------------------------------------------------*/
/*
 * init_declarator: declarator initializer_opt ;
 */
int c_parser_descent::init_declarator(string tab)
{
  trace(tab, "## init_declarator");

  if (1 == declarator(tab))
    {
      // if( 1 == initializer_opt(tab) ) //## todo
      {
        return 1;
      }
    }

  return 0;
}

/*----------------------------------------------------------------------------*/
/*
 * declarator: direct_declarator | ptr_operator declarator ;
 */
int c_parser_descent::declarator(string tab)
{
  trace(tab, "## declarator");

  if (1 == direct_declarator(tab))
    {
      return 1;
    }

  // | ptr_operator declarator
  /*##
    i did this in the part of decl_specifier
  if( 1 == ptr_operator(tab))
    {
      return declarator(tab);
    }
  */

  return 0;
}

/*----------------------------------------------------------------------------*/
/*
 * direct_declarator: declarator_id
 *  | direct_declarator
 *    '('parameter_declaration_clause ')' cv_qualifier_seq_opt
 *    exception_specification_opt
 *  | direct_declarator '[' constant_expression_opt ']'
 *  | '(' declarator ')'
 *  ;
 *
 * changing a litle the original order to ser clearly the terminals of
 * the recursion
 *
 * direct_declarator: declarator_id | '(' declarator ')'
 *
 * | direct_declarator '('parameter_declaration_clause ')'
 * cv_qualifier_seq_opt exception_specification_opt | direct_declarator
 * '[' constant_expression_opt ']' ;


 to parse this:
   int A::A_2::A_2_1::A_2_1_f(int long p1)
 it need to recover the position of A to use in qualified_id
 */
int c_parser_descent::direct_declarator(string tab)
{
  trace(tab, "## direct_declarator");
  tokens_vector_print_from_actual();

//  c_context_tokens qualified_id_context(context);
//  qualified_id_context = context;
  c_context_tokens context_tokens(context);
  c_context_tokens context_good_way(context);

  int result = 0;
  for (;;)
    {
      result = 0;

      if (1 == declarator_id(tab))
        {
          result = 1;

          context_good_way.save(context);
          token_next(tab);
          if ('(' == token_get())
            {
              if (1 == parameter_declaration_clause(tab))
                {
                  printf("### 1 == parameter_declaration_clause(tab)\n");
                }
              else  if (1 == declarator(tab))
                {
                  printf("### 1 == declarator(tab)\n");
                }
              else
                {
                  printf("### no, we are not in a member function !\n");
                  context = context_tokens.restore();
                  return 0;
                }

              token_next(tab);
              if (')' != token_get())
                {
                  context = context_tokens.restore();
                  return 0;
                }

              context_good_way.save(context);
              token_next(tab);

              if (';' == token_get()
                 )
                {
                  printf("### yes we are in a function !\n");
                  semantic.declarator_insert(tab, context);
                  // yes i restore here to consume ';' more up in the
                  // tree
                  context = context_good_way.restore();
                  return 1;
                }
              // this is posible:
              // int f1(void), f2(void);
              if (',' == token_get() )
                {
                  printf("### yes we are in a function !\n");
                  semantic.declarator_insert(tab, context);
                  // yes i restore here to consume ',' more up in the
                  // tree
                  context = context_good_way.restore();
                  return 1;
                }
            }
          else
            {
              // ## todo | direct_declarator '[' constant_expression_opt
              // ']'

              if (1 != context.class_member.is_function)
                {
                  semantic.declarator_insert(tab, context);
                }
            }
        }

      if (1 == result)
        {
          context = context_good_way.restore();
          return 1;
        }
      else
        {
          context = context_tokens.restore();
          return 0;
        }
    }

  return 0;
}
/*----------------------------------------------------------------------------*/
/*
ptr_operator:
	'*' cv_qualifier_seq_opt
	| '&'
	| COLONCOLON_opt nested_name_specifier '*' cv_qualifier_seq_opt
	;
*/
int c_parser_descent::ptr_operator(string tab)
{
  trace(tab, "## ptr_operator");

  c_context_tokens context_tokens(context);
  token_next(tab);

  if ('*' == token_get())
    {
      return 1;
    }

  if ('&' == token_get())
    {
      return 1;
    }

  //## todo rest ...

  context = context_tokens.restore();
  return 0;
}

/*----------------------------------------------------------------------------*/
/*
cv_qualifier_seq:
	cv_qualifier cv_qualifier_seq_opt
	;
*/
int c_parser_descent::cv_qualifier_seq(string tab)
{
  trace(tab, "## cv_qualifier_seq");
  return 0;
}
/*----------------------------------------------------------------------------*/
/*
cv_qualifier:
	CONST
	| VOLATILE
	;
*/
int c_parser_descent::cv_qualifier(string tab)
{
  trace(tab, "## cv_qualifier");
  int result = 0;
  c_context_tokens context_tokens(context);
  token_next(tab);

  if ( CONST == token_get())
    {
      result = 1;
    }

  if ( VOLATILE == token_get())
    {
      result = 1;
    }

  if (1 == result)
    {
      c_decl_specifier decl(c_token_get());
      decl.type_specifier = 1;

      if (1 == context.i_am_in_parameter_declaration)
        {
          context.param_vector_decl_specifier.push_back(decl);
        }
      else
        {
          semantic.push_back_vector_decl_specifier(decl);
        }

      return 1;
    }

  context = context_tokens.restore();
  return 0;
}
/*----------------------------------------------------------------------------*/
/*
 * declarator_id: COLONCOLON_opt id_expression | COLONCOLON_opt
 * nested_name_specifier_opt type_name ;
 */
int c_parser_descent::declarator_id(string tab)
{
  trace(tab, "## declarator_id");

  //## whe must have a class name before of this
  COLONCOLON_opt(tab);

  if (1 == id_expression(tab))
    {
      return 1;
    }

  //|
  //## nested_name_specifier_opt

  //contructors enter for here
  if (1 == type_name(tab))
    {
      return 1;
    }

  return 0;
}

/*----------------------------------------------------------------------------*/
/*
 * parameter_declaration_clause: parameter_declaration_list_opt
 * ELLIPSIS_opt | parameter_declaration_list ',' ELLIPSIS ;
 *
 * ## i really dont undertand this 2 rules i think first is a super set of
 * second ... :-S
 */
int c_parser_descent::parameter_declaration_clause(string tab)
{
  trace(tab, "## parameter_declaration_clause");
  context.i_am_in_parameter_declaration = 1;
  parameter_declaration_list_opt(tab);

  ELLIPSIS_opt(tab);

  context.i_am_in_parameter_declaration = 0;

  c_context_tokens context_good_way(context);
  context_good_way.save(context);
  token_next(tab);

  if (')' == token_get())
    {
      context = context_good_way.restore();
      return 1;
    }

  return 0;
}

/*
 * parameter_declaration_list: parameter_declaration |
 * parameter_declaration_list ',' parameter_declaration ;
 */
int c_parser_descent::parameter_declaration_list(string tab)
{
  trace(tab, "## parameter_declaration_list");

  c_context_tokens context_good_way(context);
  context.param_vector_decl_specifier.clear();

  if (0 == parameter_declaration(tab))
    {
      c_context_tokens context_tokens(context);
      context_good_way.save(context);
      token_next(tab);

      if (')' == token_get())
        {
          context = context_good_way.restore();
          return 1;
        }

      context = context_tokens.restore();
      return 0;
    }

  c_context_tokens context_tokens(context);

  for (;;)
    {
      context_good_way.save(context);
      token_next(tab);

      if (')' == token_get())
        {
          context = context_good_way.restore();
          return 1;
        }

      if (',' != token_get())
        {
          context = context_tokens.restore();
          return 1;
        }

      context.param_vector_decl_specifier.clear();

      if (0 == parameter_declaration(tab))
        {
          context = context_tokens.restore();
          return 0;
        }
    }

  return 0;
}

/*
 * parameter_declaration: decl_specifier_seq declarator |
 * decl_specifier_seq declarator '=' assignment_expression |
 * decl_specifier_seq abstract_declarator_opt | decl_specifier_seq
 * abstract_declarator_opt '=' assignment_expression ;
 *
 * a function can be declared f1( int ); without declarator
 */
int c_parser_descent::parameter_declaration(string tab)
{
  trace(tab, "## parameter_declaration");
  if (0 == decl_specifier_seq(tab))
    {
      return 0;
    }

  if (1 == declarator(tab))
    {
      // ## todo the rest...
      return 1;
    }

  if (VOID == token_get())
    {
      c_token token(IDENTIFIER, (char *) "void");
      semantic.identifier(context, token);
      return 1;
    }

  c_token token(IDENTIFIER, (char *) NO_IDENTIFIER);
  semantic.identifier(context, token);

  return 1;
}

/*----------------------------------------------------------------------------*/
/*
 * function_definition: decl_specifier_seq_opt declarator
 * ctor_initializer_opt function_body | decl_specifier_seq_opt declarator
 * function_try_block ;
 */
int c_parser_descent::function_definition(string tab)
{
  trace(tab, "## function_definition");

  decl_specifier_seq_opt(tab);

  if ( CLASS_SPECIFIER_STATUS_MEMBER_SPECIFIER ==
       context.class_specifier_status )
    {
      context.class_specifier_status =
        CLASS_SPECIFIER_STATUS_MEMBER_DECLARATOR;
    }

  if (0 == declarator(tab))
    {
      return 0;
    }

  //## ctor_initializer_opt

  if (0 == function_body(tab))
    {
      return 0;
    }

  return 1;
}

/*----------------------------------------------------------------------------*/
/*
function_body:
	compound_statement
	;
*/
int c_parser_descent::function_body(string tab)
{
  trace(tab, "## function_body");

  return compound_statement(tab);
}


/*----------------------------------------------------------------------------*/
