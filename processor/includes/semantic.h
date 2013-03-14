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

typedef vector<string> t_vector_using_namespace;
typedef map<string, string> t_map_using_namespace;

class c_semantic
{
private:
    t_vector_decl_specifier vector_decl_specifier;
    /*
      templates
    template <class myType>
    myType GetMax (myType a, myType b) {
     return (a>b?a:b);
    }
      1) this vector is to store myType in the line template <class myType>

      2) in de declaration of GetMax myType will be search in this vector
    */
//    t_vector_template_parameter vector_template_parameter;
//    t_map_template_parameter map_template_parameter;
//### this functionality is in context object

public:
    void push_decl_specifier(c_token token, c_context &context) {
        c_decl_specifier decl(token);
        decl.type_specifier = 1;

        if (1 == context.i_am_in_parameter_declaration) {
            context.param_vector_decl_specifier.push_back(decl);
        } else {
            push_back_vector_decl_specifier(decl);
        }
    }

    void push_back_vector_decl_specifier(c_decl_specifier decl) {
        vector_decl_specifier.push_back(decl);
    }

    /*
      in:
        int *a, *b;
      we must remove the * of a and restore de * of b

      :-) yeeeees think is b have &
        int *a, &b;
    */
    void pop_last_pointers(void) {
//        int last_id = 0;
        c_decl_specifier last_decl;

        while ( vector_decl_specifier.size() != 0) {
            if ('*' == vector_decl_specifier.back().token.id ||
                    '&' == vector_decl_specifier.back().token.id
               ) {
                if ( 0 == last_decl.token.id) {
                    last_decl = vector_decl_specifier.back();
                }
                vector_decl_specifier.pop_back();
            } else {
                vector_decl_specifier.push_back(last_decl);
                return;
            }
        }
    }

    void clear_decl_specifier(void) {
        vector_decl_specifier.clear();
    }

    void print_decl_specifier(void) {
        unsigned i = 0;

        printf("## print_decl_specifier()\n");
        printf("## {\n");
        for ( i = 0; i < vector_decl_specifier.size(); ++i) {
            printf("  %s", vector_decl_specifier[i].token.text.c_str());
            if ( 1 == vector_decl_specifier[i].has_colon_colon_after ) {
                printf("::");
            }
        }
        printf("\n");
        printf("## }\n");
    }

    int vector_decl_specifier_is_extern(void) {
        unsigned i = 0;

        for ( i = 0; i < vector_decl_specifier.size(); ++i) {
            if ( EXTERN == vector_decl_specifier[i].token.id) {
                return 1;
            }
        }

        return 0;
    }

    /*
        in vector_decl_specifier can be
          void A::
        or
          A::
    */
    string get_chain_head(void) {
        int unsigned i = 0;

        for (i = 0;  i < vector_decl_specifier.size(); ++i ) {
            if ( 1 == vector_decl_specifier[i].has_colon_colon_after ) {
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

    t_vector_using_namespace vector_using_namespace;
    t_map_using_namespace    map_using_namespace;

    void push_using_namespace(string s) {
        vector_using_namespace.push_back(s);
        map_using_namespace[s] = s;
    }

    int is_a_using_namespace(string str) {
        size_t i = 0;
        for (i = 0;  i < vector_using_namespace.size(); ++i ) {
            if ( str != vector_using_namespace[i] ) {
                continue;
            }
            c_symbol *p_symbol = ts.search_symbol(str.c_str());
            if (p_symbol) {
                // we are in this case:
                //  using namespace std;
                //  std::string;
                if ( NAMESPACE_NAME == p_symbol->type) {
                    return 1;
                }
            }
        }

        return 0;
    }

    void check_coloncolon_member_function(c_context & context, c_token token);

    void identifier_typedef(c_context & context, c_token token);
    void identifier_enum(c_context & context, c_token token);
    void identifier(c_context & context, c_token token);
    void class_specifier_identifier(c_context & context, c_token token);
    void class_member_declarator(c_context & context, c_token token);
    void member_param_declarator(c_context & context, c_token token);
    void free_function_param_declarator(c_context & context, c_token token);
    void free_declarator(c_context & context, c_token token);
    void member_insert(string & tab, c_context & context);

    void namespace_declarator(c_context & context, c_token token);
    void declarator_insert(string tab, c_context & context);

    void class_name(c_context & context, c_token token);
    void class_name_friend(c_context & context, c_token token);

    void class_pre_declaration_to_declaration(c_context & context, c_token token);
    void class_is_abstract(c_context & context);

    void template_instantiation_argument(c_context & context, c_decl_specifier & decl);
};

extern c_semantic semantic;
#endif
