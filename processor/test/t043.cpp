/*
  testing int c_trace_node::order_static = 0;
  appears A ?
*/

class c_trace_node
{
//private:
    static int order_static;
};

int c_trace_node::order_static = 0;

class A
{
};

