/*
  this is a fake of the std library
  very useful to testing abidos
*/
#ifndef _std_h
#define _std_h

struct FILE {};

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

#endif
