/*------------------------------------------------------------------------------

    Proyecto            : show_includes
    Codigo              : parser_descent.cpp
    Descripcion         :
    Version             : 0.1
    Autor               : F. Manuel Hevia Garcia
    Fecha creacion      : 03/03/2011
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
#include "semantic.h"
#include "loader.h"
/*----------------------------------------------------------------------------*/
/*
  class_name = "A::B"
  text = "B"
  B is tail
*/
int chain_is_tail(string class_name_declaration, char * text)
{
    char class_name[100];
    unsigned len_class_name = class_name_declaration.size();
    unsigned len_text  = strlen(text);

    if ( 0 == len_text) {
        return 0;
    }

    if ( 0 == len_class_name) {
        return 0;
    }

    if (len_text > len_class_name) {
        return 0;
    }

    sprintf(class_name,"%s", class_name_declaration.c_str());

    unsigned i = len_text - 1;
    unsigned i_class_name = len_class_name - 1;

    while (1) {
        if ( ':' == class_name[i_class_name] ) {
            return 1;
        }

        if ( text[i] != class_name[i_class_name] ) {
            return 0;
        }

        if ( 0 == i ) {
            return 1;
        }

        --i;
        --len_class_name;
    }

    return 1;
}
/*----------------------------------------------------------------------------*/
c_context_tokens::c_context_tokens(c_context context_param)
{
    context = context_param;
}

/*----------------------------------------------------------------------------*/
void
c_context_tokens::save(c_context context_param)
{
    context = context_param;
}

/*----------------------------------------------------------------------------*/
c_context c_context_tokens::restore(void)
{
    /*
     * if( 0 == context.i_token) { context.just_reloaded = 1; }
     */
    return context;
}

