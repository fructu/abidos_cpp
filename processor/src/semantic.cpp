/*------------------------------------------------------------------------------

    Proyecto            : abidos
    Codigo              : semantic.cpp
    Descripcion         :
    Version             : 0.1
    Autor               : F. Manuel Hevia Garcia
    Fecha creacion      : 21/03/2011
    Fecha modificacion  :

    Observaciones :


------------------------------------------------------------------------------*/
#include "semantic.h"
#include "options.h"

#include <stdio.h>
#include <stdlib.h>

/*----------------------------------------------------------------------------*/
void
c_semantic::class_specifier_identifier(c_context & context, c_token token)
{
    c_symbol symbol(token);
    if (1 == options.verbose_flag) {
        printf("## c_semantic::class_specifier_identifier(c_context context) context.class_key[%d]:[%s]\n",context.class_key, yytokens[context.class_key]);
        printf("## c_semantic::class_specifier_identifier(c_context context) symbol.type[%d]:[%s] symbol.class_key[%d]:[%s] \n"
               ,symbol.type, yytokens[symbol.type],symbol.class_key,yytokens[symbol.class_key]);
    }


    if (CLASS_SPECIFIER_STATUS_IDENTIFIER !=
            context.class_specifier_status) {
        printf
        ("error c_semantic::class_ CLASS_IDENTIFIER != context.class_specifier_status\n");
        return;
    }

    if (CLASS == context.class_key) {
        symbol.type = CLASS_NAME;
        symbol.class_key = context.class_key;
    }

    if (STRUCT == context.class_key) {
        symbol.type = CLASS_NAME;
        symbol.class_key = context.class_key;
    }
    // ## todo
    if (UNION == context.class_key) {
//        symbol.type = UNION_NAME;
        symbol.type = UNION;
//        symbol.class_key = UNION;
        symbol.class_key = CLASS;
    }
    // we only take the name of the class the fist time after CLASS
    // if( 1 == context.class_head)

    if ( TEMPLATE_DECLARATION == context.template_status ) {
        symbol.is_template = 1;
        symbol.vector_template_parameter = context.vector_template_parameter;
        symbol.map_template_parameter = context.map_template_parameter;

        // in C++ template classes must have different identifiers
        //symbol.process_token_text();
    }

    if ( 1 == context.class_pre_declaration ) {
        symbol.class_pre_declaration = 1;
    }

    if ( 0 != context.class_name_declaration.size() ) {
        string s = symbol.token.text;
        symbol.token.text = context.class_name_declaration + "::" + s;
        symbol.text = symbol.token.text;
    } else {
        if (0 != context.namespace_name_declaration.size() ) {
            string s = symbol.token.text;
            symbol.token.text = context.namespace_name_declaration + "::" + symbol.text;
            symbol.text       = symbol.token.text;
        }
    }

    symbol.is_abstract = context.is_abstract;

    context.class_name_declaration = symbol.token.text;

    ts.insert(symbol);
}

