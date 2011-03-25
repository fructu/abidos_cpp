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

#include "semantic.h"
/*----------------------------------------------------------------------
 * error_recover.
 *----------------------------------------------------------------------*/
int c_parser_descent::error_recover(void)
{
  while( token_get() != 0 )
  {
    token_next();
	if( ';' == token_get() )
	{
		return 1;
	}

	if( '}' == token_get() )
	{
		return 1;
	}
	if( ')' == token_get() )
	{
		return 1;
	}
  }

  printf("\nc_parser_descent::error_recover() -> token_get() == 0 !! \n");
  return 0;
}
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
  printf("\n## translation_unit(void)-----------------------------------\n");
	tokens_vector_clear();

	if( 1 == declaration_seq_opt() )
	{
		return 1;
	}
/*
	if( 1 == error_recover() )
	{	
		return 1;
	}
*/
	c_context_tokens context_tokens(context);
	token_next();
	if( 0 == token_get() )
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
id_expression:
    unqualified_id
    | qualified_id
    ;
*/
int c_parser_descent::id_expression(void)
{
  
  if( 1 == unqualified_id() )
  {
    return 1;
  }

  if( 1 == qualified_id() )
  {
    return 1;
  }

  return 0;
}
/*
unqualified_id:
    identifier
    | operator_function_id
    | conversion_function_id
    | '~' class_name
    | template_id
    ;
*/
int c_parser_descent::unqualified_id(void)
{
  
  if( 1 == identifier() )
  {    
    return 1;
  }

  return 0;
}