/*----------------------------------------------------------------------------*/
void
c_context_tokens::restore_but_not_i_token(c_context & context_param)
{
    /*
     * if( 0 == context.i_token) { context.just_reloaded = 1; }
     */
    // return context;

    context_param.restore_but_not_i_token(context_param);
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
c_parser_descent::c_parser_descent()
{
    context.just_reloaded = 1;
    class_no_name_number = 0;
    class_no_name = "";
}
/*----------------------------------------------------------------------------*/
string c_parser_descent::get_class_no_name(void)
{
    char str[100] = {'\0'};

    sprintf(str,"%s%d",NO_CLASS_NAME,class_no_name_number);
    class_no_name = str;

    return class_no_name;
}
/*----------------------------------------------------------------------------*/

c_parser_descent::~c_parser_descent()
{
}

/*----------------------------------------------------------------------------*/
void yytname_print(void)
{
    int i = 0;
    while (yytokens[i] != 0) {
        printf(" i=[%d] -> [%s]\n", i, yytokens[i]);
        i++;
    }
};

/*----------------------------------------------------------------------------*/
void c_parser_descent::tokens_vector_print(void)
{
    // t_tokens::iterator i_token = tokens_vector.begin();

    unsigned n = 0;

    printf("tokens_vector_print\n");
    printf("{\n");

    while (n < tokens_vector.size()) {
        c_token token;
        token = tokens_vector[n];

        printf(" [%3d] -> yytokens[%s] token.text[%s]\n", token.id,
               yytokens[token.id], token.text.c_str());

        ++n;
    }

    printf("}\n");
}

/*----------------------------------------------------------------------------*/
void c_parser_descent::tokens_vector_print_from_actual(void)
{
    printf("##### c_parser_descent::tokens_vector_print_from_actual context.i_token[%d]\n",context.i_token);
    return;
    t_tokens::iterator i_token = tokens_vector.begin() + context.i_token;

    printf("tokens_vector_print_from_actual\n");
    printf("{\n");

    while (i_token != tokens_vector.end()) {
        c_token token;
        token = (*i_token);

        printf(" [%3d] -> yytokens[%s] token.text[%s]\n", token.id,
               yytokens[token.id], token.text.c_str());

        ++i_token;
    }

    printf("}\n");
    /*
      unsigned i = context.i_token;
      printf("{\n");
      while (i < tokens_vector.size())
        {
          c_token token;
          token = tokens_vector[i];

          printf(" [%3d] -> yytokens[%s] token.text[%s]\n", token.id,
                 yytokens[token.id], token.text.c_str());

          ++i;
        }
      printf("}\n");
    */
}

/*----------------------------------------------------------------------------*/
void c_parser_descent::token_print(void)
{
    if (tokens_vector.empty()) {
        printf
        ("error c_parser_descent::token_print() -> tokens_vector.empty() \n");
        exit(-1);
    }

    if (!((0 <= context.i_token)
            && (context.i_token < tokens_vector.size()))) {
        printf
        ("error c_parser_descent::token_print() -> (context.i_token out of vector) \n");
        exit(-1);
    }

    printf(" file[%s] line[%d] yy_actual=[%3d] "
           , lex_file_name , yylineno, token_get());
    printf("-> yytokens[%s] ", yytokens[token_get()]);
    printf(" token.text[%s]\n",
           tokens_vector[context.i_token].text.c_str());
}

/*----------------------------------------------------------------------------*/
void c_parser_descent::tokens_vector_reload(void)
{
    context.clear();
}

/*----------------------------------------------------------------------------*/
void c_parser_descent::tokens_vector_clear(void)
{
    tokens_vector.clear();

    if (!tokens_vector.empty()) {
        printf
        ("########################## tokens_vector_clear COMOR  !tokens_vector.empty() !!! \n");
    }

    context.clear();
}

/*----------------------------------------------------------------------------*/
int c_parser_descent::token_get(void)
{
    if (tokens_vector.empty()) {
        printf
        ("error c_parser_descent::token_get() -> tokens_vector.empty() \n");
        exit(-1);
    }

    if (!((0 <= context.i_token)
            && (context.i_token < tokens_vector.size()))) {
        printf
        ("error c_parser_descent::token_get() -> (context.i_token out of vector) \n");
        // exit( -1 );
        context.clear();
    }

    return tokens_vector[context.i_token].id;
}
/*----------------------------------------------------------------------------*/
c_token c_parser_descent::c_token_get(void)
{
    if (tokens_vector.empty()) {
        printf
        ("error c_parser_descent::c_token_get() -> tokens_vector.empty() \n");
        exit(-1);
    }

    if (!((0 <= context.i_token)
            && (context.i_token < tokens_vector.size()))) {
        printf
        ("error c_parser_descent::c_token_get() -> (context.i_token out of vector) \n");
        // exit( -1 );
        context.clear();
    }

    return tokens_vector[context.i_token];
}
/*----------------------------------------------------------------------------*/
int c_parser_descent::token_is(int id , c_trace_node trace_node)
{
    int result = 0;

    if ( id == token_get()) {
//        trace_graph.token_is_add(c_token_get().text, trace_node.position);
        string s_id = yytokens[id];
        string s = c_token_get().text;
        trace_graph.token_is_add(s, s_id, trace_node.position);
        result = 1;
    } else {
//      char s_id[10] = {};
//      sprintf(s_id,"[%d]",id);
        string s = yytokens_short[id];
//      s += s_id;

        trace_graph.token_is_not_add( c_token_get().text, s, trace_node.position );
    }

    return result;
}
/*----------------------------------------------------------------------------*/
int c_parser_descent::token_is_not(int id , c_trace_node trace_node)
{
    int result = 0;

    if ( id == token_get()) {
        string s_id = yytokens[id];
        string s = c_token_get().text;
        trace_graph.token_is_add(s,s_id, trace_node.position);
    } else {
//      char s_id[10] = {};
//      sprintf(s_id,"[%d]",id);
        string s = yytokens_short[id];
//      s += s_id;

        trace_graph.token_is_not_add( c_token_get().text, s, trace_node.position );
        result = 1;
    }

    return result;
}
/*----------------------------------------------------------------------------*/
int c_parser_descent::token_is_one(const int id[], c_trace_node trace_node)
{
    c_context_tokens context_tokens(context);

    int i = 0;
    while ( -1 != id[i]) {
        if ( token_is(id[i], trace_node) ) {
            return id[i];
        }
        ++i;
    }

    return 0;
}
/*----------------------------------------------------------------------------*/
int c_parser_descent::is_eof(c_trace_node trace_node)
{
    trace_graph.add(trace_node, "is_eof");

    c_context_tokens context_tokens(context);

    token_next(trace_node.get_tab());

    if ( token_is(0, trace_node) ) {
        if (0 == lex_stack_size() ) {
            return 1;
        } else {
            lex_file_pop();
            //  context = context_tokens.restore();
            return 0;
        }
        return 1;
    }
    // ##todo | template_id

    context = context_tokens.restore();
    return 0;
}
/*----------------------------------------------------------------------------*/
int c_parser_descent::preanalisys(int id, c_trace_node trace_node)
{
    trace_graph.add(trace_node, "preanalisys");

    c_context_tokens context_tokens(context);

    token_next(trace_node.get_tab());

    if ( token_is(id, trace_node) ) {
        context = context_tokens.restore();
        return 1;
    }

    context = context_tokens.restore();
    return 0;
}
/*----------------------------------------------------------------------------*/
int c_parser_descent::preanalisys_has_one(const int id[], c_trace_node trace_node)
{
    trace_graph.add(trace_node, "preanalisys");

    c_context_tokens context_tokens(context);

    token_next(trace_node.get_tab());

    int i = 0;
    while ( -1 != id[i]) {
        if ( token_is(id[i], trace_node) ) {
            context = context_tokens.restore();
            return 1;
        }
        ++i;
    }

    context = context_tokens.restore();
    return 0;
}
/*----------------------------------------------------------------------------*/
/*
class A
{
  class A1
  {
  };
};

in ts:
 [A::A1] is class_name

## improve to using namespace
*/
void c_parser_descent::colon_colon_chain_process(c_token & token)
{
    if ( COLONCOLON == token.id) {
        if ( 0 != colon_colon_chain.size() ) {
            colon_colon_chain = colon_colon_chain + "::";
        }

        return;
    }

    if (IDENTIFIER == token.id) {
        colon_colon_chain = colon_colon_chain + token.text;
        c_symbol * p_symbol = ts.search_symbol(colon_colon_chain);

        if ( 0 != p_symbol ) {
            if ( 0 != p_symbol->type) {
                if ( 1 == p_symbol->is_template ) {
                    token.text = colon_colon_chain;
                    token.id = TEMPLATE_NAME;
                } else {
                    token.id = p_symbol->type;
                }
            }

            return;
        }
    }

    colon_colon_chain = "";
}

/*----------------------------------------------------------------------------*/
/*
 * void c_parser_descent::token_previous(void) { if( tokens_vector.empty()
 * ) { context.i_token = tokens_vector.begin(); return; }
 *
 * if( context.i_token > tokens_vector.begin() ) { --context.i_token;
 * return; } }
 */
/*----------------------------------------------------------------------------*/
void c_parser_descent::token_next(string tab)
{
    int t = 0;
    int get_from_lex = 0;

//    printf("%s## token_next file[%s]", tab.c_str(), lex_file_name);

    if (!((0 <= context.i_token)
            && (context.i_token < tokens_vector.size()))) {
        /*##
                printf
                (" c_parser_descent::token_next() -> (context.i_token out of vector)");
        */
        // context.clear();
        // we don't want loose other information of context
        context.i_token = 0;
        context.just_reloaded = 1;
    }

    if (tokens_vector.empty()) {
        get_from_lex = 1;
    } else {
        if (context.i_token < tokens_vector.size()) {
            if (1 == context.just_reloaded) {
                context.just_reloaded = 0;
//                printf("%s## just_reloaded == 1\n", tab.c_str());
                return;
            }

            if (context.i_token < (tokens_vector.size() - 1)) {
                ++context.i_token;
//##
//                token_print();
                return;
            }
        }

        get_from_lex = 1;
    }

    if (1 == get_from_lex) {
        context.just_reloaded = 0;

        t = yylex();
        c_token token(t, yytext);

        colon_colon_chain_process(token);
        /*
         * i tried put this part in lex but did not compile maybe for the
         * link C mode of lexical module
         */
        if (IDENTIFIER == t) {
            check_identifier(tab, token);
        }

        tokens_vector.push_back(token);

        context.i_token = (tokens_vector.size() - 1);
//##
//        token_print();
        return;
    }

    printf("error c_parser_descent::token_next(void)\n");
    exit(-1);
}
/*----------------------------------------------------------------------------*/
/*
  we look if identifier is other thing:
    - a class
    - template_name
    - namespace
*/
void c_parser_descent::check_identifier(string tab, c_token &token)
{
    c_symbol *p_symbol = ts.search_symbol(yytext);

    if (p_symbol) {
        if (p_symbol->type != 0) {
            // return symbol.type;
            /*            printf("%s## next_token found symbol [%s]",
                               tab.c_str(), yytext);
            */
            if ( 1 == p_symbol->is_template ) {
                token.id = TEMPLATE_NAME;
            } else {
                token.id = p_symbol->type;
            }
        }
        return;
    }

    /*
      t031 destructors
      A::B::~B()
    */
    if ( 1 == chain_is_tail(context.class_name_declaration, yytext) ) {
        c_symbol *p_symbol = ts.search_symbol(context.class_name_declaration);
        if (p_symbol) {
            if (p_symbol->type != 0) {
                // return symbol.type;

                if ( 1 == p_symbol->is_template ) {
                    token.id = TEMPLATE_NAME;
                } else {
                    token.id = p_symbol->type;
                }
            }
        }
        return;
    }

    //check if is template type ej template <class T> --> T
    if ( 2 == context.i_am_in_template_declaration ) {
        if ( context.map_template_parameter.count(token.text) > 0) {
            token.id = TEMPLATE_TYPE;
        }
        return;
    }

    // declarations of members functions outside of his class
    // but inside of the namespace
    if ( 0 == context.class_name_declaration.size() ) {
        if ( 0 != context.namespace_name_declaration.size() ) {
            string s = context.namespace_name_declaration + "::" + yytext;
            c_symbol *p_symbol = ts.search_symbol(s.c_str());
            if (p_symbol) {
                token.text = p_symbol->text;
//                token.id = p_symbol->type;

                if ( 1 == p_symbol->is_template ) {
                    token.id = TEMPLATE_NAME;
                } else {
                    token.id = p_symbol->type;
                }
            }
            return;
        }
    }

    // chek the using namespaces
    if ( semantic.vector_using_namespace.size() > 0 ) {
        int unsigned i = 0;

        for (i = 0;  i < semantic.vector_using_namespace.size(); ++i ) {
            string s = semantic.vector_using_namespace[i] + "::" + yytext;
            c_symbol *p_symbol = ts.search_symbol(s.c_str());
            if (p_symbol) {
                token.text = p_symbol->text;
//                token.id = p_symbol->type;

                if ( 1 == p_symbol->is_template ) {
                    token.id = TEMPLATE_NAME;
                } else {
                    token.id = p_symbol->type;
                }
                return;
            }
        }
    }

    if ( 0 != context.class_name_declaration.size() ) {
        if ( 0 != context.namespace_name_declaration.size() ) {
            string s = context.namespace_name_declaration + "::" + yytext;
            c_symbol *p_symbol = ts.search_symbol(s.c_str());
            if (p_symbol) {
//                token.text = p_symbol->text;
                token.text = s;
//                token.id = p_symbol->type;

                if ( 1 == p_symbol->is_template ) {
                    token.id = TEMPLATE_NAME;
                } else {
                    token.id = p_symbol->type;
                }
            }
        }
        return;
    }

    //preprocessor hack
    if ( 1 == context.prefix_sharp ) {
        string s = "#";
        s = s + yytext;
        c_symbol *p_symbol = ts.search_symbol(s.c_str());
        if (p_symbol) {
            token.text = p_symbol->text;
//                token.text = s;
//                token.id = p_symbol->type;
            token.id = PREPROCESSOR_DEFINITION;
        }
    }
}
/*----------------------------------------------------------------------------*/
void extract_file_from_path(char *file, char *path)
{
    unsigned n = strlen(path);

    while (n) {
        if ('/' == path[n]) {
            ++n;
            break;
        }
        --n;
    }

    unsigned n2 = 0;
    while ('\0' != path[n]) {
        file[n2] = path[n];
        ++n2;
        ++n;
    }
}

/*----------------------------------------------------------------------------*/
void c_parser_descent::yyparse(char *file_name)
{
    if (1 != lex_file_init(file_name)) {
        printf("\nERROR: yyparse did not can open [%s]\n", file_name);
        return;
    }

    ts.set();
    translation_unit();

    if (1 == options.test_all_tokens_consumed_flag) {
        if (token_get() == 0) {
            printf("##ALL_TOKENS_CONSUMED\n");
        } else {
            printf
            ("\n##---------------- rest of tokens has not been consumed -------------------------------\n");
            printf("##{\n");
            while (token_get() != 0) {
                token_next("");
            }
            printf("##}\n");
        }
    }

    if (1 == options.ts_show_flag) {
        ts.print();
    }

    char str_temp[100] = { '\0' };
    char file_gv[100];
    extract_file_from_path(str_temp, file_name);

    /*## todo
      this should pass in main parameter
    */
    sprintf(file_gv, ".abidos/out_%s.gv", str_temp);
    c_generator_class_diagram generator;

    generator.run(file_gv);

    if (1 == options.test_original_flag) {
        char file_original[100];
        sprintf(file_original, ".abidos/out_%s", str_temp);
        c_generator_original generator_original;
        generator_original.run(file_original);
    }

    if (1 == options.verbose_flag) {
        char file_original[100];
        sprintf(file_original, ".abidos/trace_%s.gv", str_temp);
        c_generator_trace generator_trace_graph;
        generator_trace_graph.run(file_original);
    }

    ts.unset();

    lex_file_end();
    yylex_destroy();
}
/*----------------------------------------------------------------------------*/
void c_parser_descent::yyparse_loader(char *file_loader)
{
    ts.set();
    char file_name[1024] = {0};

    loader.process_file(file_loader);
    loader.begin();

    if ( 1 == loader.file_get(file_name) ) {
        if (1 != lex_file_init(file_name)) {
            printf("\nERROR: yyparse_loader did not can open [%s]\n", file_name);
            return;
        }
        translation_unit();
        loader.next();
    }


    while ( 1 == loader.file_get(file_name) ) {
        /*##
          if (1 != lex_file_init(file_name)) {
              printf("\nERROR: yyparse_loader did not can open [%s]\n", file_name);
              return;
          }
          */

        if (1 != lex_file_push( file_name ) ) {
            printf("\nERROR: yyparse_loader did not can open [%s]\n", file_name);
            return;
        }

        translation_unit();
        loader.next();
    }

    if (1 == options.test_all_tokens_consumed_flag) {
        if (token_get() == 0) {
            printf("##ALL_TOKENS_CONSUMED\n");
        } else {
            printf
            ("\n##---------------- rest of tokens has not been consumed -------------------------------\n");
            printf("##{\n");
            while (token_get() != 0) {
                token_next("");
            }
            printf("##}\n");
        }
    }


    if (1 == options.ts_show_flag) {
        ts.print();
    }

    char str_temp[100] = { '\0' };
    char file_gv[100];
    extract_file_from_path(str_temp, file_loader);

    /*## todo
      this should pass in main parameter
    */
    sprintf(file_gv, ".abidos/out_%s.gv", str_temp);
    c_generator_class_diagram generator;

    generator.run(file_gv);

    if (1 == options.test_original_flag) {
        char file_original[100];
        sprintf(file_original, ".abidos/out_%s", str_temp);
        c_generator_original generator_original;
        generator_original.run(file_original);
    }

    if (1 == options.verbose_flag) {
        char file_original[100];
        sprintf(file_original, ".abidos/trace_%s.gv", str_temp);
        c_generator_trace generator_trace_graph;
        generator_trace_graph.run(file_original);
    }

    ts.unset();
    lex_file_end();
    yylex_destroy();
}
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
int c_parser_descent::test_01(void)
{
    char file_name[] = "test/t1.cpp";

    printf("c_parser_descent::test_01()\n");
    printf("{\n");

    if (!(yyin = fopen(file_name, "r"))) {
        printf("\nERROR: yyparse did not can open [%s]\n", file_name);
        return 0;
    }
    // yytname_print();
    tokens_vector_reload();
    token_next("");
    while (token_get() != 0) {
        token_next("");
    }

    printf("reload\n");
    tokens_vector_reload();
    token_next("");
    while (token_get() != 0) {
        token_next("");
    }

    tokens_vector_print();
    yylex_destroy();

    printf("}\n");

    return 0;
}

/*----------------------------------------------------------------------------*/
