/*
function_specifier:
	INLINE
	| VIRTUAL
	| EXPLICIT
	;
*/
class A
{
  inline void fa(void);
  virtual void fv1(int i1);
  explicit void fe1(int i1);  
};

