/*
  this is a fake of the std library
  very useful to testing abidos
*/
#include <stdio.h>

#ifndef _std_h
#define _std_h
//ban_symbols_on
namespace std
{
class string
{
};

template <class T1, class T2>
class map
{
  public:
    int size(void);
};


template <class T1>
class vector
{
  public:
    int size(void);
};

}
//ban_symbols_off
#endif
