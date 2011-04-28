/*
 decl_specifier: ... | FRIEND | TYPEDEF ;
 
*/
class A
{
};

typedef A t_A;

class B
{
  //context.class_specifier_status 
  //## todo i need new state
  friend class A;
};

