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
private:
  t_vector_decl_specifier vector_decl_specifier;

public:
  void push_back_vector_decl_specifier(c_decl_specifier decl)
  {
    vector_decl_specifier.push_back(decl);
  }

  void clear_decl_specifier(void)
  {
    vector_decl_specifier.clear();
  }

  void identifier(c_context & context, c_token token);
  void class_specifier_identifier(c_context & context, c_token token);
  void class_member_declarator(c_context & context, c_token token);
  void member_param_declarator(c_context & context, c_token token);
  void free_function_param_declarator(c_context & context, c_token token);
  void free_declarator(c_context & context, c_token token);
  void member_insert(c_context & context);
  
  void declarator_insert(c_context & context);

  void class_name(c_context & context, c_token token);
};

extern c_semantic semantic;
#endif
