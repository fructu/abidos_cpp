/*
 decl_specifier: ... | FRIEND | TYPEDEF ;
 
*/
class A
{
};

typedef A t_A;

class B
{
  friend A a;  
};

