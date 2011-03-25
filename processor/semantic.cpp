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
/*----------------------------------------------------------------------------*/
void c_semantic::identifier(c_context & context, c_token token)
{
    printf("## c_semantic::identifier(c_context context)\n\n");
    
    c_symbol symbol(token);

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
    if( BASE_CLASS_DECLARATION == context.class_declaration)
    {
        context.class_name_declaration = token.text;
    }

    ts.insert(symbol);
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
    if( BASE_CLASS_DECLARATION == context.class_declaration)
    {
      //printf("\n\n###  WE ADDING BASES CLASS TO [%s]\n\n",context.class_name_declaration.c_str());

      c_symbol * p_symbol = ts.search_symbol( context.class_name_declaration );
      if( p_symbol )
      {
        c_base_class base_class(token.text, context.access_specifier);
        p_symbol->map_base_class[token.text] = base_class;

        return;
      }
    }
}
/*----------------------------------------------------------------------------*/
c_semantic semantic;
/*----------------------------------------------------------------------------*/
