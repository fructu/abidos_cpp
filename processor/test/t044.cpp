/*
  testing predeclaration;
  testing abstract class;
*/

  class Value;

  class A{
    Value v;
  };

  class Value {
    int value;
  };

  class B{
  public:
    void f1(void);
    virtual void f(void) = 0;
    void f2(void);
  };

  class C{
  public:
    void f1(void);
    void f2(void);
  };
