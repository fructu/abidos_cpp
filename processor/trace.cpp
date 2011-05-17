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

c_trace_graph trace;
/*----------------------------------------------------------------------------*/
/*
class c_generator_trace
{
private:
  FILE * f_out;
  void functions(t_vector_trace_nodes & vector);
  void calls(t_vector_trace_nodes & vector);
public:
  void run(char *p_file_out);
};
*/
void c_generator_trace::functions(t_vector_trace_nodes & vector)
{
}
/*----------------------------------------------------------------------------*/
void c_generator_trace::calls(t_vector_trace_nodes & vector)
{
  unsigned i = 0;
  unsigned len = vector.size();

  if ( 0 < len )
    {
      fprintf(f_out, "  ROOT->");
      fprintf(f_out, "%s;\n",vector[0].function_name.c_str() );
    }

  for ( i = 1; i < len; ++i)
    {
      fprintf(f_out, "  %s->",vector[0].function_name_previous.c_str() );
      fprintf(f_out, "%s;",vector[0].function_name.c_str() );
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
  if (NULL == f_out)
    {
      printf("  c_generator_trace::run() cant fopen()\n");
      return;
    }

  fprintf(f_out, "digraph G {");
  calls(trace.vector);
  fprintf(f_out, "}");

  if (NULL != f_out)
    {
      fclose(f_out);
      f_out = NULL;
    }
}
/*----------------------------------------------------------------------------*/

