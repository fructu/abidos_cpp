/*------------------------------------------------------------------------------

    Proyecto            : abidos
    Codigo              : semantic.h
    Descripcion         :
    Version             : 0.1
    Autor               : F. Manuel Hevia Garcia
    Fecha creacion      : 21/03/2011
    Fecha modificacion  :

    Observaciones :


------------------------------------------------------------------------------*/
#ifndef semantic_h
#define semantic_h

#include "parser_descent.h"
#include "symbols_table.h"

class c_semantic
{
  public:
    void identifier(c_context & context, c_token token);
    void class_name(c_context & context, c_token token);
};

extern c_semantic semantic;
#endif