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
void c_semantic::identifier(c_context context, c_token token)
{
    printf("## void c_semantic::identifier(c_context context)\n\n");
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

    ts.insert(symbol);
}
/*----------------------------------------------------------------------------*/
c_semantic semantic;
/*----------------------------------------------------------------------------*/
