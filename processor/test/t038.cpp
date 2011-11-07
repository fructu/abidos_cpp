/*
  using namespace testing
*/

namespace n1
{

  class A
  {
    void f(int p1)
    {
    }
  };
}

using namespace n1;

class B
{
  A a;
};

namespace std
{
  class string
  {
  };

  template <class T>
  class vector {
  };

  class V
  {
    vector<int> o_vector;
  };
}

typedef std::vector<float> t_vector_float;
typedef std::vector<int> t_vector_int;

using namespace std;
typedef vector<char> t_vector_char;
typedef vector<string> t_vector_string;
//typedef vector<string> t_vector_string;

namespace C
{
  template <class T1,class T2,class T3>
  class v {
  };
  

  template <class Z1,class Z2,class Z3>
  class m {
  };
}

typedef C::v<int, float, double> t_vector;
typedef C::m<char, int, float *> t_map;

typedef vector < long int > t_vector_class_member;


namespace PP
{
  template <class T1, class T2>
  class P {
  };

  class Q {
    P<double int &, float *> p1;
    P<long int, double float> p2;
  };
}

typedef PP::P<int * * * & &, float> p5;


  template <class T1, class T2>
  class Z {
    t_vector_string s;
  };

  typedef Z<int long *, float double & *> z6;

