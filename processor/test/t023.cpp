/*
 compositions
 aggregations 
*/

class father
{
};

class A
{
};

class B: father
{
  A a;
};

class C
{
  B * b;
};

class D
{
  B & fDa(void);
  A & b;
};
