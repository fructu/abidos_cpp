/*
  junping to nested constructor definition
*/

class A
{
  A();
  ~A();  
  class B
  {
    B();
    ~B();

    class C
    {
      C();
      ~C(); 
      unsigned int * c(int i1, unsigned int long i2, float double f2);
    };

    void f(void);
  };
};

A::A()
{
}

A::~A()
{
}

A::B::C::C()
{
}

A::B::C::~C()
{
}

unsigned int * A::B::C::c(int i1, unsigned int long i2, float double f2)
{
}

void A::B::f(void)
{
}

A::B::B()
{
}

A::B::~B()
{
}

