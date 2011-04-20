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

#include <stdio.h>
#include <stdlib.h>
/*----------------------------------------------------------------------------*/
void
c_semantic::class_specifier_identifier(c_context & context, c_token token)
{
  c_symbol symbol(token);
  printf("## c_semantic::class_(c_context context)\n\n");

  if (CLASS_SPECIFIER_STATUS_IDENTIFIER !=
      context.class_specifier_status)
    {
      printf
      ("##error c_semantic::class_ CLASS_IDENTIFIER != context.class_specifier_status\n\n");
      return;
    }

  if (CLASS == context.class_key)
    {
      symbol.type = CLASS_NAME;
      symbol.class_key = context.class_key;
    }

  if (STRUCT == context.class_key)
    {
      symbol.type = CLASS_NAME;
      symbol.class_key = context.class_key;
    }
  // ## todo
  if (UNION == context.class_key)
    {
      symbol.type = UNION;
      symbol.class_key = context.class_key;
    }
  // we only take the name of the class the fist time after CLASS
  // if( 1 == context.class_head)
  {
    context.class_name_declaration = token.text;
  }

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
  printf("## c_semantic::class_member_declarator()\n");

  if (CLASS_SPECIFIER_STATUS_MEMBER_DECLARATOR !=
      context.class_specifier_status)
    {
      printf
      ("error c_semantic::class_member_declarator() CLASS_SPECIFIER_STATUS_MEMBER_DECLARATOR !=	context.class_specifier_status\n\n");
      exit(-1);
    }
  else
    {
      c_symbol *p_symbol =
        ts.search_symbol(context.class_name_declaration);
      if (p_symbol)
        {
          if (0 == p_symbol->class_key)
            {
              printf
              ("error c_semantic::class_member_declarator()  0 == p_symbol->class_key )\n\n");
              exit(-1);
            }

          if ( 1 == context.member_definition_outside )
            {
              //## maybe is good idea check if it has beed declared and save the file where are his definition
              printf("## c_semantic::class_member_declarator() this is outside declaration the method must be declared before\n");
              return;
            }

          int is_constructor = 0;
          int is_destructor = 0;
          int is_function = 0;

          if ( context.class_name_declaration == token.text )
            {
              if ( 1 == context.class_member.is_destructor)
                {
                  is_destructor = 1;
                  is_function = 1;
                }
              else
                {
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

          if ( 0 == context.access_specifier)
            {
              context.class_key = p_symbol->class_key;
              switch (p_symbol->class_key)
                {
                case CLASS:
                  context.access_specifier = PRIVATE;
                  break;
                case STRUCT:
                  context.access_specifier = PUBLIC;
                  break;
                default:
                  printf
                  ("error c_semantic::class_member_declarator()  0 == p_symbol->class_key )\n\n");
                  exit(-1);
                }
            }

          class_member.access_specifier = context.access_specifier;
          context.class_member = class_member;

          return;
        }
    }
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
  printf
  ("## c_semantic::member_param_declarator(c_context context [%s])\n\n",
   token.text.c_str());

  if (CLASS_SPECIFIER_STATUS_MEMBER_DECLARATOR !=
      context.class_specifier_status)
    {
      printf("error c_semantic::member_param_declarator() "
             "CLASS_SPECIFIER_STATUS_MEMBER_DECLARATOR != context.class_specifier_status\n\n");
      exit(-1);
    }
  else
    {
      c_symbol *p_symbol =
        ts.search_symbol(context.class_name_declaration);
      if (p_symbol)
        {
          if (0 == p_symbol->class_key)
            {
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
  printf
  ("## c_semantic::free_function_param_declarator(c_context context [%s])\n\n",
   token.text.c_str());

  c_parameter parameter(token,context.param_vector_decl_specifier);
  context.declarator.is_function = 1;
  context.declarator.parameter_insert(parameter);
}
/*----------------------------------------------------------------------------*/
void
c_semantic::free_declarator(c_context & context, c_token token)
{
  printf("## c_semantic::free_declarator()\n\n");

  if (NO_CLASS_STATUS !=
      context.class_specifier_status)
    {
      printf
      ("error c_semantic::free_declarator() NO_CLASS_STATUS !=	context.class_specifier_status\n\n");
      exit(-1);
    }

  c_declarator declarator(token, vector_decl_specifier);
  context.declarator = declarator;
}
/*----------------------------------------------------------------------------*/
/*
  maybe is something like this void A::fa1(void){...
  we must put the context like we are inside of the class A
  ## -> ensure this context is clear when the } of member is consumed
*/
void c_semantic::check_coloncolon_member_function(c_context & context, c_token token)
{
  printf("## c_semantic::check_coloncolon_member_function(c_context context) token.text[%s]\n\n",token.text.c_str());

  if ( 0 == vector_decl_specifier.size() )
    {
      return;
    }

  unsigned last = vector_decl_specifier.size() - 1;

  if ( 1 == vector_decl_specifier[last].has_colon_colon_after )
    {
      printf("## MARK_20 we are in class[%s]\n", vector_decl_specifier[last].token.text.c_str());
      context.i_am_in_member = 1;
      context.member_definition_outside = 1;
      context.class_specifier_status = CLASS_SPECIFIER_STATUS_MEMBER_DECLARATOR;
      context.class_name_declaration = vector_decl_specifier[last].token.text;
    }
}
/*----------------------------------------------------------------------------*/
void c_semantic::identifier(c_context & context, c_token token)
{
  printf("## c_semantic::identifier(c_context context) token.text[%s]\n\n",token.text.c_str());

  if (CLASS_SPECIFIER_STATUS_IDENTIFIER ==
      context.class_specifier_status)
    {
      class_specifier_identifier(context, token);
    }

  // maybe is something like this void A::fa1(void){...
  if (CLASS_SPECIFIER_STATUS_MEMBER_DECLARATOR !=
      context.class_specifier_status)
    {
      check_coloncolon_member_function(context, token);
    }

  if (CLASS_SPECIFIER_STATUS_MEMBER_DECLARATOR ==
      context.class_specifier_status)
    {
      if (1 == context.i_am_in_parameter_declaration)
        {
          member_param_declarator(context, token);
          return;
        }

      if (1 == context.i_am_in_member)
        {
          class_member_declarator(context, token);
          context.member_declaration = token.text;
        }

      return;
    }

  // indentifier out of a class, struct...
  if (NO_CLASS_STATUS ==
      context.class_specifier_status)
    {
      if (1 == context.i_am_in_parameter_declaration)
        {
          free_function_param_declarator(context, token);
          return;
        }

      if (0 == context.i_am_in_member)
        {
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
  // we are in the base class declaration
  // example class A : public B
  if (CLASS_SPECIFIER_STATUS_BASE_DECLARATION ==
      context.class_specifier_status)
    {
      // printf("\n\n### WE ADDING BASES CLASS TO
      // [%s]\n\n",context.class_name_declaration.c_str());

      c_symbol *p_symbol =
        ts.search_symbol(context.class_name_declaration);
      if (p_symbol)
        {
          if (0 == context.access_specifier)
            {
              context.access_specifier = PUBLIC;
            }

          c_base_class base_class(token.text, context.access_specifier);
          p_symbol->insert(base_class);

          return;
        }
    }

  if (CLASS_SPECIFIER_STATUS_MEMBER_DECLARATOR ==
      context.class_specifier_status)
    {
      /*
            if (1 == context.i_am_in_parameter_declaration)
              {
                member_param_declarator(context, token);
                return;
              }
      */
      if (1 == context.i_am_in_member)
        {
          class_member_declarator(context, token);
          context.member_declaration = token.text;
        }

      return;
    }
}

/*----------------------------------------------------------------------------*/
void c_semantic::member_insert(string & tab, c_context & context)
/*
 * int f1(int p1); has been parsed and we can insert in ts
 *
 * class A { int f1(int p1); };
 */
{
  printf("%s## c_semantic::member_insert(c_context context)[%s]\n"
         ,tab.c_str()
         ,context.class_member.token.text.c_str()
        );

  c_symbol *p_symbol = ts.search_symbol(context.class_name_declaration);
  if (p_symbol)
    {
      if (0 == p_symbol->class_key)
        {
          printf
          ("error c_semantic::member_param_declarator()  0 == p_symbol->class_key )\n\n");
          exit(-1);
        }
      //## it must bee declared yet
      // todo -> put the file where is the definition
      if ( 1 == context.member_definition_outside )
        {
          printf("## MARK_30");
          context.class_member.token.print(" ");
          return;
        }
      p_symbol->members.insert(context.class_member);
    }
}
/*----------------------------------------------------------------------------*/
void c_semantic::declarator_insert(string & tab, c_context & context)
{
  printf("%s## c_semantic::declarator_insert(c_context context)\n\n",tab.c_str());
  if (NO_CLASS_STATUS ==
      context.class_specifier_status)
    {
      c_symbol symbol(context.declarator.token);
      symbol.free_declarator = 1;
      symbol.declarator = context.declarator;

      ts.insert(symbol);
    }
  else
    {
      member_insert(tab, context);
    }
}
/*----------------------------------------------------------------------------*/
c_semantic semantic;
/*----------------------------------------------------------------------------*/
