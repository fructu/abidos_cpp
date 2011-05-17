/*
   define classes inside classes
*/

class A
{

  class A_1
  {
    int A_1_i;
  };

  class A_2
  {
    class A_2_1
    {
      int A_2_1_i1;
      int A_2_1_f(int long p1);
    };
  };

  class A_3
  {
    class A_3_1
    {
    };
  };
  
  A();
};

A::A()
{
};

class B
{
};

B::B::B::B()
{
}

B B::B::B::B()
{
}

int B::B::B::B(int p1)
{
}

int B::B::B::B::f_b(int p1)
{
}

float aqui;

int A::A_2(int long p1)
{
}

int A::A_2::A_2_1(int long p1)
{
}

float aqui;

int A::A_2::A_2_1::A_2_1_f(int long p1)
{
}