/*----------------------------------------------------------------------------*/
void
c_semantic::class_member_declarator(c_context & context, c_token token)
/*
 * a member is for example a1
 *
 * class A { int a1; };
 */
{
    if (1 == options.verbose_flag) {
        printf("## c_semantic::class_member_declarator()\n");

        printf("##:           context.class_name_declaration[%s]\n",context.class_name_declaration.c_str());
        printf("##:           token.text[%s]\n",token.text.c_str());
        printf("##:           context.template_status[%s]\n",
               table_template_declaration_status[context.template_status]);

        printf("### context.class_specifier_status[%d] -> [%s] \n",context.class_specifier_status, table_parser_status[context.class_specifier_status]);
    }

    if (CLASS_SPECIFIER_STATUS_MEMBER_DECLARATOR !=
            context.class_specifier_status) {
        printf
        ("error c_semantic::class_member_declarator() CLASS_SPECIFIER_STATUS_MEMBER_DECLARATOR !=	context.class_specifier_status\n");
        exit(-1);
    }

    c_symbol *p_symbol =
        ts.search_symbol(context.class_name_declaration);
    if (0 == p_symbol ) {
        printf("## c_semantic::class_member_declarator() context.class_name_declaration[%s] not found!\n",context.class_name_declaration.c_str());
        return;
    }
    if (1 == options.verbose_flag) {
        printf("## c_semantic::class_member_declarator() context.class_name_declaration[%s] founded\n",context.class_name_declaration.c_str());
    }

    if (0 == p_symbol->class_key) {
        printf
        ("error c_semantic::class_member_declarator()  0 == p_symbol->class_key )\n\n");
        exit(-1);
    }

    if ( 1 == context.member_definition_outside ) {
        //## maybe is good idea check if it has beed declared and save the file where are his definition
        if (1 == options.verbose_flag) {
            printf("## c_semantic::class_member_declarator() this is outside declaration the method must be declared before\n");
        }
//              return;
    }

    int is_constructor = 0;
    int is_destructor = 0;
    int is_function = 0;

    if ( context.class_name_declaration == token.text ||
            chain_is_tail(context.class_name_declaration, (char *)token.text.c_str() )
       ) {
        if ( 1 == context.class_member.is_destructor) {
            is_destructor = 1;
            is_function = 1;
        } else {
            is_constructor = 1;
            is_function = 1;
        }
    }

    c_class_member class_member(token,
                                vector_decl_specifier);

    class_member.is_constructor = is_constructor;
    class_member.is_destructor = is_destructor;
    class_member.is_function = is_function;
    // here i get the class_key from the symbol
    // ## but i think it should be avalible in
    //    context.access_specifier
    //    context.class_key
    // tested with t010.cpp
    //   the first access_specifier is lost, i must do this:

    if ( 0 == context.access_specifier) {
        context.class_key = p_symbol->class_key;
        switch (p_symbol->class_key) {
        case CLASS:
            context.access_specifier = PRIVATE;
            break;
        case STRUCT:
            context.access_specifier = PUBLIC;
            break;
        case NAMESPACE:
            context.access_specifier = PUBLIC;
            break;
        default:
            printf
            ("error c_semantic::class_member_declarator()  0 == p_symbol->class_key )\n\n");
            exit(-1);
        }
    }

    if ( 1 == context.is_template_instantiation ) {
        class_member.is_template_instantiation = context.is_template_instantiation;
        class_member.vector_template_argument = context.vector_template_argument;
        class_member.map_template_argument    = context.map_template_argument;
    }

    class_member.access_specifier = context.access_specifier;
    context.class_member = class_member;

    return;

}
/*----------------------------------------------------------------------------*/
void
c_semantic::member_param_declarator(c_context & context, c_token token)
/*
 * p1 is param of function member f1
 *
 * class A { int f1(int p1); };
 */
{
    if (1 == options.verbose_flag) {
        printf
        ("## c_semantic::member_param_declarator(c_context context [%s])\n\n",
         token.text.c_str());
    }

    if (CLASS_SPECIFIER_STATUS_MEMBER_DECLARATOR !=
            context.class_specifier_status) {
        printf("error c_semantic::member_param_declarator() "
               "CLASS_SPECIFIER_STATUS_MEMBER_DECLARATOR != context.class_specifier_status\n\n");
        exit(-1);
    } else {
        c_symbol *p_symbol =
            ts.search_symbol(context.class_name_declaration);
        if (p_symbol) {
            if (0 == p_symbol->class_key) {
                printf
                ("error c_semantic::member_param_declarator()  0 == p_symbol->class_key )\n\n");
                exit(-1);
            }

            c_parameter parameter(token,
                                  context.param_vector_decl_specifier);
            context.class_member.is_function = 1;
            context.class_member.parameter_insert(parameter);

            return;
        }
    }
}
/*----------------------------------------------------------------------------*/
void
c_semantic::free_function_param_declarator(c_context & context, c_token token)
{
    if (1 == options.verbose_flag) {
        printf
        ("## c_semantic::free_function_param_declarator(c_context context [%s])\n\n",
         token.text.c_str());
    }

    c_parameter parameter(token,context.param_vector_decl_specifier);
    context.declarator.is_function = 1;
    context.declarator.parameter_insert(parameter);
}
/*----------------------------------------------------------------------------*/
void
c_semantic::free_declarator(c_context & context, c_token token)
{
    if (1 == options.verbose_flag) {
        printf("## c_semantic::free_declarator()\n\n");
    }

    if (NO_CLASS_STATUS !=
            context.class_specifier_status) {
        printf
        ("error c_semantic::free_declarator() NO_CLASS_STATUS !=	context.class_specifier_status\n\n");
        exit(-1);
    }

    c_declarator declarator(token, vector_decl_specifier);
    context.declarator = declarator;
    /*
      template <class myType>
      myType GetMax (myType a, myType b) {
        return (a>b?a:b);
      }
      we are in GetMax line ...
    */
    if ( TEMPLATE_DECLARATION == context.template_status ) {
        context.declarator.is_template = 1;
    }
}
/*----------------------------------------------------------------------------*/
/*
  maybe is something like this void A::fa1(void){...
  we must put the context like we are inside of the class A
  ## -> ensure this context is clear when the } of member is consumed
*/
void c_semantic::check_coloncolon_member_function(c_context & context, c_token token)
{
    if (1 == options.verbose_flag) {
        printf("## c_semantic::check_coloncolon_member_function(c_context context) token.text[%s]\n",token.text.c_str());
        printf("##                                            context.class_name_declaration.[%s]\n\n",context.class_name_declaration.c_str());
    }

    if ( 0 == vector_decl_specifier.size() ) {
        if (1 == options.verbose_flag) {
            printf("## c_semantic::check_coloncolon_member_function(c_context context) 0 == vector_decl_specifier.size()\n");
        }
        return;
    }

    unsigned last = vector_decl_specifier.size() - 1;

    if ( 1 == vector_decl_specifier[last].has_colon_colon_after ) {

        context.i_am_in_member = 1;
        context.member_definition_outside = 1;
        context.class_specifier_status = CLASS_SPECIFIER_STATUS_MEMBER_DECLARATOR;

        if ( 0 == context.class_name_declaration.size() ) {
            context.class_name_declaration = vector_decl_specifier[last].token.text;
        }

        return;
    }

    /*
      if the chains begin with template the program did not enter
      in the previous if
    */
    c_symbol *p_symbol_class =
        ts.search_symbol(context.class_name_declaration);

    if (0 != p_symbol_class ) {
        /*
                c_class_member * p_member = 0;

                p_member = p_symbol_class->members.get(context.class_member.get_full_name());

                if ( 0 == p_member ) {
                    return;
                }
        */
        context.i_am_in_member = 1;
        context.member_definition_outside = 1;
        context.class_specifier_status = CLASS_SPECIFIER_STATUS_MEMBER_DECLARATOR;
    }
}
/*----------------------------------------------------------------------------*/
void c_semantic::identifier_typedef(c_context & context, c_token token)
{
    if (1 == options.verbose_flag) {
        printf("## void c_semantic::identifier_typedef(c_token token) token.text[%s]\n\n",token.text.c_str());
    }

    /*
      ## it must search in the stack

      stack 0
        class A{};

      stack 1
        int A;

      typedef class A t_A;

      typedef vector < c_decl_specifier > t_vector_decl_specifier;
    */

    if ( 0 != context.class_name_declaration.size() ) {
        c_symbol symbol(token);

        symbol.is_typedef = 1;
        symbol.typedef_points_to = context.class_name_declaration;
        symbol.type = TYPEDEF_NAME;

        if ( 1 == context.is_template_instantiation ) {
            symbol.is_template_instantiation = context.is_template_instantiation;
            symbol.vector_template_argument = context.vector_template_argument;
            symbol.map_template_argument    = context.map_template_argument;
        }

        ts.insert(symbol);
    }

    if (NO_CLASS_STATUS ==
            context.class_specifier_status) {
        c_declarator declarator(token, vector_decl_specifier);
        declarator.is_typedef = 1;

        if ( 1 == context.is_template_instantiation ) {
            declarator.is_template_instantiation = context.is_template_instantiation;
            declarator.vector_template_argument = context.vector_template_argument;
            declarator.map_template_argument    = context.map_template_argument;
        }
        context.declarator = declarator;
    }
}

