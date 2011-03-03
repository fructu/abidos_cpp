/*------------------------------------------------------------------------------

    Proyecto            : show_includes
    Codigo              : parser_descent.h
    Descripcion         :
    Version             : 0.1
    Autor               : F. Manuel Hevia Garcia
    Fecha creacion      : 03/03/2011
    Fecha modificacion  :

    Observaciones :


------------------------------------------------------------------------------*/
#ifndef parser_descent_h
#define parser_descent_h

#include "lex_yacc.h"

class c_parser_descent
{
  private:
    int yy_actual;
    
    int token_get(void);
    void token_next(void);
  public:    
    c_parser_descent();
    ~c_parser_descent();
    void yyparse(char * file_name);
};

#endif
