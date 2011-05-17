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

  for (i = 0; i < level; ++i)
    {
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
  sprintf(str_level, "_%d_", level);

  function_name_previous = function_name;
  function_name = str_level + s;

  if (1 != options.verbose_flag)
    {
      return;
    }

  string tab = get_tab();
  printf("%s[%2d]%s\n", tab.c_str(), level, s.c_str());
}
/*----------------------------------------------------------------------------*/
void c_trace_graph::add(c_trace_node & node, string s)
{
  node.set(s);
  vector.push_back(node);
}

c_trace_graph trace_graph;
/*----------------------------------------------------------------------------*/
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
      fprintf(f_out, "  %s->",vector[i].function_name_previous.c_str() );
      fprintf(f_out, "%s;\n",vector[i].function_name.c_str() );
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
  fprintf(f_out, "/*\n");
  fprintf(f_out, " cat %s | dot -Tpng > %s.png\n",p_file_out,p_file_out);
  fprintf(f_out, "*/\n");
  fprintf(f_out, "digraph G {\n");
  calls(trace_graph.vector);
  fprintf(f_out, "}\n");

  if (NULL != f_out)
    {
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


