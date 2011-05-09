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

};

class B
{
};

/*
## this will be funy ... :-)

int A::A_2::A_2_1::A_2_1_f(int long p1)
{
};
*/
