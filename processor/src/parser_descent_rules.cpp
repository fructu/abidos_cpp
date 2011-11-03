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
#include <string.h>
#include "parser_descent.h"
#include "tokens.h"

#include "semantic.h"
#include "trace.h"
/*----------------------------------------------------------------------
 * error_recover.
 *----------------------------------------------------------------------*/
int
c_parser_descent::error_recover(c_trace_node trace_node)
{
    while (token_get() != 0) {
        token_next(trace_node.get_tab());
        if (';' == token_get()) {
            return 1;
        }

        if ('}' == token_get()) {
            return 1;
        }
        if (')' == token_get()) {
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
    c_trace_node trace_node;
    trace_node.set("translation_unit");

    tokens_vector_clear();

    if (1 == declaration_seq_opt(trace_node)) {
        return 1;
    }
    /*
     * if( 1 == error_recover(trace_node) ) { return 1; }
     */
    c_context_tokens context_tokens(context);
    token_next(trace_node.get_tab());
    if (0 == token_get()) {
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
int c_parser_descent::id_expression(c_trace_node trace_node)
{
    trace_graph.add(trace_node, "id_expression");

    if (1 == unqualified_id(trace_node)) {
        return 1;
    }

    if (1 == qualified_id(trace_node)) {
        return 1;
    }

    return 0;
}

/*
 * unqualified_id: identifier | operator_function_id |
 * conversion_function_id | '~' class_name | template_id ;
 */
int c_parser_descent::unqualified_id(c_trace_node trace_node)
{
    trace_graph.add(trace_node, "unqualified_id");

    if (1 == identifier(trace_node)) {
        return 1;
    }

    //## todo

    // destructor
    // | '~' class_name
    c_context_tokens context_tokens(context);
    token_next(trace_node.get_tab());
    if ( token_is('~', trace_node) ) {
        context.class_member.is_destructor = 1;
        if (1 == class_name(trace_node)) {
            return 1;
        }
        context.class_member.is_destructor = 0;
    }
    context = context_tokens.restore();
//### todo test this:
    /*
        if (1 == template_id(trace_node)) {
            return 1;
        }
    */
    return 0;
}

/*
 * qualified_id: nested_name_specifier TEMPLATE_opt unqualified_id ;
 */
int c_parser_descent::qualified_id(c_trace_node trace_node)
{
    trace_graph.add(trace_node, "qualified_id");

    if (0 == nested_name_specifier(trace_node)) {
        return 0;
    }

    //## todo TEMPLATE_opt

    if (0 == unqualified_id(trace_node)) {
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

int c_parser_descent::nested_name_specifier(c_trace_node trace_node)
{
    trace_graph.add(trace_node, "nested_name_specifier");

    c_context_tokens context_tokens(context);

    int result = 0;
    string chain = "";

    chain = semantic.get_chain_head();

    while ( 1 ) {
        context_tokens.save(context);
        token_next(trace_node.get_tab());

        const int vector_id[]={ CLASS_NAME, NAMESPACE_NAME, TEMPLATE_NAME, -1};

        if (token_is_one(vector_id,trace_node) == 0) {
            context = context_tokens.restore();
            if ( 1 == result) {
                context.class_name_declaration = chain;
            }
            return result;
        }

        if ( 0 != chain.size() ) {
            chain = chain + "::";
        }
        chain = chain + c_token_get().text;

        token_next(trace_node.get_tab());
        if ( token_is_not(COLONCOLON, trace_node) ) {
            context = context_tokens.restore();
            return 0;
        }

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
int c_parser_descent::typedef_name(c_trace_node trace_node)
{
    trace_graph.add(trace_node, "typedef_name");

    c_context_tokens context_tokens(context);

    token_next(trace_node.get_tab());

    if ( token_is(TYPEDEF_NAME, trace_node) ) {
//#### deprecated tu avoid duplicated decls when decl is typedef
//        semantic.push_decl_specifier(c_token_get(), context);
        return 1;
    }

    context = context_tokens.restore();
    return 0;
}
/*----------------------------------------------------------------------------*/
/*
 * class_name: CLASS_NAME | template_id ;
 */
int c_parser_descent::class_name(c_trace_node trace_node)
{
    trace_graph.add(trace_node, "class_name");

    c_context_tokens context_tokens(context);

    token_next(trace_node.get_tab());

    if ( token_is(CLASS_NAME, trace_node) ) {
        // the context must be specified more up in rules tree
        // context.class_specifier_status =
        // CLASS_SPECIFIER_STATUS_IDENTIFIER;
        // -> this rule is used in serveral others rules.

        semantic.class_name(context, c_token_get());
        return 1;
    }
    /*## todo test this
        if ( 1 == template_id(trace_node) ) {
            return 1;
        }
    */
    context = context_tokens.restore();
    return 0;
}
/*----------------------------------------------------------------------------*/
/*
template_name:
	// identifier
	TEMPLATE_NAME
*/
int c_parser_descent::template_name(c_trace_node trace_node)
{
    trace_graph.add(trace_node, "template_name");
    c_context_tokens context_tokens(context);

    token_next(trace_node.get_tab());
    if ( token_is(TEMPLATE_NAME, trace_node) ) {
        token_print();
        semantic.class_name(context, c_token_get());

        c_decl_specifier decl(c_token_get());
        semantic.push_back_vector_decl_specifier(decl);
        return 1;
    }

    context = context_tokens.restore();
    return 0;
}
/*----------------------------------------------------------------------
 * Lexical elements.
 *----------------------------------------------------------------------*/
/*
 * identifier: IDENTIFIER ;
 */
int c_parser_descent::identifier(c_trace_node trace_node)
{
    trace_graph.add(trace_node, "identifier");
    c_context_tokens context_tokens(context);

    token_next(trace_node.get_tab());

    if ( token_is(IDENTIFIER, trace_node) ) {
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
int c_parser_descent::statement(c_trace_node trace_node)
{
    trace_graph.add(trace_node, "statement");
    return 1;
}

/*
compound_statement:
	'{' statement_seq_opt '}'
	;
*/
int c_parser_descent::compound_statement(c_trace_node trace_node)
{
    trace_graph.add(trace_node, "compound_statement");

    c_context_tokens context_tokens(context);

    token_next(trace_node.get_tab());
    if (  token_is_not('{', trace_node) ) {
        context = context_tokens.restore();
        return 0;
    }

    context.declarator.has_body = 1;

    if ( 0 == statement_seq_opt(trace_node) ) {
//      context = context_tokens.restore();
        return 0;
    }

    token_next(trace_node.get_tab());
    if ( token_is_not('}', trace_node) ) {
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
int c_parser_descent::statement_seq(c_trace_node trace_node)
{
    trace_graph.add(trace_node, "statement_seq");

    int n_open_braket = 0;

    c_context_tokens context_good_way(context);

    while ( token_get() != 0) {
        //## dummy
        statement(trace_node);

        context_good_way.save(context);
        token_next(trace_node.get_tab());
        if ( token_is('{', trace_node) ) {
            ++n_open_braket;
        }

        if ( token_is('}', trace_node) ) {
            if ( 0 == n_open_braket ) {
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
int c_parser_descent::declaration_seq(c_trace_node trace_node)
{
    trace_graph.add(trace_node, "declaration_seq");

    int result = 0;

    while (1 == declaration(trace_node)) {
        printf("## while declaration [ok] -------------------------------------------------------------------\n\n");
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
int c_parser_descent::declaration(c_trace_node trace_node)
{
    trace_graph.add(trace_node, "declaration");

    if ( 1 == is_eof(trace_node) ) {
        return 0;
    }

    if (1 == template_declaration(trace_node)) {
        return 1;
    }

    c_context_tokens context_tokens(context);
    if (1 == block_declaration(trace_node)) {
        return 1;
    }
    context = context_tokens.restore();

    if ( CLASS_SPECIFIER_STATUS_MEMBER_SPECIFIER != context.class_specifier_status ) {
        semantic.clear_decl_specifier();
    }

    string class_name_bk = context.class_name_declaration;
    if (1 == function_definition(trace_node)) {
        semantic.declarator_insert(trace_node.get_tab(), context);
        context.class_name_declaration = class_name_bk;
        return 1;
    }

    if (1 == preprocessor_include(trace_node)) {
        return 1;
    }

    if (1 == namespace_definition(trace_node)) {
        return 1;
    }

    context.class_name_declaration = class_name_bk;
    return 0;
}

/*----------------------------------------------------------------------------*/
/*
 * block_declaration: simple_declaration | asm_definition |
 * namespace_alias_definition | using_declaration | using_directive ;
 */
// ## todo rest of | ...
int c_parser_descent::block_declaration(c_trace_node trace_node)
{
    trace_graph.add(trace_node, "block_declaration");

    if (1 == simple_declaration(trace_node)) {
        return 1;
    }
    // tokens_vector_reload();

    if (1 == using_directive(trace_node)) {
        return 1;
    }

    return 0;
}

/*----------------------------------------------------------------------------*/
/*
 * simple_declaration: decl_specifier_seq_opt init_declarator_list_opt ';'
 * ;
 */
int c_parser_descent::simple_declaration(c_trace_node trace_node)
{
    trace_graph.add(trace_node, "simple_declaration");

    if ( CLASS_SPECIFIER_STATUS_MEMBER_SPECIFIER != context.class_specifier_status ) {
        semantic.clear_decl_specifier();
    }

    decl_specifier_seq_opt(trace_node);	// long int a = 0; this is 'long
    // int'
    init_declarator_list_opt(trace_node);	// long int a = 0; this is 'a = 0'

    /*//### i think decls no is optional is a must
        if( 0 == decl_specifier_seq_opt(trace_node))
        {
          return 0;
        }

        init_declarator_list_opt(trace_node);
    */

    c_context_tokens context_tokens(context);

    token_next(trace_node.get_tab());

    // functions with body does not have ; in the end
    printf("\n\n#### context.declarator.has_body[%d]\n\n",context.declarator.has_body);

    if ( 1 == context.declarator.has_body ) {
        context.declarator.has_body = 0;
//      tokens_vector_clear();
        return 1;
    } else {
        if ( token_is(';', trace_node) ) {
//          tokens_vector_clear();
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
int c_parser_descent::decl_specifier_seq(c_trace_node trace_node)
{
    trace_graph.add(trace_node, "decl_specifier_seq");
    // decl_specifier_seq_opt->decl_specifier_seq->decl_specifier_seq_opt...INFINITE
    // decl_specifier_seq_opt(trace_node);

    int result = 0;
    c_context_tokens context_tokens(context);

    while (1 == decl_specifier(trace_node)) {
        result = 1;
    }

    if (1 == result) {
        // tokens_vector_print_from_actual();
    } else {
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
int c_parser_descent::decl_specifier(c_trace_node trace_node)
{
    trace_graph.add(trace_node, "decl_specifier");

    const int vector_id[]={';' , ')', COLONCOLON,IDENTIFIER, '~', '#', -1};
    if (preanalisys_has_one(  vector_id,trace_node) ) {
        return 0;
    }

    if (1 == storage_class_specifier(trace_node)) {
        return 1;
    }

    if (1 == type_specifier(trace_node)) {
        return 1;
    }

    if (1 == function_specifier(trace_node)) {
        return 1;
    }

    if ( 1 == friend_specifier(trace_node)) {
        return 1;
    }

    if ( 1 == typedef_specifier(trace_node)) {
        return 1;
    }

    if ( 1 == ptr_specifier(trace_node)) {
        return 1;
    }

    return 0;
}

int c_parser_descent::friend_specifier(c_trace_node trace_node)
{
    trace_graph.add(trace_node, "friend_specifier");
    int result = 0;
    c_context_tokens context_tokens(context);
    token_next(trace_node.get_tab());

    if ( token_is(FRIEND, trace_node) ) {
        result = 1;
    }

// friend class c_generator_original;
    if (1 == result) {
        semantic.push_decl_specifier(c_token_get(), context);
    } else {
        context = context_tokens.restore();
        return 0;
    }

    c_context_tokens context_tokens_2(context);
    token_next(trace_node.get_tab());

    if ( token_is(CLASS, trace_node) ) {
        semantic.push_decl_specifier(c_token_get(), context);

        token_next(trace_node.get_tab());
        context.class_specifier_status = CLASS_SPECIFIER_STATUS_FRIEND_DECLARATOR;
        //## this need a improve to parse thinks like A::B...
        if ( token_is(CLASS_NAME, trace_node) ) {
            semantic.class_name_friend(context, c_token_get());
            return 1;
        }
        return 1;
    }

    context = context_tokens_2.restore();
    return 1;
}

int c_parser_descent::typedef_specifier(c_trace_node trace_node)
{
    trace_graph.add(trace_node, "typedef_specifier");
    int result = 0;
    c_context_tokens context_tokens(context);
    token_next(trace_node.get_tab());

    if ( token_is(TYPEDEF, trace_node) ) {
        result = 1;
    }

// friend class c_generator_original;
    if (1 == result) {
        //this should pass to c_symbol and in context = 0;
        semantic.push_decl_specifier(c_token_get(), context);

        context.is_typedef = 1;
        //## testing
        return 1;
    } else {
        context = context_tokens.restore();
        return 0;
    }

    c_context_tokens context_tokens_2(context);
/*    
    token_next(trace_node.get_tab());

    if ( token_is(CLASS, trace_node) ||
            token_is(STRUCT, trace_node)
       ) {
*/
    if( 1 == simple_type_specifier(trace_node) ){
        semantic.push_decl_specifier(c_token_get(), context);

        token_next(trace_node.get_tab());
//      context.class_specifier_status = CLASS_SPECIFIER_STATUS_FRIEND_DECLARATOR;
        //## this need a improve to parse thinks like A::B...
        if ( token_is(CLASS_NAME, trace_node) ) {
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
int c_parser_descent::storage_class_specifier(c_trace_node trace_node)
{
    trace_graph.add(trace_node, "storage_class_specifier");

    c_context_tokens context_tokens(context);
    token_next(trace_node.get_tab());

    const int vector_id[]={AUTO, REGISTER, STATIC, EXTERN, MUTABLE, -1};
    if (token_is_one(  vector_id,trace_node) != 0) {
        semantic.push_decl_specifier(c_token_get(), context);
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
int c_parser_descent::type_specifier(c_trace_node trace_node)
{
    trace_graph.add(trace_node, "type_specifier");

    c_context_tokens context_tokens(context);
    /*
      ## the nodes of tree increased -> bad idea

      if( preanalisys( CLASS, trace_node) )
        {
        if (1 == class_specifier(trace_node))
          {
            context_tokens.restore_but_not_i_token(context);
            return 1;
          }
        }
    */
    /* ###
      this rule in theory should be in others parts but for now
      is working t036.cpp

      one of this parts is this
        elaborated_type_specifier
    */
    /* ### last drop
        if (1 == template_id(trace_node)) {
            return 1;
        }
    */
    if (1 == simple_type_specifier(trace_node)) {
        return 1;
    }

    if (1 == class_specifier(trace_node)) {
//        context_tokens.restore_but_not_i_token(context);
        return 1;
    }

    if (1 == cv_qualifier(trace_node)) {
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
int c_parser_descent::simple_type_specifier(c_trace_node trace_node)
{
    trace_graph.add(trace_node, "simple_type_specifier");

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
    if (1 == type_name(trace_node)) {
        c_context_tokens context_tokens_0(context);
        //pre-analisys
        token_next(trace_node.get_tab());
        if ( token_is_not('(', trace_node) ) {
            result = 1;
        }

        if ( token_is(COLONCOLON, trace_node) ) {
            has_colon_colon_after = 1;
//          context = context_tokens_0.restore();
//          return 1;
        }

        context = context_tokens_0.restore();
    } else {
        if ( 1 == nested_name_specifier(trace_node) ) {
            has_colon_colon_after = 1;
        }
    }

    if (1 == template_id(trace_node)) {
        result = 1;
    }

    if ( 0 == result ) {
        token_next(trace_node.get_tab());
//##
//      context.class_name_declaration = class_name;
    }

    if ( 2 == context.i_am_in_template_declaration ) {
        if ( token_is(TEMPLATE_TYPE, trace_node) ) {
            result = 1;
        }
    }

    const int vector_id[]={
        CHAR, WCHAR_T, BOOL, SHORT, INT, LONG
        , SIGNED, UNSIGNED, FLOAT, DOUBLE, VOID, -1
    };

    if (token_is_one(vector_id,trace_node) != 0) {
        result = 1;
    }

    /*### todo fix workarround {
      template <class T1, int N> -> argument N can bee a number
      it must be improve to get other thins like defines
      ###{
    */
    if (0 == result) {
        if (1 == context.is_template_instantation) {
            if ( token_is(INTEGER, trace_node) ) {
                result = 1;
            }
        }
    }
    //###}

    if (1 == result) {
        c_decl_specifier decl(c_token_get());
        decl.type_specifier = 1;
        decl.has_colon_colon_after = has_colon_colon_after;

        if (1 == context.i_am_in_parameter_declaration) {
            context.param_vector_decl_specifier.push_back(decl);
        } else if (1 == context.is_template_instantation) {
            //### todo this if is weird
            if ( token_is_not('>', trace_node) ) {
                //### todo maybe mov this code to semantic ???
                c_template_argument argument;
                // yes we need the next parameter to fill vector_argument
                size_t i = context.vector_template_argument.size();

                if( i <= context.vector_template_parameter.size() ) {
                  argument.token = context.vector_template_parameter[i].token;
                  argument.vector_decl_specifier.push_back(decl);
                  context.vector_template_argument.push_back(argument);
                } else {
                  printf("#### [fix] [todo] error c_parser_descent::simple_type_specifier maybe this is because the context.vector_template_parameter is not reset\n");
                }
            }
        } else {
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
int c_parser_descent::function_specifier(c_trace_node trace_node)
{
    trace_graph.add(trace_node, "function_specifier");

    c_context_tokens context_tokens(context);
    token_next(trace_node.get_tab());

    const int vector_id[]={
        INLINE, VIRTUAL, EXPLICIT, -1
    };

    if (token_is_one(  vector_id,trace_node) != 0) {
        semantic.push_decl_specifier(c_token_get(), context);
        return 1;
    }

    context = context_tokens.restore();
    return 0;
}

/*
  rule mine
*/
int c_parser_descent::ptr_specifier(c_trace_node trace_node)
{
    trace_graph.add(trace_node, "ptr_specifier");

    c_context_tokens context_tokens(context);
    token_next(trace_node.get_tab());

    const int vector_id[]={
        '*', '&', -1
    };

    if (token_is_one(  vector_id,trace_node) != 0) {
        semantic.push_decl_specifier(c_token_get(), context);
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
int c_parser_descent::type_name(c_trace_node trace_node)
{
    trace_graph.add(trace_node, "type_name");

    if (1 == class_name(trace_node)) {
        return 1;
    }

    if (1 == typedef_name(trace_node)) {
        return 1;
    }

    //## todo rest

    return 0;
}
/*----------------------------------------------------------------------------*/
/*
namespace_definition:
	named_namespace_definition
	| unnamed_namespace_definition
	;
*/
int c_parser_descent::namespace_definition(c_trace_node trace_node)
{
    trace_graph.add(trace_node, "namespace_definition");

    if (1 == named_namespace_definition(trace_node)) {
        return 1;
    }

    //## todo rest
    return 0;
}
/*----------------------------------------------------------------------------*/
/*
named_namespace_definition:
	original_namespace_definition
	| extension_namespace_definition
	;
*/
int c_parser_descent::named_namespace_definition(c_trace_node trace_node)
{
    trace_graph.add(trace_node, "named_namespace_definition");

    if (1 == original_namespace_definition(trace_node)) {
        return 1;
    }

    //## todo rest
    return 0;
}
/*
original_namespace_definition:
	NAMESPACE identifier '{' namespace_body '}'
	;
*/
int c_parser_descent::original_namespace_definition(c_trace_node trace_node)
{
    trace_graph.add(trace_node, "original_namespace_definition");
    c_context_tokens context_tokens(context);

    token_next(trace_node.get_tab());
    if ( token_is_not(NAMESPACE, trace_node) ) {
        context = context_tokens.restore();
        return 0;
    }

    token_next(trace_node.get_tab());
    if ( token_is_not(IDENTIFIER, trace_node) ) {
        context = context_tokens.restore();
        return 0;
    }

    context.namespace_name_declaration = c_token_get().text;

    c_token token_namespace(c_token_get());

    token_next(trace_node.get_tab());
    if ( token_is_not('{', trace_node) ) {
        context = context_tokens.restore();
        return 0;
    }

    if (0 == namespace_body(trace_node)) {
        context = context_tokens.restore();
        return 0;
    }

    token_next(trace_node.get_tab());
    if ( token_is_not('}', trace_node) ) {
        context = context_tokens.restore();
        return 0;
    }

    semantic.namespace_declarator(context, token_namespace);
    context.namespace_name_declaration = "";

    return 1;
}
/*
namespace_body:
	declaration_seq_opt
	;
*/
int c_parser_descent::namespace_body(c_trace_node trace_node)
{
    trace_graph.add(trace_node, "namespace_body");

    if (1 == declaration_seq_opt(trace_node)) {
        return 1;
    }

    return 0;
}
/*
using_directive:
	USING NAMESPACE COLONCOLON_opt nested_name_specifier_opt namespace_name ';'
	;
*/
int c_parser_descent::using_directive(c_trace_node trace_node)
{
    trace_graph.add(trace_node, "using_directive");
    c_context_tokens context_tokens(context);

    token_next(trace_node.get_tab());
    if ( token_is_not(USING, trace_node) ) {
      context = context_tokens.restore();
      return 0;
    }

    token_next(trace_node.get_tab());
    if ( token_is_not(NAMESPACE, trace_node) ) {
      context = context_tokens.restore();
      return 0;
    }

    token_next(trace_node.get_tab());
    if ( token_is_not(NAMESPACE_NAME, trace_node) ) {
      context = context_tokens.restore();
      return 0;
    }

    semantic.push_using_namespace(c_token_get().text);

    token_next(trace_node.get_tab());
    if ( token_is(';', trace_node) ) {
        return 1;
    }

    context = context_tokens.restore();
    return 1;
}
/*----------------------------------------------------------------------------*/
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
int c_parser_descent::class_specifier(c_trace_node trace_node)
{
    trace_graph.add(trace_node, "class_specifier");

    int was_typedef = 0;
    string class_name_previous = context.class_name_declaration;

    was_typedef = context.is_typedef;
    context.is_typedef = 0;

    //maybe will need read class other time
    c_context_tokens context_tokens_1(context);

    if (0 == class_head(trace_node)) {
        context.class_name_declaration = class_name_previous;
        return 0;
    }

    //we can get this point wihout identifer of class
    // typedef class A t_A; in this clase
    // A is not a IDENTIFIER is CLASS_NAME -> enter in next if
    if ( 0 == context.class_name_declaration.size() ) {
        if ( 1 == was_typedef ) {
            context = context_tokens_1.restore();
            if (0 == class_key(trace_node)) {
                context.class_name_declaration = class_name_previous;
                return 0;
            }

            context.class_specifier_status = CLASS_SPECIFIER_STATUS_IDENTIFIER;

            c_context_tokens context_tokens_2(context);
            token_next(trace_node.get_tab());
            // typedef class {};
            if ( token_is('{', trace_node) ) {
                ++class_no_name_number;
                c_token no_identifier(IDENTIFIER, get_class_no_name().c_str());
                semantic.identifier(context, no_identifier);
            } else {
                //typedef class A t_A;
                if ( token_is(CLASS_NAME, trace_node) ) {
                    context.class_name_declaration = c_token_get().text;
                }

                context.is_typedef = was_typedef;
                if ( 1 == identifier(trace_node)) {
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
            context.class_name_declaration = get_class_no_name().c_str();
        } else {
            printf("error c_parser_descent::class_specifier() class without name\n");
            context.class_name_declaration = class_name_previous;
            return 0;
        }
    }
    c_context_tokens context_tokens(context);
    // context.class_specifier = 1;

    token_next(trace_node.get_tab());
    if ( token_is_not('{', trace_node) ) {
        context = context_tokens.restore();
        context.class_name_declaration = class_name_previous;
        return 0;
    }

    printf("%s## class_specifier {\n", trace_node.get_tab().c_str());

    // we need to know what class is processing
    /*
      ### todo
        saving this part of context should be in another object
        and be a composition of context -> c_template_context
    */
    string class_name = context.class_name_declaration;
    int i_am_in_template_declaration = context.i_am_in_template_declaration;
    t_vector_template_parameter vector_template_parameter = context.vector_template_parameter;
    t_map_template_parameter map_template_parameter = context.map_template_parameter;

    int is_template_instantation = context.is_template_instantation;
    t_vector_template_argument vector_template_argument = context.vector_template_argument;
    t_map_template_argument map_template_argument = context.map_template_argument;

    tokens_vector_clear();
    context.class_name_declaration = class_name;
    context.i_am_in_template_declaration = i_am_in_template_declaration;
    context.vector_template_parameter = vector_template_parameter;
    context.map_template_parameter = map_template_parameter;

    context.is_template_instantation = is_template_instantation;
    context.vector_template_argument = vector_template_argument;
    context.map_template_argument = map_template_argument;

    context_tokens.save(context);

    member_specification_opt(trace_node);

    token_next(trace_node.get_tab());
    if ( token_is('}', trace_node) ) {

        printf("%s## class_specifier }\n", trace_node.get_tab().c_str());

        context.is_typedef = was_typedef;
        if ( 1 == context.is_typedef) {
            if ( 1 == identifier(trace_node)) {
                char * pch = NULL;
                pch = strstr((char *)context.class_name_declaration.c_str(),NO_CLASS_NAME);
                if (NULL != pch) {
                    ts.change_key(context.class_name_declaration, c_token_get().text);
                }
            } else {
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
int c_parser_descent::class_head(c_trace_node trace_node)
{
    trace_graph.add(trace_node, "class_head");

    if (0 == class_key(trace_node)) {
        return 0;
    }

    context.class_specifier_status = CLASS_SPECIFIER_STATUS_IDENTIFIER;
    // ## what happend if have not name ?
    identifier_opt(trace_node);

    context.class_specifier_status =
        CLASS_SPECIFIER_STATUS_BASE_DECLARATION;

    base_clause_opt(trace_node);

    // printf( " #### class_head-> [%s]\n",token_identifier.text.c_str()
    // );
    return 1;
}

/*----------------------------------------------------------------------------*/
/*
 * class_key: CLASS | STRUCT | UNION ;
 */
int c_parser_descent::class_key(c_trace_node trace_node)
{
    trace_graph.add(trace_node, "class_key");

    // #### here is the error
    // token_next() pass the class token
    c_context_tokens context_tokens(context);
    token_next(trace_node.get_tab());

    if ( token_is(CLASS, trace_node) ) {
        context.class_key = CLASS;
        context.access_specifier = PRIVATE;
        return 1;
    }

    if ( token_is(STRUCT, trace_node) ) {
        context.class_key = STRUCT;
        context.access_specifier = PUBLIC;
        return 1;
    }

    if ( token_is(UNION, trace_node) ) {
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
int c_parser_descent::member_specification(c_trace_node trace_node)
{
    trace_graph.add(trace_node, "member_specification");

    if ( preanalisys('}', trace_node) ) {
        return 0;
    }

    // | access_specifier ':'
    if (1 == access_specifier(trace_node)) {
        c_context_tokens context_tokens(context);
        token_next(trace_node.get_tab());

        if ( token_is(':', trace_node) ) {
            return 1;
        }
        context = context_tokens.restore();
    }

    if (1 == member_declaration(trace_node)) {
        return 1;
    }
    //## working to define classes inside classes
    if ( 1 == simple_declaration(trace_node) ) {
        return 1;
    }

    return 0;
}

/*----------------------------------------------------------------------------*/
/*
 * member_declaration: decl_specifier_seq_opt member_declarator_list_opt
 * ';' | function_definition SEMICOLON_opt | qualified_id ';' |
 * using_declaration | template_declaration ;
 */
int c_parser_descent::member_declaration(c_trace_node trace_node)
{
    trace_graph.add(trace_node, "member_declaration");
    c_context_tokens context_tokens(context);

    context.class_specifier_status =
        CLASS_SPECIFIER_STATUS_MEMBER_SPECIFIER;
    semantic.clear_decl_specifier();
    decl_specifier_seq_opt(trace_node);

    //friend class A;
    //have not member_declarator_list_opt
    if ( CLASS_SPECIFIER_STATUS_FRIEND_DECLARATOR == context.class_specifier_status ) {
        token_next(trace_node.get_tab());

        if ( token_is(';', trace_node) ) {
//          tokens_vector_clear();
            return 1;
        }
//      tokens_vector_clear();
        return 1;
    }

    context.class_specifier_status =
        CLASS_SPECIFIER_STATUS_MEMBER_DECLARATOR;

//  c_context_tokens context_tokens_2(context);

    if (1 == member_declarator_list_opt(trace_node)) {

        token_next(trace_node.get_tab());

        if ( token_is(';', trace_node) ) {
            int access_bk = context.access_specifier;
            tokens_vector_clear();
            context.access_specifier = access_bk;

            return 1;
        }
    }

    context = context_tokens.restore();

    context.class_specifier_status =
        CLASS_SPECIFIER_STATUS_MEMBER_SPECIFIER;

    context.i_am_in_member = 1;
    semantic.clear_decl_specifier();

    if ( 1 == function_definition(trace_node) ) {
        //SEMICOLON_opt(trace_node);
        semantic.declarator_insert(trace_node.get_tab(), context);
        context.i_am_in_member = 0;
//      tokens_vector_clear();
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
int c_parser_descent::member_declarator_list(c_trace_node trace_node)
{
    trace_graph.add(trace_node, "member_declarator_list");

    c_context_tokens context_tokens(context);
    c_context_tokens context_good_way(context);

    if (0 == member_declarator(trace_node)) {
        return 0;
    }

    for (;;) {
        context_good_way.save(context);
        token_next(trace_node.get_tab());

        if ( token_is(';', trace_node) ) {
            // yes i restore here to consume ';' more up in the tree
            context = context_good_way.restore();
            return 1;
        }

        if ( token_is_not(',', trace_node) ) {
            context = context_tokens.restore();
            return 1;
        }

        if (0 == member_declarator(trace_node)) {
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
int c_parser_descent::member_declarator(c_trace_node trace_node)
{
    trace_graph.add(trace_node, "member_declarator");

    // declarator pure_specifier_opt
    context.i_am_in_member = 1;
    if (1 == declarator(trace_node)) {
        // if( 1 == pure_specifier_opt(trace_node) ) //## todo
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
int c_parser_descent::base_clause(c_trace_node trace_node)
{
    trace_graph.add(trace_node, "base_clause");

    c_context_tokens context_tokens(context);
    token_next(trace_node.get_tab());

    if ( token_is_not(':', trace_node) ) {
        context = context_tokens.restore();
        return 0;
    }

    if (0 == base_specifier_list(trace_node)) {
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
int c_parser_descent::base_specifier_list(c_trace_node trace_node)
{
    trace_graph.add(trace_node, "base_specifier_list");

    if (0 == base_specifier(trace_node)) {
        return 0;
    }

    c_context_tokens context_tokens(context);

    for (;;) {
        context_tokens.save(context);
        token_next(trace_node.get_tab());
        if ( token_is_not(',', trace_node) ) {
            context = context_tokens.restore();
            return 1;
        }

        if (0 == base_specifier(trace_node)) {
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
int c_parser_descent::base_specifier(c_trace_node trace_node)
{
    trace_graph.add(trace_node, "base_specifier");


    // ##todo rest of rules


    // | access_specifier VIRTUAL_opt COLONCOLON_opt
    // nested_name_specifier_opt class_name

    if (0 == access_specifier(trace_node)) {
        // ## this dont must break the down
        // return 0;
    }
    // ## todo midle

    context.class_specifier_status =
        CLASS_SPECIFIER_STATUS_BASE_DECLARATION;

    if (1 == class_name(trace_node)) {
        return 1;
    }


    return 0;
}

/*----------------------------------------------------------------------------*/
/*
 * access_specifier: PRIVATE | PROTECTED | PUBLIC ;
 */
int c_parser_descent::access_specifier(c_trace_node trace_node)
{
    trace_graph.add(trace_node, "access_specifier");

    c_context_tokens context_tokens(context);
    token_next(trace_node.get_tab());

    if ( token_is(PRIVATE, trace_node) ) {
        context.access_specifier = PRIVATE;
        return 1;
    }

    if ( token_is(PROTECTED, trace_node) ) {
        context.access_specifier = PROTECTED;
        return 1;
    }

    if ( token_is(PUBLIC, trace_node) ) {
        context.access_specifier = PUBLIC;
        return 1;
    }

    context = context_tokens.restore();

    return 0;
}

/*----------------------------------------------------------------------
 * Templates.
 *----------------------------------------------------------------------*/
/*
 template_declaration:
	EXPORT_opt TEMPLATE '<' template_parameter_list '>' declaration
	;
*/
int c_parser_descent::template_declaration(c_trace_node trace_node)
{
    trace_graph.add(trace_node, "template_declaration");

    c_context_tokens context_tokens(context);

    token_next(trace_node.get_tab());
    tokens_vector_print_from_actual();
    if ( token_is_not(TEMPLATE, trace_node) ) {
        context = context_tokens.restore();
        return 0;
    }

    context.i_am_in_template_declaration = 1;

    token_next(trace_node.get_tab());
    if ( token_is_not('<', trace_node) ) {
        context = context_tokens.restore();
        return 0;
    }

    if ( 0 == template_parameter_list(trace_node) ) {
        context = context_tokens.restore();
        return 0;
    }

    token_next(trace_node.get_tab());
    if ( token_is_not('>', trace_node) ) {
        context = context_tokens.restore();
        context.i_am_in_template_declaration = 0;
        return 0;
    }

    context.i_am_in_template_declaration = 2;

    if ( 1 == declaration(trace_node) ) {
        context.i_am_in_template_declaration = 0;
        return 1;
    }

    context = context_tokens.restore();
    return 0;
}
/*----------------------------------------------------------------------------*/
/*
template_parameter_list:
	template_parameter
	| template_parameter_list ',' template_parameter
	;
*/
int c_parser_descent::template_parameter_list(c_trace_node trace_node)
{
    trace_graph.add(trace_node, "template_parameter_list");

    c_context_tokens context_tokens(context);
    c_context_tokens context_good_way(context);

    if ( 0 == template_parameter(trace_node) ) {
        return 0;
    }

    for (;;) {
        context_good_way.save(context);
        token_next(trace_node.get_tab());

        if ( token_is('>', trace_node) ) {
            // yes i restore here to consume '>' more up in the tree
            context = context_good_way.restore();
            return 1;
        }

        if ( token_is_not(',', trace_node) ) {
            context = context_tokens.restore();
            return 0;
        }

        if (0 == template_parameter(trace_node)) {
            context = context_tokens.restore();
            return 0;
        }
    }

    context = context_tokens.restore();
    return 0;
}
/*----------------------------------------------------------------------------*/
/*
template_parameter:
	type_parameter
	| parameter_declaration
	;
*/
int c_parser_descent::template_parameter(c_trace_node trace_node)
{
    trace_graph.add(trace_node, "template_parameter");
    c_context_tokens context_tokens(context);


    if ( 1 == type_parameter(trace_node) ) {
        return 1;
    }

    //## todo rest

    context = context_tokens.restore();
    return 0;
}
/*----------------------------------------------------------------------------*/
/*
type_parameter:
	CLASS identifier_opt
	| CLASS identifier_opt '=' type_id
	| TYPENAME identifier_opt
	| TYPENAME identifier_opt '=' type_id
	| TEMPLATE '<' template_parameter_list '>' CLASS identifier_opt
	| TEMPLATE '<' template_parameter_list '>' CLASS identifier_opt '=' template_name
	;
*/
int c_parser_descent::type_parameter(c_trace_node trace_node)
{
    trace_graph.add(trace_node, "type_parameter");
    c_context_tokens context_tokens(context);

    //## todo rest
    token_next(trace_node.get_tab());
    const int vector_id[]={CLASS, TYPENAME, INT, -1};
    if (token_is_one(  vector_id,trace_node) != 0) {
        c_decl_specifier decl(c_token_get());
        context.template_parameter.vector_decl_specifier.push_back(decl);

        context.declaring_template_type = 1;

        identifier_opt(trace_node);
        context.declaring_template_type = 0;
        return 1;
    }

    context = context_tokens.restore();
    return 0;
}
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*
template_id:
	template_name '<' template_argument_list '>'
	;
*/
int c_parser_descent::template_id(c_trace_node trace_node)
{
    trace_graph.add(trace_node, "template_id");

    if ( 0 == template_name(trace_node) ) {
        return 0;
    }

    c_context_tokens context_tokens(context);

    context.is_template_instantation = 1;
    c_symbol *p_symbol = ts.search_symbol(yytext);
    if (p_symbol) {
        if (p_symbol->type != 0) {
            // return symbol.type;
            printf("## next_token found symbol [%s]",
                   yytext);
            if ( 1 == p_symbol->is_template ) {
                context.is_template_instantation = 1;
                context.map_template_parameter = p_symbol->map_template_parameter;
                context.vector_template_parameter = p_symbol->vector_template_parameter;
            }
        }
    }

    token_next(trace_node.get_tab());
    if ( token_is_not('<', trace_node) ) {
        context = context_tokens.restore();
        return 0;
    }

    if ( 0 == template_argument_list(trace_node) ) {
        context = context_tokens.restore();
        return 0;
    }

    token_next(trace_node.get_tab());
    if ( token_is_not('>', trace_node) ) {
        context = context_tokens.restore();
        return 0;
    }
    /*#### maybe mv this to semantic
                          context.is_template_instantation = 0;
                          context.map_template_parameter.clear();
                          context.vector_template_parameter.clear();
    */
    return 1;
}
/*----------------------------------------------------------------------------*/
/*
template_argument_list:
	template_argument
	| template_argument_list ',' template_argument
	;
*/
int c_parser_descent::template_argument_list(c_trace_node trace_node)
{
    trace_graph.add(trace_node, "template_argument_list");
    c_context_tokens context_tokens(context);
    c_context_tokens context_good_way(context);

    if ( 0 == template_argument(trace_node) ) {
        return 0;
    }

    for (;;) {
        context_good_way.save(context);
        token_next(trace_node.get_tab());

        if ( token_is('>', trace_node) ) {
            // yes i restore here to consume '>' more up in the tree
            context = context_good_way.restore();
            return 1;
        }

        if ( token_is_not(',', trace_node) ) {
            context = context_tokens.restore();
            return 0;
        }

        if (0 == template_argument(trace_node)) {
            context = context_tokens.restore();
            return 0;
        }
    }

    context = context_tokens.restore();
    return 0;
}
/*----------------------------------------------------------------------------*/
/*
template_argument:
	assignment_expression
	| type_id
	| template_name
	;
*/
int c_parser_descent::template_argument(c_trace_node trace_node)
{
    trace_graph.add(trace_node, "template_argument");

    context.is_template_instantation = 1;

    //### todo search types in context.map_template_parameter
    //### todo type_id
    //### drop type_specifier
    /*
        if (1 == type_id(trace_node)) {
            return 1;
        }
    */
    if (1 == type_specifier(trace_node)) {
        return 1;
    }

    context.is_template_instantation = 0;

    return 0;
}
/*----------------------------------------------------------------------
 * Epsilon (optional) definitions.
 *----------------------------------------------------------------------*/
/*
 * declaration_seq_opt: //epsilon | declaration_seq ;
 */
// todo epsilon
int c_parser_descent::declaration_seq_opt(c_trace_node trace_node)
{
    trace_graph.add(trace_node, "declaration_seq_opt");

    if (1 == declaration_seq(trace_node)) {
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
int c_parser_descent::COLONCOLON_opt(c_trace_node trace_node)
{
    trace_graph.add(trace_node, "COLONCOLON_opt");

    c_context_tokens context_tokens(context);

    token_next(trace_node.get_tab());
    if ( token_is(COLONCOLON, trace_node) ) {
        return 1;
    }

    context = context_tokens.restore();
    return 0;
}

/*----------------------------------------------------------------------------*/
/*
 * decl_specifier_seq_opt: //epsilon | decl_specifier_seq ;
 */
int c_parser_descent::decl_specifier_seq_opt(c_trace_node trace_node)
{
    trace_graph.add(trace_node, "decl_specifier_seq_opt");

    return decl_specifier_seq(trace_node);
}
/*----------------------------------------------------------------------------*/
/*
statement_seq_opt:
	// epsilon
	| statement_seq
	;
*/
int c_parser_descent::statement_seq_opt(c_trace_node trace_node)
{
    trace_graph.add(trace_node, "statement_seq_opt");

    return statement_seq(trace_node);
}
/*----------------------------------------------------------------------------*/
/*
 * init_declarator_list_opt: //epsilon | init_declarator_list ;
 */
int c_parser_descent::init_declarator_list_opt(c_trace_node trace_node)
{
    trace_graph.add(trace_node, "init_declarator_list_opt");

    return init_declarator_list(trace_node);
}

/*----------------------------------------------------------------------------*/
/*
 * identifier_opt: // epsilon | identifier ;
 */
int c_parser_descent::identifier_opt(c_trace_node trace_node)
{
    trace_graph.add(trace_node, "identifier_opt");

    return identifier(trace_node);
}

/*
 * parameter_declaration_list_opt: // epsilon | parameter_declaration_list
 * ;
 */
int
c_parser_descent::parameter_declaration_list_opt(c_trace_node trace_node)
{
    trace_graph.add(trace_node, "parameter_declaration_list_opt");
    return parameter_declaration_list(trace_node);
}

/*
 * ELLIPSIS_opt: // epsilon | ELLIPSIS ;
 */
int c_parser_descent::ELLIPSIS_opt(c_trace_node trace_node)
{
    trace_graph.add(trace_node, "ELLIPSIS_opt");

    c_context_tokens context_tokens(context);

    token_next(trace_node.get_tab());
    if ( token_is(ELLIPSIS, trace_node) ) {
        c_decl_specifier decl(c_token_get());
        decl.type_specifier = 1;

        if (1 == context.i_am_in_parameter_declaration) {
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
int c_parser_descent::member_specification_opt(c_trace_node trace_node)
{
    trace_graph.add(trace_node, "member_specification_opt");

    int result = 0;

    while (1 == member_specification(trace_node)) {
        result = 1;
    }

    return result;
}

/*----------------------------------------------------------------------------*/
/*
 * base_clause_opt: // epsilon | base_clause ;
 */
int c_parser_descent::base_clause_opt(c_trace_node trace_node)
{
    trace_graph.add(trace_node, "base_clause_opt");

    return base_clause(trace_node);
}

/*----------------------------------------------------------------------------*/
/*
 * member_declarator_list_opt: epsilon | member_declarator_list ;
 */
int c_parser_descent::member_declarator_list_opt(c_trace_node trace_node)
{
    trace_graph.add(trace_node, "member_declarator_list_opt");

    return member_declarator_list(trace_node);
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
int c_parser_descent::init_declarator_list(c_trace_node trace_node)
{
    trace_graph.add(trace_node, "init_declarator_list");
    c_context_tokens context_tokens(context);
    c_context_tokens context_good_way(context);

    if (0 == init_declarator(trace_node)) {
        return 0;
    }

    for (;;) {
        context_good_way.save(context);
        token_next(trace_node.get_tab());

        if ( token_is(';', trace_node) ) {
            // yes i restore here to consume ';' more up in the tree
            context = context_good_way.restore();
            return 1;
        }

        if ( token_is_not(',', trace_node) ) {
            context = context_tokens.restore();
            return 1;
        }

        if (0 == init_declarator(trace_node)) {
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
int c_parser_descent::init_declarator(c_trace_node trace_node)
{
    trace_graph.add(trace_node, "init_declarator");

    if (1 == declarator(trace_node)) {
        // if( 1 == initializer_opt(trace_node) ) //## todo
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
int c_parser_descent::declarator(c_trace_node trace_node)
{
    trace_graph.add(trace_node, "declarator");

    const int vector_id[]={')', ';', -1};
    if (preanalisys_has_one(  vector_id,trace_node) ) {
        return 0;
    }

    if (1 == direct_declarator(trace_node)) {
        return 1;
    }

    // | ptr_operator declarator
    /*##
      i did this in the part of decl_specifier
    if( 1 == ptr_operator(trace_node))
      {
        return declarator(trace_node);
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
 * changing a litle the original order to set clearly the terminals of
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
int c_parser_descent::direct_declarator(c_trace_node trace_node)
{
    trace_graph.add(trace_node, "direct_declarator");
    tokens_vector_print_from_actual();

//  c_context_tokens qualified_id_context(context);
//  qualified_id_context = context;
    c_context_tokens context_tokens(context);
    c_context_tokens context_good_way(context);

    int result = 0;
    for (;;) {
        result = 0;
        if (1 == declarator_id(trace_node)) {
            result = 1;

            context_good_way.save(context);
            token_next(trace_node.get_tab());
            if ( token_is('(', trace_node) ) {
                if (1 == parameter_declaration_clause(trace_node)) {
                    printf("### 1 == parameter_declaration_clause(trace_node)\n");
                } else  if (1 == declarator(trace_node)) {
                    printf("### 1 == declarator(trace_node)\n");
                } else {
                    printf("### no, we are not in a member function !\n");
                    context = context_tokens.restore();
                    return 0;
                }

                token_next(trace_node.get_tab());
                if ( token_is_not(')', trace_node) ) {
                    context = context_tokens.restore();
                    return 0;
                }

                context_good_way.save(context);
                token_next(trace_node.get_tab());

                if ( token_is(';', trace_node) ) {
                    printf("### yes we are in a function !\n");
                    semantic.declarator_insert(trace_node.get_tab(), context);
                    // yes i restore here to consume ';' more up in the
                    // tree
                    context = context_good_way.restore();
                    return 1;
                }
                // this is posible:
                // int f1(void), f2(void);
                if ( token_is(',', trace_node) ) {
                    printf("### yes we are in a function !\n");
                    semantic.declarator_insert(trace_node.get_tab(), context);
                    // yes i restore here to consume ',' more up in the
                    // tree
                    context = context_good_way.restore();
                    return 1;
                }
            } else {
                // ## todo | direct_declarator '[' constant_expression_opt
                // ']'
                if (1 != context.class_member.is_function) {
                    if ( 1 != context.declarator.is_function) {
                        semantic.declarator_insert(trace_node.get_tab(), context);
                    }
                }
//                return 1;
            }
        }

        if (1 == result) {
            context = context_good_way.restore();
            return 1;
        } else {
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
int c_parser_descent::ptr_operator(c_trace_node trace_node)
{
    trace_graph.add(trace_node, "ptr_operator");

    c_context_tokens context_tokens(context);
    token_next(trace_node.get_tab());

    const int vector_id[]={ '*', '&', -1 };

    if (token_is_one(  vector_id,trace_node) != 0) {
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
int c_parser_descent::cv_qualifier_seq(c_trace_node trace_node)
{
    trace_graph.add(trace_node, "cv_qualifier_seq");
    return 0;
}
/*----------------------------------------------------------------------------*/
/*
cv_qualifier:
	CONST
	| VOLATILE
	;
*/
int c_parser_descent::cv_qualifier(c_trace_node trace_node)
{
    trace_graph.add(trace_node, "cv_qualifier");
    int result = 0;
    c_context_tokens context_tokens(context);
    token_next(trace_node.get_tab());

    if ( token_is(CONST, trace_node) ) {
        result = 1;
    }

    if ( token_is(VOLATILE, trace_node) ) {
        result = 1;
    }

    if (1 == result) {
        semantic.push_decl_specifier(c_token_get(), context);
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
int c_parser_descent::declarator_id(c_trace_node trace_node)
{
    trace_graph.add(trace_node, "declarator_id");

    //## whe must have a class name before of this
    COLONCOLON_opt(trace_node);

    if (1 == id_expression(trace_node)) {
        return 1;
    }

    //|
    //## nested_name_specifier_opt

    //contructors enter for here
    if (1 == type_name(trace_node)) {
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
int c_parser_descent::parameter_declaration_clause(c_trace_node trace_node)
{
    trace_graph.add(trace_node, "parameter_declaration_clause");
    if ( preanalisys(')', trace_node) ) {
        return 1;
    }

    context.i_am_in_parameter_declaration = 1;
    parameter_declaration_list_opt(trace_node);

    ELLIPSIS_opt(trace_node);

    context.i_am_in_parameter_declaration = 0;

    c_context_tokens context_good_way(context);
    context_good_way.save(context);
    token_next(trace_node.get_tab());

    if ( token_is(')', trace_node) ) {
        context = context_good_way.restore();
        return 1;
    }

    return 0;
}

/*
 * parameter_declaration_list: parameter_declaration |
 * parameter_declaration_list ',' parameter_declaration ;
 */
int c_parser_descent::parameter_declaration_list(c_trace_node trace_node)
{
    trace_graph.add(trace_node, "parameter_declaration_list");

    c_context_tokens context_good_way(context);
    context.param_vector_decl_specifier.clear();

    if (0 == parameter_declaration(trace_node)) {
        c_context_tokens context_tokens(context);
        context_good_way.save(context);
        token_next(trace_node.get_tab());

        if ( token_is(')', trace_node) ) {
            context = context_good_way.restore();
            return 1;
        }

        context = context_tokens.restore();
        return 0;
    }

    c_context_tokens context_tokens(context);

    for (;;) {
        context_good_way.save(context);
        token_next(trace_node.get_tab());

        if ( token_is(')', trace_node) ) {
            context = context_good_way.restore();
            return 1;
        }

        if ( token_is_not(',', trace_node) ) {
            context = context_tokens.restore();
            return 1;
        }

        context.param_vector_decl_specifier.clear();

        if (0 == parameter_declaration(trace_node)) {
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
int c_parser_descent::parameter_declaration(c_trace_node trace_node)
{
    trace_graph.add(trace_node, "parameter_declaration");
    if (0 == decl_specifier_seq(trace_node)) {
        return 0;
    }

    if (1 == declarator(trace_node)) {
        // ## todo the rest...
        return 1;
    }

    if ( token_is(VOID, trace_node) ) {
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
 *
 * decl_specifier_seq_opt has been proccessed in:
 *    declaration->block_declaration->
 *      ->simple_declaration->decl_specifier_seq_opt
 */
int c_parser_descent::function_definition(c_trace_node trace_node)
{
    trace_graph.add(trace_node, "function_definition");

    decl_specifier_seq_opt(trace_node);

    if ( CLASS_SPECIFIER_STATUS_MEMBER_SPECIFIER ==
            context.class_specifier_status ) {
        context.class_specifier_status =
            CLASS_SPECIFIER_STATUS_MEMBER_DECLARATOR;
    }

    if (0 == declarator(trace_node)) {
        return 0;
    }

    //## ctor_initializer_opt

    if (0 == function_body(trace_node)) {
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
int c_parser_descent::function_body(c_trace_node trace_node)
{
    trace_graph.add(trace_node, "function_body");

    return compound_statement(trace_node);
}
/*----------------------------------------------------------------------------*/
