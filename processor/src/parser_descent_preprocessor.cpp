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
int c_parser_descent::preprocessor(c_trace_node trace_node)
{
    trace_graph.add(trace_node, "preprocessor");

    if (1 == preprocessor_define(trace_node)) {
        return 1;
    }

    if (1 == preprocessor_include(trace_node)) {
        return 1;
    }

    if (1 == preprocessor_ifndef(trace_node)) {
        return 1;
    }

    if (1 == preprocessor_endif(trace_node)) {
        return 1;
    }

    return 0;
}
/*----------------------------------------------------------------------------*/
/*
 * preprocessor_include: '#' IDENTIFER STRING
 *  ;
 *
 * THIS IS BAD IDEA -> preprocessor_include: '#' PREPROCESSOR_INCLUDE STRING
 * we can write this:
 *    class include {};
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
    /*
      //IDENTIFIER is bad idea:
      //  class include {};
      //  #include <h>

        if ( token_is_not(IDENTIFIER, trace_node) ) {
            context = context_tokens.restore();
            return 0;
        }
    */
    if ( "include" != c_token_get().text ) {
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

//        c_context_tokens context_tokens_good_way(context);

        token_next(trace_node.get_tab());
        while ( token_is_not('>', trace_node) ) {
            //## todo acumulate string with the file
            token_next(trace_node.get_tab());
            /*            if ( 1 == is_eof(trace_node) ) {
                            return 0;
                        }*/
            if ( token_is(0, trace_node) ) {
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
/*
 * preprocessor_define: '#'define <id>
 *  ;
 *
  preprocessor variables can be have the same identifier than other
  classes variables ...
*/
int c_parser_descent::preprocessor_define(c_trace_node trace_node)
{
    trace_graph.add(trace_node, "preprocessor_define");

    c_context_tokens context_tokens(context);

    token_next(trace_node.get_tab());
    if ( token_is_not('#', trace_node) ) {
        context = context_tokens.restore();
        return 0;
    }

    token_next(trace_node.get_tab());
    if ( "define" != c_token_get().text ) {
        context = context_tokens.restore();
        return 0;
    }

    token_next(trace_node.get_tab());
    if ( "" == c_token_get().text ) {
        context = context_tokens.restore();
        return 0;
    }

    c_token token = c_token_get();
    c_symbol symbol(token);
    symbol.type = PREPROCESSOR_DEFINITION;
    symbol.text = "#" + symbol.token.text;
    ts.insert(symbol);

    return 1;
}
/*----------------------------------------------------------------------------*/
/*
  preprocessor_ifndef: #ifndef lines #endif
    ;
  lines: lines #define|other
    ;
*/
int c_parser_descent::preprocessor_ifndef(c_trace_node trace_node)
{
    trace_graph.add(trace_node, "preprocessor_ifndef");

    c_context_tokens context_tokens(context);

    token_next(trace_node.get_tab());
    if ( token_is_not('#', trace_node) ) {
        context = context_tokens.restore();
        return 0;
    }

    context.prefix_sharp = 1;

    token_next(trace_node.get_tab());
    if ( "ifndef" != c_token_get().text ) {
        context = context_tokens.restore();
        return 0;
    }

    token_next(trace_node.get_tab());
    if ( token_is(PREPROCESSOR_DEFINITION, trace_node) ) {
        context.prefix_sharp = 0;
        // we must eat all tokens until #endif
        while ( 1 ) {
            token_next(trace_node.get_tab());
            if ( token_is(0, trace_node) ) {
                return 0;
            }

            if ( token_is_not('#', trace_node) ) {
                continue;
            }

            token_next(trace_node.get_tab());
            if ( "endif" == c_token_get().text ) {
                return 1;
            }
        }
    } else {
        //we dont eat ...
        context.prefix_sharp = 0;
        return 1;
    }

    context = context_tokens.restore();
    return 0;
}
/*----------------------------------------------------------------------------*/
/*
  preprocessor_endif: #endif
    ;
*/
int c_parser_descent::preprocessor_endif(c_trace_node trace_node)
{
    trace_graph.add(trace_node, "preprocessor_endif");

    c_context_tokens context_tokens(context);

    token_next(trace_node.get_tab());
    if ( token_is_not('#', trace_node) ) {
        context = context_tokens.restore();
        return 0;
    }

    token_next(trace_node.get_tab());
    if ( "endif" != c_token_get().text ) {
        context = context_tokens.restore();
        return 0;
    }

    return 1;
}
/*----------------------------------------------------------------------------*/
