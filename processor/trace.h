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
    
  public:
    void set(string s);
};

typedef vector<c_trace_node> t_vector_trace_nodes;

class c_trace_graph
{
  private:
    t_vector_trace_nodes vector;
  public:
    void add(c_trace_node & node, string s);
};
#endif
