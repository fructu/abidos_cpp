/*------------------------------------------------------------------------------

    Proyecto            : abidos
    Codigo              : trace.h
    Descripcion         :
    Version             : 0.1
    Autor               : F. Manuel Hevia Garcia
    Fecha creacion      : 17/05/2011
    Fecha modificacion  :

    Observaciones :


------------------------------------------------------------------------------*/
#ifndef trace_h
#define trace_h

#include <string>
#include <vector>

using namespace std;

#define TAB " "

class c_trace_node
{
private:
    static int order_static;
    int order;
    int level;

    string function_name;
    string function_name_previous;

    friend class c_generator_trace;
public:
    unsigned position;

    string function_token_text_is;
    string function_token_text_is_not;

    string function_previous_token_text_is;
    string function_previous_token_text_is_not;

    c_trace_node() {
        order = 0;
        level = 0;

        position = 0;

        function_name = "";
        function_name_previous = "";

        string function_token_text_is = "";
        string function_token_text_is_not = "";

        string function_previous_token_text_is = "";
        string function_previous_token_text_is_not = "";
    }

    string get_tab(void);

    void set(string s);
};

typedef vector<c_trace_node> t_vector_trace_nodes;

class c_generator_trace
{
private:
    FILE * f_out;
    void functions(t_vector_trace_nodes & vector);
    void calls(t_vector_trace_nodes & vector);
public:
    void run(char *p_file_out);
};

class c_trace_graph
{
private:
    t_vector_trace_nodes vector;
    friend class c_generator_trace;
public:
    void add(c_trace_node & node, string s);
    void token_is_add(string s, unsigned position);
    void token_is_not_add(string t, string s, unsigned position);
};

void c_trace_graph_test(void);

extern c_trace_graph trace_graph;
#endif

