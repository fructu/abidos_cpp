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
}

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
typedef vector<string> t_vector_string;
