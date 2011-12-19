/*
  testing:
    std::string //it should drop std:: in parser
    unsigned channel : 6; //parsing :
*/

int f(int i = 0);

#include<string>

using namespace std;

class T{
  string s1;
  std::string s2;  
};

namespace N {
  class C{};
};

using namespace N;

using namespace N;

class B{
  C n1;
  N::C n2;
};

struct strip_data
{
  int i;
  unsigned channel : 6;
  int j;
  unsigned value : 10;
  void f(void) const;
  int table_1[10];
  int table_2[10][20];
  int table_3[10][20][30];
  int k;
};

class A
{
  class A_1{};

  class A_2
  {
    A_1 a1;
  };
};

