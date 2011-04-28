/*
storage_class_specifier:
	AUTO
	| REGISTER
	| STATIC
	| EXTERN
	| MUTABLE
	;

*/
class A
{
  auto int a1;
  auto int * p_i;
  register int  r1;
  static int s1;
  extern int long e1;
  mutable m1;
};

