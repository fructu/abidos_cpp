/*
  definition of function member outside of class
*/
class A
{
  A();
  A(int p1);
  ~A();
  void fA(void);
  int unsigned fA(int i1, float double fd2, char c3);
};

A::A()
{
}

A::A(int p1)
{
}

void A::fA(void)
{
}

int unsigned A::fA(int i1, float double fd2, char c3)
{
}

void f1_no_member(char a, float f)
{
}

A::~A()
{
}

