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
//void c_semantic::member_declarator(c_context & context, c_token token)
//{
//	context.member_declarator = 1;
//}
/*----------------------------------------------------------------------------*/
void c_semantic::class_specifier_identifier(c_context & context, c_token token)
{
    c_symbol symbol(token);
	printf("## c_semantic::class_(c_context context)\n\n");
    if( CLASS_SPECIFIER_STATUS_IDENTIFIER != context.class_specifier_status)
    {
	    printf("##error c_semantic::class_ CLASS_IDENTIFIER != context.class_specifier_status\n\n");
    	return;
    }

    if( CLASS == context.class_key )
    {
        symbol.type = CLASS_NAME;
        symbol.class_key = context.class_key;
    }

    if( STRUCT == context.class_key )
    {
        symbol.type = CLASS_NAME;
        symbol.class_key = context.class_key;
    }

    //## todo
    if( UNION == context.class_key )
    {
        symbol.type = UNION;
        symbol.class_key = context.class_key;
    }

    // we only take the name of the class the fist time after CLASS
//    if( 1 == context.class_head)
    {
        context.class_name_declaration = token.text;
    }

    ts.insert(symbol);
}
/*----------------------------------------------------------------------------*/
/*
	a member is for example a1

	class A
	{
	  int a1;
	};
*/
void c_semantic::class_member_declarator(c_context & context, c_token token)
{
    printf("## c_semantic::class_member_declarator()\n\n");

	if( CLASS_SPECIFIER_STATUS_MEMBER_DECLARATOR !=	context.class_specifier_status )
	{
      printf("error c_semantic::class_member_declarator() CLASS_SPECIFIER_STATUS_MEMBER_DECLARATOR !=	context.class_specifier_status\n\n");
      exit(-1);
    }
    else
    {
      c_symbol * p_symbol = ts.search_symbol( context.class_name_declaration );
      if( p_symbol )
      {
      /*
        c_base_class base_class(token.text, context.access_specifier);
        p_symbol->map_base_class[token.text] = base_class;
	  */
	    if( 0 == p_symbol->class_key )
	    {
	    	printf("error c_semantic::class_member_declarator()  0 == p_symbol->class_key )\n\n");
	    	exit(-1);
	    }

	    c_class_member class_member( token , vector_decl_specifier);

	    p_symbol->map_class_member[token.text] = class_member;

        return;
      }
    }
}
/*----------------------------------------------------------------------------*/
void c_semantic::identifier(c_context & context, c_token token)
{
    printf("## c_semantic::identifier(c_context context)\n\n");

	if( CLASS_SPECIFIER_STATUS_IDENTIFIER == context.class_specifier_status)
	{
		class_specifier_identifier(context, token);
	}

	if( CLASS_SPECIFIER_STATUS_MEMBER_DECLARATOR ==	context.class_specifier_status )
	{
		class_member_declarator(context, token);
	}
}
/*----------------------------------------------------------------------------*/
/*
  contexts:
   class A: public B // B enter for this way BASE_CLASS_DECLARATION
*/
void c_semantic::class_name(c_context & context, c_token token)
{
    //we are in the base class declaration
    // example class A : public B
    if( CLASS_SPECIFIER_STATUS_BASE_DECLARATION == context.class_specifier_status)
    {
      //printf("\n\n###  WE ADDING BASES CLASS TO [%s]\n\n",context.class_name_declaration.c_str());

      c_symbol * p_symbol = ts.search_symbol( context.class_name_declaration );
      if( p_symbol )
      {
      	if( 0 == context.access_specifier )
      	{
      		context.access_specifier = PUBLIC;
      	}

        c_base_class base_class(token.text, context.access_specifier);
        p_symbol->map_base_class[token.text] = base_class;

        return;
      }
    }
}
/*----------------------------------------------------------------------------*/
c_semantic semantic;
/*----------------------------------------------------------------------------*/

