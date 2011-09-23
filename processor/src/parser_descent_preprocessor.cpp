/*------------------------------------------------------------------------------

    Proyecto            : show_includes
    Codigo              : parser_descent_preprocessor.cpp
    Descripcion         :
    Version             : 0.1
    Autor               : F. Manuel Hevia Garcia
    Fecha creacion      : 26/05/2011
    Fecha modificacion  :

    Observaciones :


------------------------------------------------------------------------------*/
#include <stdlib.h>
#include <string.h>
#include "parser_descent.h"
#include "options.h"
#include "tokens.h"
#include "generator_class_diagram.h"
#include "generator_original.h"
#include "trace.h"

#include "../../preprocessor/ts.h"
/*----------------------------------------------------------------------------*/
/*
 * preprocessor_include: '#' PREPROCESSOR_INCLUDE STRING
 */
int c_parser_descent::preprocessor_include(c_trace_node trace_node)
{
    trace_graph.add(trace_node, "preprocessor_include");

    c_context_tokens context_tokens(context);

    token_next(trace_node.get_tab());
    if ( token_is_not('#', trace_node) ) {
        context = context_tokens.restore();
        return 0;
    }

    token_next(trace_node.get_tab());
    if ( token_is_not(PREPROCESSOR_INCLUDE, trace_node) ) {
        context = context_tokens.restore();
        return 0;
    }

/*
  a include may be:
    #include "file.h"
  or
    #include <iostream>
  the problem is STRING has " inside
*/
    token_next(trace_node.get_tab());
    if ( token_is_not(STRING, trace_node) ) {
        if ( token_is_not('<', trace_node) ) {
          context = context_tokens.restore();
          return 0;
        }

        token_next(trace_node.get_tab());
        while( token_is_not('>', trace_node) ) {
          //## todo acumulate string with the file
          token_next(trace_node.get_tab());
          if ( 1 == is_eof(trace_node) ) {
            return 0;
          }
        }

        if ( token_is('>', trace_node) ) {
          return 1;
        }

        context = context_tokens.restore();
        return 0;
    }

    c_cell file_actual;

    file_actual.fill(lex_file_name, (char *) "\"");

    char file_without_commillas[ID_MAX_LEN]={'\0'};

    sprintf(file_without_commillas,"%s",c_token_get().text.c_str());
    str_drop_char(file_without_commillas, '\"');

    c_cell file_included;

    file_included.fill(file_without_commillas, (char *) "\"");

    file_actual.path_resolve(file_included);

    if (1 != lex_file_push( file_included.full() ) ) {
        printf("\nERROR:c_parser_descent::preprocessor_include[%s]\n", file_included.full() );
        exit(-1);
    }

    return 1;
}
/*----------------------------------------------------------------------------*/