/*----------------------------------------------------------------------------*/
void c_semantic::identifier_enum(c_context & context, c_token token)
{
    if (1 == options.verbose_flag) {
        printf("## void c_semantic::identifier_enum(c_token token) token.text[%s]\n\n",token.text.c_str());
    }

    c_symbol symbol(token);

    symbol.type = ENUM_NAME;

    if ( 0 != context.class_name_declaration.size() ) {
        string s = symbol.token.text;
        symbol.token.text = context.class_name_declaration + "::" + s;
        symbol.text = symbol.token.text;
    } else {
        if (0 != context.namespace_name_declaration.size() ) {
            string s = symbol.token.text;
            symbol.token.text = context.namespace_name_declaration + "::" + symbol.text;
            symbol.text = symbol.token.text;
        }
    }

    ts.insert(symbol);
}
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
void c_semantic::identifier(c_context & context, c_token token)
{
    if (1 == options.verbose_flag) {
        printf("## c_semantic::identifier(c_context context) token.text[%s]\n",token.text.c_str());
    }
    /*##deprectated
      if ( 0 < vector_decl_specifier.size() )
        {
          if ( TYPEDEF == vector_decl_specifier[0].token.id )
            {
              identifier_typedef(token);
              return;
            }
        }
    */
    if ( TEMPLATE_PARAMETER_LIST == context.template_status ) {
        if ( 1 == context.declaring_template_type ) {
            context.template_parameter.token = token;

            context.vector_template_parameter.push_back(context.template_parameter);
            context.map_template_parameter[token.text] = context.template_parameter;
            //parameter has been processed
            context.template_parameter.clear();

            return;
        }
    }

    if ( 1 == context.is_typedef) {
        identifier_typedef(context, token);
        return;
    }

    if ( 1 == context.is_enum_declaration) {
        identifier_enum(context, token);
        return;
    }

    if (CLASS_SPECIFIER_STATUS_IDENTIFIER ==
            context.class_specifier_status) {
        class_specifier_identifier(context, token);
    }

    // maybe is something like this void A::fa1(void){...
    if (CLASS_SPECIFIER_STATUS_MEMBER_DECLARATOR !=
            context.class_specifier_status) {
        check_coloncolon_member_function(context, token);
    }

    if (CLASS_SPECIFIER_STATUS_MEMBER_DECLARATOR ==
            context.class_specifier_status) {
        if (1 == context.i_am_in_parameter_declaration) {
            member_param_declarator(context, token);
            return;
        }

        if (1 == context.i_am_in_member) {
            class_member_declarator(context, token);
            context.member_declaration = token.text;
        }

        return;
    }

    // indentifier out of a class, struct...
    if (NO_CLASS_STATUS ==
            context.class_specifier_status) {
        if (1 == context.i_am_in_parameter_declaration) {
            free_function_param_declarator(context, token);
            return;
        }

        if (0 == context.i_am_in_member) {
            if (context.namespace_name_declaration.size() > 0) {
                token.text = context.namespace_name_declaration + "::" + token.text;
            }

            free_declarator(context, token);
            context.declaration =  token.text;
        }

        return;
    }
}

