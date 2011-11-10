/*
  testing preprocessor
*/
#ifndef d
#define d
class esta_si{};
#endif

#ifndef d
//this would dont appears in ts
class esta_no{};
#endif

class esta_tambien{};

