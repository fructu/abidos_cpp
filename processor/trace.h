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
};

extern c_trace_graph trace;
#endif