/*----------------------------------------------------------------------------*/
void c_semantic::class_name(c_context & context, c_token token)
/*
 * contexts: class A: public B // B enter for this way
 * BASE_CLASS_DECLARATION
 */
{
    if (1 == options.verbose_flag) {
        printf("## c_semantic::class_name() token.text[%s]\n",token.text.c_str());
        printf("##       context.i_am_in_member[%d]\n",context.i_am_in_member);
        printf("##       context.class_specifier_status[%s]\n",table_parser_status[context.class_specifier_status]);
    }


    // we are in the base class declaration
    // example class A : public B
    if (CLASS_SPECIFIER_STATUS_BASE_DECLARATION ==
            context.class_specifier_status) {
        // printf("\n\n### WE ADDING BASES CLASS TO
        // [%s]\n\n",context.class_name_declaration.c_str());

        c_symbol *p_symbol =
            ts.search_symbol(context.class_name_declaration);
        if (p_symbol) {
            if (0 == context.access_specifier) {
                context.access_specifier = PUBLIC;
            }

            c_base_class base_class(token.text, context.access_specifier);
            p_symbol->insert_base_class(base_class);

            return;
        }
    }

    if (CLASS_SPECIFIER_STATUS_MEMBER_DECLARATOR ==
            context.class_specifier_status) {
        // void f_B(B b); b is a object of the class B
        if (1 == context.i_am_in_parameter_declaration) {
            /* it will be processed when b be consumed
                      member_param_declarator(context, token);
            */
            return;
        }

        if (1 == context.i_am_in_member) {
            class_member_declarator(context, token);
            context.member_declaration = token.text;
        }

        return;
    }

    // maybe is something like this void A::A(){...
    if (
        NO_CLASS_STATUS ==
        context.class_specifier_status) {
        check_coloncolon_member_function(context, token);

        if (
            CLASS_SPECIFIER_STATUS_MEMBER_DECLARATOR !=
            context.class_specifier_status) {
            return;
        }

        if (1 == context.i_am_in_member) {
            class_member_declarator(context, token);
            context.member_declaration = token.text;
        }

        return;
    }
    if (1 == options.verbose_flag) {
        printf("## c_semantic::class_name() token.text[%s] END\n",token.text.c_str());
    }
}

