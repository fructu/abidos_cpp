/*------------------------------------------------------------------------------

    Proyecto            : abidos
    Codigo              : trace.cpp
    Descripcion         :
    Version             : 0.1
    Autor               : F. Manuel Hevia Garcia
    Fecha creacion      : 17/05/2011
    Fecha modificacion  :

    Observaciones :


------------------------------------------------------------------------------*/
#include <stdio.h>
#include "trace.h"
#include "options.h"
/*----------------------------------------------------------------------------*/
int c_trace_node::order_static = 0;
/*----------------------------------------------------------------------------*/
string c_trace_node::get_tab(void)
{
//  tab = tab + TAB;
    string tab = "";
    int i = 0;

    for (i = 0; i < level; ++i) {
        tab = tab + TAB;
    }

    return tab;
};
/*----------------------------------------------------------------------------*/
void c_trace_node::set(string s)
{
    ++c_trace_node::order_static;
    order = c_trace_node::order_static;

    ++level;

    char str_level[100] = {};
    sprintf(str_level, "_%d_", order);

    function_name_previous = function_name;
    function_previous_token_text_is = function_token_text_is;
    function_previous_token_text_is_not = function_token_text_is_not;

    function_name = str_level + s;

    if (1 != options.verbose_flag) {
        return;
    }

    string tab = get_tab();
//  printf("%s[%2d]%s\n", tab.c_str(), level, s.c_str());
    printf("%s[%2d]%s\n", tab.c_str(), level, function_name.c_str());
}
/*----------------------------------------------------------------------------*/
void c_trace_graph::add(c_trace_node & node, string s)
{
    node.set(s);
    node.position = vector_nodes.size();
    vector_nodes.push_back(node);
}
/*----------------------------------------------------------------------------*/
void put_scaped(string & s)
{
    if ( "<" == s ) {
//        s = " LT";
        s = " \\<";
    } else if (">" == s) {
//        s = " GT";
        s = " \\>";
    } else if ("{" == s) {
//        s = " OPEN_BRACKET";
        s = " \\{ ";
    } else if ("}" == s) {
//        s = " CLOSE_BRACKET";
        s = " \\} ";
    } else if ("\\" == s) {
        s = " BACKSLASH";
    } else if ( "{" == s || "}" == s || "[" == s || "]" == s || "/" == s || "," == s|| ":" == s) {
        s = "\\" + s;
    } else {//s can be a string
        char str_in[1024] = {'\0'};
        char str_out[1024] = {'\0'};
        int i_in = 0;
        int i_out = 0;
        char c=0;

        sprintf(str_in,"%s",s.c_str() );
        while ( '\0' != str_in[i_in]) {
            c=str_in[i_in];
            if ( '{' == c || '}' == c || '[' == c || ']' == c) {
                /*
                    str_out[i_out]='\\';
                    ++i_out;
                    str_out[i_out]=str_in[i_in];
                */
                str_out[i_out]='_';
            } else if ('/' == c) {
                str_out[i_out]='_';
            } else if ('"' == c) {
                str_out[i_out]='_';
            } else {
                str_out[i_out]=str_in[i_in];
            }
            ++i_in;
            ++i_out;
        }
        str_out[i_out]='\0';
        s=str_out;
    }

}
/*----------------------------------------------------------------------------*/
void c_trace_graph::token_is_add(string s, string s_id, unsigned position)
{
    unsigned len = vector_nodes.size();

    if ( 0 == len) {
        printf(" error void c_trace_graph::token_is_add(string s) ( 0 == len)\n");
        return;
    }

    unsigned last = position;

    put_scaped(s);

    s = s + " is " + s_id;

    if ( 0 == vector_nodes[last].function_token_text_is.size() ) {
        vector_nodes[last].function_token_text_is = s;
    } else {
        vector_nodes[last].function_token_text_is = vector_nodes[last].function_token_text_is + s;
    }
}
/*----------------------------------------------------------------------------*/
void c_trace_graph::token_is_not_add(string t, string s, unsigned position)
{
    unsigned len = vector_nodes.size();


    if ( 0 == len) {
        printf(" error void c_trace_graph::token_is_not_add(string s) ( 0 == len)\n");
        return;
    }

    unsigned last = position;

    put_scaped(s);
    put_scaped(t);

    if ( 0 == vector_nodes[last].function_token_text_is_not.size() ) {
        vector_nodes[last].function_token_text_is_not = t + "\\ is_not " + s;
    } else {
        vector_nodes[last].function_token_text_is_not = vector_nodes[last].function_token_text_is_not + "\\ "+ s;
    }
}
c_trace_graph trace_graph;
/*----------------------------------------------------------------------------*/
void c_generator_trace::functions(t_vector_trace_nodes & v)
{
    /*
      A [
        URL="A[test//t001.cpp:1];A[test//t001.cpp:1];",
        label="{A|}"
      ]
    */
    unsigned i = 0;
    unsigned len = v.size();
    int shape_record = 0;

    fprintf(f_out, "  ROOT[label=\"ROOT\"]");

    for ( i = 1; i < len; ++i) {
        if ( 0 == v[i].function_token_text_is.size() &&
                0 == v[i].function_token_text_is_not.size()
           ) {
            shape_record = 0;
            fprintf(f_out, "  %s[label=\"%s"
                    ,v[i].function_name.c_str()
                    ,v[i].function_name.c_str()
                   );
        } else {
            shape_record = 1;
            fprintf(f_out, "  %s[label=\"{%s"
                    ,v[i].function_name.c_str()
                    ,v[i].function_name.c_str()
                   );
        }

        if ( 0 < v[i].function_token_text_is.size() ) {
            fprintf(f_out, "|[%s]\\l"
                    ,v[i].function_token_text_is.c_str()
                   );
        }

        if ( 0 < v[i].function_token_text_is_not.size() ) {
            fprintf(f_out, "|[%s]"
                    ,v[i].function_token_text_is_not.c_str()
                   );
        }
        /*
        node [shape=box, color=blue]
        */
        if ( 1 == shape_record ) {
            if ( 0 == v[i].function_token_text_is.size() ) {
                fprintf(f_out, "}\", shape=\"record\", color=black, style=dotted]\n");
            } else {
                fprintf(f_out, "}\", shape=\"record\", color=green]\n");
            }
        } else {
            fprintf(f_out, "\"]\n");
        }
    }
}
/*----------------------------------------------------------------------------*/
void c_generator_trace::calls(t_vector_trace_nodes & v)
{
    unsigned i = 0;
    unsigned len = v.size();

    if ( 0 < len ) {
        fprintf(f_out, "  ROOT->");
        fprintf(f_out, "%s;\n",v[0].function_name.c_str() );
    }

    for ( i = 1; i < len; ++i) {
        fprintf(f_out, "  %s->",v[i].function_name_previous.c_str() );
        fprintf(f_out, "%s;\n",v[i].function_name.c_str() );
    }
}
/*----------------------------------------------------------------------------*/
/*
  echo "digraph G {Hello->World}" | dot -Tpng >hello.png
*/
void c_generator_trace::run(char *p_file_out)
{
    printf("## void c_generator_trace::run(char * p_file_out [%s])\n",
           p_file_out);

    f_out = NULL;

    f_out = fopen(p_file_out, "w");
    if (NULL == f_out) {
        printf("  c_generator_trace::run() cant fopen[%s]()\n", p_file_out);
        return;
    }
    fprintf(f_out, "/*\n");
    fprintf(f_out, " cat %s | dot -Tpng > %s.png\n",p_file_out,p_file_out);
    fprintf(f_out, "*/\n");
    fprintf(f_out, "digraph G {\n");
    functions(trace_graph.vector_nodes);
    calls(trace_graph.vector_nodes);
    fprintf(f_out, "}\n");

    if (NULL != f_out) {
        fclose(f_out);
        f_out = NULL;
    }
}
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
void f1(c_trace_node trace_node)
{
    trace_node.set("f1");
}

void f2(c_trace_node trace_node)
{
    trace_node.set("f2");
}

void f3(c_trace_node trace_node)
{
    trace_node.set("f3");
}

void c_trace_graph_test(void)
{
    c_trace_node trace_node;
    trace_node.set("test");
    f1(trace_node);
    f2(trace_node);
    f3(trace_node);
}
/*----------------------------------------------------------------------------*/


