/*
  ptr_operator
  note: && is the and operator -> int && i; does not compile
*/
class A
{
  int * p_i;
  int & r_i;
  int ** pp_i;

  void * fa(void);
  void * fa(int **&*&** p_i);
  void & fa(int &*& p_i); 
};

void & A::fa(int **&*&** p_i)
{
}

void * A::fa(int **&*&** p_i)
{
}

A & f1(int i)
{
}


A * f2(int i)
{
}