/*----------------------------------------------------------------------------*/

void
c_semantic::class_name_friend(c_context & context, c_token token)
{
    if ( CLASS_SPECIFIER_STATUS_FRIEND_DECLARATOR !=
            context.class_specifier_status) {
        printf
        ("error c_semantic::class_name_friend() CLASS_SPECIFIER_STATUS_FRIEND_DECLARATOR ==	context.class_specifier_status\n");
        exit(1);
    }

//    token.print(" ");

//    {
    // printf("\n\n### WE ADDING BASES CLASS TO
    // [%s]\n\n",context.class_name_declaration.c_str());

    c_symbol *p_symbol =
        ts.search_symbol(context.class_name_declaration);
    if (p_symbol) {
        c_friend_class friend_class(token.text);
        p_symbol->insert_friend_class(friend_class);

        return;
    }
//    }

    return;
}

/*----------------------------------------------------------------------------*/
void c_semantic::member_insert(string & tab, c_context & context)
/*
 * int f1(int p1); has been parsed and we can insert in ts
 *
 * class A { int f1(int p1); };
 */
{
    if ( context.class_member.token.text.size() <= 0) {
        if (1 == options.verbose_flag) {
            printf("%s##error c_semantic::member_insert(c_context context) member empty\n"
                   ,tab.c_str()
                  );
        }
        return;
    }
    if (1 == options.verbose_flag) {
        printf("%s## c_semantic::member_insert(c_context context)[%s]\n"
               ,tab.c_str()
               ,context.class_member.token.text.c_str()
              );
    }

    c_symbol *p_symbol = ts.search_symbol(context.class_name_declaration);
    if (p_symbol) {
        if (0 == p_symbol->class_key) {
            printf
            ("error c_semantic::member_param_declarator()  0 == p_symbol->class_key )\n\n");
            exit(-1);
        }
        //## it must bee declared yet
        // todo -> put the file where is the definition
        if ( 1 == context.member_definition_outside ) {
            c_class_member * p_member = 0;

            p_member = p_symbol->members.get(context.class_member.get_full_name());

            if ( 0 == p_member ) {
                return;
            }

            p_member->token_definition = context.class_member.token;
            return;
        }

        p_symbol->members.insert(context.class_member);
    }
}
/*----------------------------------------------------------------------------*/
void c_semantic::declarator_insert(string tab, c_context & context)
{
    if (1 == options.verbose_flag) {
        printf("%s## c_semantic::declarator_insert(c_context context) context.declarator.token[%s]\n"
               , tab.c_str()
               , context.declarator.get_full_name().c_str()
              );
    }

    if (NO_CLASS_STATUS ==
            context.class_specifier_status) {
        if ( 0 == context.declarator.token.text.size() ) {
            if (1 == options.verbose_flag) {
                printf("%s## warning c_semantic::declarator_insert context.declarator.token.text.size() == 0\n"
                       , tab.c_str()
                      );
                printf("%s##   aborting ts.insert(symbol); context.class_member.token.text[%s]\n", tab.c_str()
                       , context.class_member.token.text.c_str()
                      );
                printf("%s##   maybe this is the definition of function member and this is ok\n", tab.c_str()
                      );
            }
            return;
        }
        c_symbol symbol(context.declarator.token);
        symbol.free_declarator = 1;
        symbol.declarator = context.declarator;
        if ( 1 == context.declarator.is_typedef ) {
            symbol.is_typedef = 1;
            symbol.type = TYPEDEF_NAME;

            //typedef int t_int;
            //typedef t_int t2_int;
            // t2_int should point to t_int
            // t_int  should point to "" nothing
            if ( 0 != context.declarator.vector_decl_specifier.size() ) {
                c_symbol * p = ts.search_symbol(context.declarator.vector_decl_specifier[0].token.text);

                if ( 0 != p ) {
                    symbol.typedef_points_to = p->token.text;
                }
            }
        }

        // void f(int p1) -> his name will be "f(int)"
        if ( 1 == context.declarator.is_function ) {
            symbol.text = context.declarator.get_full_name();
            if ( TEMPLATE_DECLARATION == context.template_status ) {
                symbol.is_template = 1;
                symbol.vector_template_parameter = context.vector_template_parameter;
                symbol.map_template_parameter = context.map_template_parameter;
                symbol.process_token_text();
            }
        }

        if ( 1 == context.is_template_instantiation ) {
            symbol.is_template_instantiation = context.is_template_instantiation;
            symbol.vector_template_argument = context.vector_template_argument;
            symbol.map_template_argument    = context.map_template_argument;
        }

        // extern declaration
        c_symbol *p_symbol_search = ts.search_symbol(symbol.text);
        if ( p_symbol_search ) {
            if ( 1 == p_symbol_search->declarator.is_extern) {
                p_symbol_search->declarator.extern_points_token.save(symbol.declarator.token);
                return;
            }
        }

        ts.insert(symbol);
    } else {
        member_insert(tab, context);
    }
}
/*----------------------------------------------------------------------------*/
void
c_semantic::namespace_declarator(c_context & context, c_token token)
{
    c_symbol symbol(token);
    if (1 == options.verbose_flag) {
        printf("## c_semantic::namespace_declarator(c_context context) token[%s]\n"
               ,token.text.c_str());
    }

    // yes a namespace will be parse like a class sometimes...
    symbol.class_key = NAMESPACE;
    symbol.type = NAMESPACE_NAME;

    ts.insert(symbol);
}
/*----------------------------------------------------------------------------*/
/*
  now we are in 2

  //1
  class Value;

  //2
  class Value {
    int v;
  };


  and is better have the file and the line of the point 2
*/
void
c_semantic::class_pre_declaration_to_declaration(c_context & context, c_token token)
{
    c_symbol *p_symbol =
        ts.search_symbol(context.class_name_declaration);

    if (0 == p_symbol ) {
        printf("## c_semantic::class_pre_declaration_to_declaration() context.class_name_declaration[%s] not found!\n",context.class_name_declaration.c_str());
        return;
    }

    if (1 == options.verbose_flag) {
        printf("## c_semantic::class_pre_declaration_to_declaration() context.class_name_declaration[%s] founded\n",context.class_name_declaration.c_str());
    }

    if (0 == p_symbol->class_key) {
        printf
        ("error c_semantic::class_pre_declaration_to_declaration()  0 == p_symbol->class_key )\n\n");
        exit(-1);
    }

    if (0 == context.class_pre_declaration ) {
        printf
        ("error c_semantic::class_pre_declaration_to_declaration() 0 == context.class_pre_declaration )\n\n");
        exit(-1);
    }

    p_symbol->token = token;

    ts.insert(*p_symbol);
}
/*----------------------------------------------------------------------------*/
void
c_semantic::class_is_abstract(c_context & context)
{
    c_symbol *p_symbol = 0;
    p_symbol = ts.search_symbol(context.class_name_declaration);
    if ( 0 != p_symbol) {
        p_symbol->is_abstract = 1;
    }
}
/*----------------------------------------------------------------------------*/
void
c_semantic::template_name_search_and_set_context(c_context & context, c_token token)
{
    c_symbol *p_symbol = ts.search_symbol(token.text.c_str());

    if (p_symbol) {
        if (p_symbol->type != 0) {
            if ( 1 == p_symbol->is_template ) {
                context.map_template_parameter = p_symbol->map_template_parameter;
                context.vector_template_parameter = p_symbol->vector_template_parameter;
            }
        }
    }
}
/*----------------------------------------------------------------------------*/
void
c_semantic::template_instantiation_argument(c_context & context, c_decl_specifier & decl)
{
    if (1 == options.verbose_flag) {
        printf("## c_semantic::template_instantiation_argument(c_context & context, c_decl_specifier & decl) decl.token[%s]\n"
               , decl.token.text.c_str()
              );
    }

    c_template_argument argument;

    // yes we need the next parameter to fill vector_argument
    size_t i = context.vector_template_argument.size();

    if ( 0 < i ) {
        argument = context.vector_template_argument[i - 1];
        context.vector_template_argument.pop_back();

        argument.vector_decl_specifier.push_back(decl);
        context.vector_template_argument.push_back(argument);
    }
}
/*----------------------------------------------------------------------------*/
c_semantic semantic;
/*----------------------------------------------------------------------------*/
