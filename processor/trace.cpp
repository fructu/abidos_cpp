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
#include "trace.h"
/*----------------------------------------------------------------------------*/
int c_trace_node::order_static = 0;
/*----------------------------------------------------------------------------*/
void c_trace_node::set(string s)
{
  ++c_trace_node::order_static;
  order = c_trace_node::order_static;
  ++level;
  function_name_previous = function_name;
  function_name = s;
}
/*----------------------------------------------------------------------------*/
void c_trace_graph::add(c_trace_node & node, string s)
{
  node.set(s);
  vector.push_back(node);
}
/*----------------------------------------------------------------------------*/
