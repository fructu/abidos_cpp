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
#include "parser_descent.h"
#include "tokens.h"
/*----------------------------------------------------------------------
 * Translation unit.
 *----------------------------------------------------------------------*/
/*
translation_unit:
	declaration_seq_opt
	;
*/
int c_parser_descent::translation_unit(void)
{
  printf("## translation_unit(void)\n");

	if( 1 == declaration_seq_opt() )
	{
		return 1;
	}

	return 0;
}
/*----------------------------------------------------------------------
 * Lexical elements.
 *----------------------------------------------------------------------*/
/*
identifier:
	IDENTIFIER
	;
*/
int c_parser_descent::identifier(c_token & token_identifier)
{
  printf("## declaration_seq(void)\n");
	t_tokens::iterator i_token = i_token_actual;

	token_next();
	if( IDENTIFIER == token_get() )
	{
		token_identifier = *i_token_actual;
		return 1;
	}

	// restore
	i_token_actual = i_token;
	return 0;
}

/*----------------------------------------------------------------------
 * Declarations.
 *----------------------------------------------------------------------*/
/*
declaration_seq:
	declaration
	| declaration_seq declaration
	;
*/
int c_parser_descent::declaration_seq(void)
{
  printf("## declaration_seq(void)\n");

	int result = 0;

	while( 1 == declaration() )
	{
		result = 1;
	}

	return result;
}
/*----------------------------------------------------------------------------*/
/*
declaration:
	block_declaration
	| function_definition
	| template_declaration
	| explicit_instantiation
	| explicit_specialization
	| linkage_specification
	| namespace_definition
	;
*/
//## todo rest of | ...
int c_parser_descent::declaration(void)
{
  printf("## declaration(void)\n");

	if( 1 == block_declaration() )
	{
		return 1;
	}

	return 0;
}
/*----------------------------------------------------------------------------*/
/*
block_declaration:
	simple_declaration
	| asm_definition
	| namespace_alias_definition
	| using_declaration
	| using_directive
	;
*/
//## todo rest of | ...
int c_parser_descent::block_declaration(void)
{
  printf("## block_declaration(void)\n");

	if( 1 == simple_declaration() )
	{
		return 1;
	}

	return 0;
}
/*----------------------------------------------------------------------------*/
/*
simple_declaration:
	decl_specifier_seq_opt init_declarator_list_opt ';'
	;
*/
int c_parser_descent::simple_declaration(void)
{
  printf("## simple_declaration(void)\n");
	t_tokens::iterator i_token = i_token_actual;

	decl_specifier_seq_opt();

	init_declarator_list_opt();

	token_next();

	if( ';' == token_get() )
	{
		return 1;
	}

	i_token_actual = i_token;
	return 0;
}
/*----------------------------------------------------------------------------*/
/*
decl_specifier_seq:
	decl_specifier_seq_opt decl_specifier
	;
*/
int c_parser_descent::decl_specifier_seq(void)
{
  printf("## decl_specifier_seq(void)\n");

	if( 1 == decl_specifier() )
	{
		return 1;
	}

	return 0;
}
/*----------------------------------------------------------------------------*/
/*
decl_specifier:
	storage_class_specifier
	| type_specifier
	| function_specifier
	| FRIEND
	| TYPEDEF
	;
*/
//## todo rest of | ...
int c_parser_descent::decl_specifier(void)
{
  printf("## decl_specifier(void)\n");

	if( 1 == type_specifier() )
	{
		return 1;
	}

	return 0;
}
/*----------------------------------------------------------------------------*/
/*
type_specifier:
	simple_type_specifier
	| class_specifier
	| enum_specifier
	| elaborated_type_specifier
	| cv_qualifier
	;
*/
//## todo rest of | ...
int c_parser_descent::type_specifier(void)
{
  printf("## type_specifier(void)\n");

	if( 1 == class_specifier() )
	{
		return 1;
	}

	return 0;
}
/*----------------------------------------------------------------------------*/
/*
class_specifier:
	class_head '{' member_specification_opt '}'
	;
*/
//## todo rest of rule
int c_parser_descent::class_specifier(void)
{
  printf("## class_specifier(void)\n");

	t_tokens::iterator i_token = i_token_actual;

	if( 0 == class_head() )
	{
		i_token_actual = i_token;
		return 0;
	}

	token_next();
	if( '{' != token_get() )
	{
		i_token_actual = i_token;
		return 0;
	}

//## todo
//	member_specification_opt();


	token_next();
	if( '}' != token_get() )
	{
		i_token_actual = i_token;
		return 0;
	}
	
	return 1;
}
/*----------------------------------------------------------------------------*/
/*
class_head:
	class_key identifier_opt base_clause_opt
	| class_key nested_name_specifier identifier base_clause_opt
	;
*/
//## todo next |
int c_parser_descent::class_head(void)
{
  printf("## class_head(void)\n");

	if( 0 == class_key() )
	{
		return 0;
	}

	//## todo
	c_token token_identifier;
	if( 0 == identifier(token_identifier) )
	{		
		return 0;
	}

	printf( " #### class_head-> [%s]\n",token_identifier.text.c_str() );

	return 1;
}
/*----------------------------------------------------------------------------*/
/*
class_key:
	CLASS
	| STRUCT
	| UNION
	;
*/
int c_parser_descent::class_key(void)
{
  printf("## class_key(void)\n");
	t_tokens::iterator i_token = i_token_actual;

	token_next();

	if( CLASS == token_get() )
	{
		return 1;
	}

	if( STRUCT == token_get() )
	{
		return 1;
	}

	if( UNION == token_get() )
	{
		return 1;
	}

	i_token_actual = i_token;
	return 0;
}
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------
 * Epsilon (optional) definitions.
 *----------------------------------------------------------------------*/
/*
declaration_seq_opt:
	//epsilon 
	| declaration_seq
	;
*/
//todo epsilon
int c_parser_descent::declaration_seq_opt(void)
{
  printf("## declaration_seq_opt(void)\n");

	declaration_seq();

	return 1;
}
/*----------------------------------------------------------------------------*/
/*
decl_specifier_seq_opt:
	//epsilon
	| decl_specifier_seq
	;
*/
int c_parser_descent::decl_specifier_seq_opt(void)
{
  printf("## decl_specifier_seq_opt(void)\n");

	decl_specifier_seq();

	return 1;
}
/*----------------------------------------------------------------------------*/
/*
init_declarator_list_opt:
	//epsilon
	| init_declarator_list
	;
*/
int c_parser_descent::init_declarator_list_opt(void)
{
  printf("## init_declarator_list_opt(void)\n");

// ## todo
//	init_declarator_list();

	return 1;
}
/*----------------------------------------------------------------------------*/
