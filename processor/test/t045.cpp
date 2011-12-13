/*
  testing overloading operators;
*/
class foo {
public:
/*
    int         i;

    foo( int x = 0 )
        : i( x )
    {
    }
*/
    int f();
    int operator*();
//    int operator()();

    /*    
    {

        cout << "operator*() called\n";
        return i;
    
    }
*/        

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

};


int foo::operator*(){
}

