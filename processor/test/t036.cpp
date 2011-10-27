/*
  template testing
*/

template <class T, class T2, typename T3>
class A {
      T t;
 public:
      void f(int i)
      {
      }
};

template <class T1, int N>
class A2 {
};

class A_2{
};

template <class T, typename T2>
class A3 {
};

typedef A3<int,double> t_A3;
typedef int T_int;
class B {
//  A_2<int,1> a2;
  A3<int,float> a3;// template instantation
  t_A3 a3_2;
  T_int i;
};


typedef int T_int;
typedef int T2_int;

class AA
{
  T_int i;
  T_int i2;
  void f(T_int f1, int f2);
};

void f_free(T_int f1, int f2);
