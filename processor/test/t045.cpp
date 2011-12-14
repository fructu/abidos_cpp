/*
  testing:
     overloading operators;
     default parameters values;
*/
/*
int f(int i = 0);
*/
class foo {
public:
    foo( int x = 0)
        : i( x )
    {
    }

    int f();

    int operator*();

    int operator()()
    {
        cout << "operator*() called\n";
        return i;
    
    }

    foo operator++()
    {
        cout << "operator++() called\n";
        ++ i;
        return * this;
    }

    foo operator++(int num)
    {
        cout << "operator++(int) called\n";
        i ++;
        return * this;
    }


    int operator[]();

    int operator new();
    int operator new[]();
};

int foo::operator*(){
}

int foo::operator[]()
{
}

int foo::operator new()
{
}

int foo::operator new[]()
{
}