/*
qualified_id:
    nested_name_specifier TEMPLATE_opt unqualified_id
    ;
*/    
int c_parser_descent::qualified_id(void)
{

  return 0;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------
 * Context-dependent identifiers.
 *----------------------------------------------------------------------*/
/*
class_name:
//identifier
	CLASS_NAME
	| template_id
	;
*/
int c_parser_descent::class_name(void)
{
	c_context_tokens context_tokens(context);

	token_next();

	if( CLASS_NAME == token_get() )
	{
        semantic.class_name( context, c_token_get() );
		return 1;
	}

//##todo | template_id

	context = context_tokens.restore();
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
int c_parser_descent::identifier(void)
{
	c_context_tokens context_tokens(context);
    
    tokens_vector_print_from_actual();
	token_next();
    tokens_vector_print_from_actual();
	if( IDENTIFIER == token_get() )
	{
        semantic.identifier( context, c_token_get() );
		return 1;
	}

	context = context_tokens.restore();
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

	int result = 0;

	while( 1 == declaration() )
	{
		printf("#----------------------------------------declaration [ok]\n\n");
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

	if( 1 == simple_declaration() )
	{
		return 1;
	}

//	tokens_vector_reload();


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
	
	decl_specifier_seq_opt();
	init_declarator_list_opt();

	c_context_tokens context_tokens(context);

	token_next();

	if( ';' == token_get() )
	{
        tokens_vector_clear();
		return 1;
	}

	context = context_tokens.restore();
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
  
    //decl_specifier_seq_opt->decl_specifier_seq->decl_specifier_seq_opt...INFINITE
    //decl_specifier_seq_opt();

    int result = 0;
    c_context_tokens context_tokens(context);
    
	while( 1 == decl_specifier() )
	{
        tokens_vector_print_from_actual();
        result = 1;
	}

    if( 1 == result )
    {
      token_next();
      token_next();
    }
    else
    {
      context = context_tokens.restore();
    }
    
        
        tokens_vector_print_from_actual();

	return result;
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
  
    if( 1 == simple_type_specifier() )
    {
      return 1;
    }

	if( 1 == class_specifier() )
	{
		return 1;
	}

	return 0;
}
/*----------------------------------------------------------------------------*/
/*
simple_type_specifier:
    COLONCOLON_opt nested_name_specifier_opt type_name
    | CHAR
    | WCHAR_T
    | BOOL
    | SHORT
    | INT
    | LONG
    | SIGNED
    | UNSIGNED
    | FLOAT
    | DOUBLE
    | VOID
    ;
*/
int c_parser_descent::simple_type_specifier(void)
{
  
  //## todo  COLONCOLON_opt nested_name_specifier_opt type_name
  c_context_tokens context_tokens(context);
  token_next();

  if( CHAR == token_get())
  {
    return 1;
  }

  if( WCHAR_T == token_get())
  {
    return 1;
  }

  if( BOOL == token_get())
  {
    return 1;
  }

  if( SHORT == token_get())
  {
    return 1;
  }

  if( INT == token_get())
  {
    return 1;
  }

  if( LONG == token_get())
  {
    return 1;
  }

  if( SIGNED == token_get())
  {
    return 1;
  }
  
  if( UNSIGNED == token_get())
  {
    return 1;
  }
    
  if( FLOAT == token_get())
  {
    return 1;
  }
  
  if( DOUBLE == token_get())
  {
    return 1;
  }

  if( VOID == token_get())
  {
    return 1;
  }

  context = context_tokens.restore();  
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

tokens_vector_print_from_actual();

	if( 0 == class_head() )
	{
		return 0;
	}

	c_context_tokens context_tokens(context);
	token_next();
	if( '{' != token_get() )
	{

		context = context_tokens.restore();
		return 0;
	}

    tokens_vector_clear();
    context_tokens.save(context);

	member_specification_opt();

	token_next();
	if( '}' != token_get() )
	{
		context = context_tokens.restore();
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

	if( 0 == class_key() )
	{
		return 0;
	}

    context.class_declaration = BASE_CLASS_DECLARATION;

	identifier_opt();

	base_clause_opt();

//	printf( " #### class_head-> [%s]\n",token_identifier.text.c_str() );

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

  //#### here is the error
  // token_next() pass the class token
	c_context_tokens context_tokens(context);
	token_next(); 

	if( CLASS == token_get() )
	{
        context.class_key = CLASS;
		return 1;
	}

	if( STRUCT == token_get() )
	{
        context.class_key = STRUCT;
		return 1;
	}

	if( UNION == token_get() )
	{
        context.class_key = UNION;
		return 1;
	}

	context = context_tokens.restore();

	return 0;
}
/*----------------------------------------------------------------------------*/
/*
member_specification:
    member_declaration     member_specification_opt
    | access_specifier ':' member_specification_opt
    ;
    
member_specification_opt have while to drop the recursion    
*/ 
int c_parser_descent::member_specification(void)
{

  if( 0 == member_declaration() )
  {
    //| access_specifier ':'
    if( 0 == access_specifier() )
    {
      return 0;
    }
    //this information
    //private public protected
    //-> context.access_specifier

    c_context_tokens context_tokens(context);
    token_next();

    if( ':' != token_get() )
    {
      context = context_tokens.restore();
      return 0;
    }
  }  

  return 1;
}
/*----------------------------------------------------------------------------*/
/*
member_declaration:
    decl_specifier_seq_opt member_declarator_list_opt ';'
    | function_definition SEMICOLON_opt
    | qualified_id ';'
    | using_declaration
    | template_declaration
    ;
*/
int c_parser_descent::member_declaration(void)
{
  
  decl_specifier_seq_opt();
  
  //## todo save context
  //token_next();

  if( 1 == member_declarator_list_opt() )
  {
    c_context_tokens context_tokens(context);
    token_next();    
    
    if( ';' != token_get() )
    {
        context = context_tokens.restore();
        return 0;
    }   
    
    return 1;
  }

  return 0;
}
/*----------------------------------------------------------------------------*/
/*
member_declarator_list:
    member_declarator
    | member_declarator_list ',' member_declarator
    ;
    
example
  in  
    int a, b, c, d
  consume 
    a, b, c, d  
*/
int c_parser_descent::member_declarator_list(void)
{
  
  if( 0 == member_declarator() )
  {
    return 0;
  }
  
  c_context_tokens context_tokens(context);
  token_next();

  for(;;)
  {
    if( ',' != token_get() )
    {
        context = context_tokens.restore();
        return 1;
    }

    if( 0 == member_declarator() )
    {
      context = context_tokens.restore();
      return 0;
    }
  }
  

  return 1;
}
/*----------------------------------------------------------------------------*/
/*
member_declarator:
    declarator pure_specifier_opt
    | declarator constant_initializer_opt
    | identifier_opt ':' constant_expression
    ;
*/
int c_parser_descent::member_declarator(void)
{
  
  //declarator pure_specifier_opt
  if( 1 == declarator() )
  {
    //if( 1 == pure_specifier_opt() ) //## todo
    {
        return 1;      
    }
  }

  //## todo rest of | ...
  return 0;
}
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------
 * Derived classes.
 *----------------------------------------------------------------------*/
/*
base_clause:
	':' base_specifier_list
	;
*/
int c_parser_descent::base_clause(void)
{

	c_context_tokens context_tokens(context);
	token_next();

	if( ':' != token_get() )
	{
		context = context_tokens.restore();
		return 0;
	}

	if( 0 == base_specifier_list() )
	{
		context = context_tokens.restore();
		return 0;
	}

	return 1;
}
/*----------------------------------------------------------------------------*/
/*
base_specifier_list:
	base_specifier
	| base_specifier_list ',' base_specifier
	;
*/
int c_parser_descent::base_specifier_list(void)
{

	if( 0 == base_specifier() )
	{
		return 0;
	}	

    c_context_tokens context_tokens(context);

    for(;;)
    {
      token_next();    
      if( ',' != token_get() )
      {
        context = context_tokens.restore();
        return 1;
      }

      if( 0 == base_specifier() )
      {
        context = context_tokens.restore();
        return 0;
      }
    }

	return 0;
}
/*----------------------------------------------------------------------------*/
/*
base_specifier:
	COLONCOLON_opt nested_name_specifier_opt class_name
	| VIRTUAL access_specifier_opt COLONCOLON_opt nested_name_specifier_opt class_name
	| access_specifier VIRTUAL_opt COLONCOLON_opt nested_name_specifier_opt class_name
	;
*/
int c_parser_descent::base_specifier(void)
{


	//##todo rest of rules


//  | access_specifier VIRTUAL_opt COLONCOLON_opt nested_name_specifier_opt class_name
	
	if( 0 == access_specifier() )
	{
      //## this dont must break the down
      //return 0;
	}

	//## todo midle


	if( 1 == class_name() )
	{
		return 1;
	}


	return 0;
}
/*----------------------------------------------------------------------------*/
/*
access_specifier:
	PRIVATE
	| PROTECTED
	| PUBLIC
	;
*/
int c_parser_descent::access_specifier(void)
{

	c_context_tokens context_tokens(context);
	token_next();

	if( PRIVATE == token_get() )
	{
        context.access_specifier = PRIVATE;
		return 1;
	}

	if( PROTECTED == token_get() )
	{
        context.access_specifier = PROTECTED;
		return 1;
	}

	if( PUBLIC == token_get() )
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
declaration_seq_opt:
	//epsilon 
	| declaration_seq
	;
*/
//todo epsilon
int c_parser_descent::declaration_seq_opt(void)
{

	if( 1 == declaration_seq() )
	{
		return 1;
	}

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

// ## todo
//	init_declarator_list();

	return 1;
}
/*----------------------------------------------------------------------------*/
/*
identifier_opt:
//	 epsilon 
	| identifier
	;
*/
int c_parser_descent::identifier_opt(void)
{
//	c_token token_identifier;

	identifier();

	return 1;
}
/*----------------------------------------------------------------------------*/
/*
member_specification_opt:
    epsilon 
    | member_specification
    ;
    
i try to drop indirect recursion
member_specification_opt -> member_declaration -> member_specification_opt ...
*/
int c_parser_descent::member_specification_opt(void)
{
    int result = 0;
    
    while( 1 == member_specification() )
    {
       result = 1;
    }

    return result;
}
/*----------------------------------------------------------------------------*/
/*
base_clause_opt:
//	epsilon 
	| base_clause
	;
*/
int c_parser_descent::base_clause_opt(void)
{

	base_clause();

	return 1;
}
/*----------------------------------------------------------------------------*/
/*
member_declarator_list_opt:
    epsilon
    | member_declarator_list
    ;
*/
int c_parser_descent::member_declarator_list_opt(void)
{

    return member_declarator_list();
}
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------
 * Declarators.
 *----------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*
declarator:
    direct_declarator
    | ptr_operator declarator
    ;
*/
int c_parser_descent::declarator(void)
{

    if( 1 == direct_declarator() )
    {
      return 1;
    }
  
  //## todo the rest
    return 1;
}
/*----------------------------------------------------------------------------*/
/*
direct_declarator:
    declarator_id
    | direct_declarator '('parameter_declaration_clause ')' cv_qualifier_seq_opt exception_specification_opt
    | direct_declarator '[' constant_expression_opt ']'
    | '(' declarator ')'
    ;
*/
int c_parser_descent::direct_declarator(void)
{
  
    //## todo while ...
    if( 1 == declarator_id() )
    {
      return 1;
    }

    return 0;
}
/*----------------------------------------------------------------------------*/
/*
declarator_id:
    COLONCOLON_opt id_expression
    | COLONCOLON_opt nested_name_specifier_opt type_name
    ;
*/
int c_parser_descent::declarator_id(void)
{
  
    //## COLONCOLON_opt()
    
    if( 1 == id_expression() )
    {
       return 1;
    }
  
    return 0;
}
/*----------------------------------------------------------------------------*/
/*
function_definition:
    decl_specifier_seq_opt declarator ctor_initializer_opt function_body
    | decl_specifier_seq_opt declarator function_try_block
    ;
*/
int c_parser_descent::function_definition(void)
{
    //## todo the rest
    
    if( 0 == declarator() )
    {
        return 0;
    }

    return 1;
}
/*----------------------------------------------------------------------------*/
