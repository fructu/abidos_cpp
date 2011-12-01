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
#include "lex_yacc.h"

#include "preprocessor_ts.h"
#include "loader.h"
#include <sys/param.h>
/*----------------------------------------------------------------------------*/
void c_parser_descent::push_file(const char * f)
{
    char path[MAXPATHLEN];
    char path2[MAXPATHLEN];

    getcwd(path, MAXPATHLEN);
    getcwd(path2, MAXPATHLEN);
    printf("####pwd -> %s\n", path);

    c_cell file_actual;
    c_cell file_lex;

    sprintf(path2,"%s/",path);
    file_actual.fill(path2, (char *) "\"");
    printf("####file_actual   -> %s\n", file_actual.full());

    printf("####f -> %s\n", f);
    printf("####lex_file_name -> %s\n", lex_file_name);

    file_lex.fill(lex_file_name, (char *) "\"");
    printf("####file_lex 1 -> %s\n", file_lex.full());
    if (lex_file_name[0] != '/') {
        file_actual.path_resolve(file_lex);
    }
    printf("####file_lex 2 -> %s\n", file_lex.full());
//    file_actual.fill(lex_file_name, (char *) "\"");

    char file_without_commillas[ID_MAX_LEN]={'\0'};

    sprintf(file_without_commillas,"%s",f);
    str_drop_char(file_without_commillas, '\"');

    c_cell file_included;
    file_included.fill(file_without_commillas, (char *) "\"");

//    file_actual.path_resolve(file_included);
//    file_actual.path_resolve(file_included);

    printf("####file_lex -> %s\n", file_lex.full());
    printf("####file_included a -> %s\n", file_included.full());
    file_lex.path_resolve(file_included);
    printf("####file_included b -> %s\n", file_included.full());

    if (1 != lex_file_push( file_included.full() ) ) {
//      if (1 != lex_file_push( file_without_commillas ) ) {
        printf("\nERROR:c_parser_descent::push_file[%s]\n"
               , file_included.full());
        exit(-1);
//      }
    }
}
/*----------------------------------------------------------------------------*/
void c_parser_descent::push_file_loader(const char * f)
{
    char file_without_commillas[ID_MAX_LEN]={'\0'};

    sprintf(file_without_commillas,"%s",f);
    str_drop_char(file_without_commillas, '\"');

    if ( 1 != loader.include_file_get(file_without_commillas) ) {
        printf("\nERROR:c_parser_descent::push_file_loader[%s]\n"
               , file_without_commillas );
        exit(-1);
    }

    if (1 != lex_file_push( file_without_commillas ) ) {
        printf("\nERROR:c_parser_descent::push_file_loader[%s]\n"
               , file_without_commillas );
        exit(-1);
    }
}
/*----------------------------------------------------------------------------*/
void c_parser_descent::push_sharp_file(const char * f)
{
    if (strcmp("vector", f) == 0) {
        if ( 1 == options.loader_flag) {
            push_file_loader("../test_includes/std.h");
        } else {
            push_file("../test_includes/std.h");
        }
    } else if (strcmp("map", f) == 0) {
        if ( 1 == options.loader_flag) {
            push_file_loader("../test_includes/std.h");
        } else {
            push_file("../test_includes/std.h");
        }
    } else if (strcmp("string", f) == 0) {
        if ( 1 == options.loader_flag) {
            push_file_loader("../test_includes/std.h");
        } else {
            push_file("../test_includes/std.h");
        }
    } else if (strcmp("stdio.h", f) == 0) {
        if ( 1 == options.loader_flag) {
            push_file_loader("../test_includes/stdio.h");
        } else {
            push_file("../test_includes/stdio.h");
        }
    } else if (strcmp("inttypes.h", f) == 0) {
        if ( 1 == options.loader_flag) {
            push_file_loader("../test_includes/inttypes.h");
        } else {
            push_file("../test_includes/inttypes.h");
        }
    }
}
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

    if (1 == preprocessor_other_dummy(trace_node)) {
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

        char f[1024];
        get_string_between_sharps(f);
        push_sharp_file(f);
        return 1;
    }
    if ( 1 == options.loader_flag) {
        push_file_loader(c_token_get().text.c_str());
    } else {
        push_file(c_token_get().text.c_str());
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

    /*
      lex must eat the rest of the line
      #define D 1
      ## todo: for the moment 1 is not parsing...
    */
    skip_until_eol_preprocessor();

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

    context.prefix_sharp = 1; //in ts should have the prefix '#'

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
/*
  preprocessor_other_dummy: #<ANYTHING>
    ;
*/
int c_parser_descent::preprocessor_other_dummy(c_trace_node trace_node)
{
    trace_graph.add(trace_node, "preprocessor_other_dummy");

    c_context_tokens context_tokens(context);

    token_next(trace_node.get_tab());
    if ( token_is_not('#', trace_node) ) {
        context = context_tokens.restore();
        return 0;
    }

    skip_until_eol_preprocessor();

    return 1;
}
/*----------------------------------------------------------------------------*/
