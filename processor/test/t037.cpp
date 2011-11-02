/*
  namespace testing
*/
namespace n1
{
  int i1;
  void f(int p1);

  class A
  {
    int i;
    void f(int p1);
    void f2(void);
    class A_1
    {
      int i;
      void f(int p1);
    };
  };

  void A::f2(void)
  {
  }
}

void n1::f(int p1)
{
}

void n1::A::f(int p1)
{
}

void n1::A::A_1::f(int p1)
{
}

namespace C
{
  class C_1
  {
    public:
     void fc(void);
  };
}

void C::C_1::fc(void)
{
}

int main()
{
  C::C_1 o;

  o.fc();
}
