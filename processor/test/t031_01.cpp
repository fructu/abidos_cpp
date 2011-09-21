/*
  ##issue where f member appers like unsigned int * void f(void) in diagram
  
hevia@vulcano:~/working/abidos/processor/test$ g++ t031_01.cpp 
t031_01.cpp:21: error: ‘PATATA’ does not name a type
hevia@vulcano:~/working/abidos/processor/test$ g++ t031_01.cpp 
t031_01.cpp:6: error: new types may not be defined in a return type
t031_01.cpp:6: note: (perhaps a semicolon is missing after the definition of ‘A::C’)
t031_01.cpp:11: error: two or more data types in declaration of ‘f’
t031_01.cpp:18: error: no ‘void A::f()’ member function declared in class ‘A’
  class C
  {
  };<--- really need this ';'

*/
class A
{
    class C
    {
      unsigned int * c(int i1, unsigned int long i2, float f2);
    };

    void f(void);
};

unsigned int * A::C::c(int i1, unsigned int long i2, float f2)
{
}

void A::f(void)
{
}

main()
{
}
