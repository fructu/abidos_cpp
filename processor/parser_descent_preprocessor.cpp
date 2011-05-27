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

    token_next(trace_node.get_tab());
    if ( token_is_not(STRING, trace_node) ) {
        context = context_tokens.restore();
        return 0;
    }

    if (1 != lex_file_push(c_token_get().text.c_str())) {
        printf("\nERROR:c_parser_descent::preprocessor_include[%s]\n", c_token_get().text.c_str() );
        exit(-1);
    }

    return 1;
}
/*----------------------------------------------------------------------------*/

