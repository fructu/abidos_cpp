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

template <class T>
class A3 {
};

class B {
  A3<int> a3;// template instantation
};
