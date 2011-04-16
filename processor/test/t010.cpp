/*
	private, public, protected members
*/
class A
{
  int a;
private:
  int b;
protected:  
  int c;
public:
  int d;  
};

struct S: public A
{
  int s1;
  int s2;
  int s3;
private:
  int s4;
};
