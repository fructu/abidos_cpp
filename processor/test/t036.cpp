/*
  template testing
*/
/*
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
*/

class A_2{
};

template <class T>
class A3 {
};

//typedef A3<int> t_A3;
class B {
  A_2 a2;
  A3<int> a3;// template instantation
//  t_A3 a3_2;
};
