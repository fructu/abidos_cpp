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

  void print_decl_specifier(void)
  {
    unsigned i = 0;

    printf("## print_decl_specifier()\n");
    printf("## {\n");
    for ( i = 0; i < vector_decl_specifier.size(); ++i)
      {
        printf("  %s", vector_decl_specifier[i].token.text.c_str());
        if ( 1 == vector_decl_specifier[i].has_colon_colon_after )
          {
            printf("::");
          }
      }
    printf("\n");
    printf("## }\n");
  }

  /*
      in vector_decl_specifier can be
        void A::
      or
        A::
  */
  string get_chain_head(void)
  {
    int unsigned i = 0;

    for (i = 0;  i < vector_decl_specifier.size(); ++i )
      {
        if ( 1 == vector_decl_specifier[i].has_colon_colon_after )
          {
            return vector_decl_specifier[i].token.text;
          }
      }

    return "";
  }
  /*
    c_decl_specifier get_last_decl_specifier(void)
    {
      c_token token;
      c_decl_specifier dcl(token);
      unsigned len = vector_decl_specifier.size();
      if( 0 == len )
        {
          return dcl;
        }

      dcl = vector_decl_specifier[len-1];
      return dcl;
    }
  */

  void check_coloncolon_member_function(c_context & context, c_token token);

  void identifier_typedef(c_context & context, c_token token);
  void identifier(c_context & context, c_token token);
  void class_specifier_identifier(c_context & context, c_token token);
  void class_member_declarator(c_context & context, c_token token);
  void member_param_declarator(c_context & context, c_token token);
  void free_function_param_declarator(c_context & context, c_token token);
  void free_declarator(c_context & context, c_token token);
  void member_insert(string & tab, c_context & context);

  void declarator_insert(string tab, c_context & context);

  void class_name(c_context & context, c_token token);
  void class_name_friend(c_context & context, c_token token);
};

extern c_semantic semantic;
#endif
