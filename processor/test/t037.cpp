/*
  namespace testing
*/

namespace n1
{
  int i;

  void f(int p1);

  class A
  {
    int i;
    void f(int p1);
    
    class A_1
    {
      int i;
      void f(int p1);
    };
  };
}

void n1::A::f(int p1)
{
}

void n1::A::A_1::f(int p1)
{
}

