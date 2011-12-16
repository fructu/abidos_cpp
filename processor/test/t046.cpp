/*
  testing:
     overloading operators;
     default parameters values;
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

