/*
  constructor
*/

class A
{
  A();
};

class B
{
  int i;

  B()
  {
    i = 0;
  }
};

class C
{
  C(int i);
};

class D: C
{
//
  D(int f):C(f){}
// this is not in grammar
//  D(int f):C(f);
};

/*
D:D(int f)
{
}
*/
