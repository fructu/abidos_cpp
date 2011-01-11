/*------------------------------------------------------------------------------

	Proyecto            : 
	Codigo              : includes_semantic.h
	Descripcion         : 
	Version             : 0.1
	Autor               : F. Manuel Hevia Garcia
	Fecha creacion      : 04/06/2008
	Fecha modificacion  : 

	Observaciones:
     	->

------------------------------------------------------------------------------*/
#ifndef semantico_h
#define semantico_h
/*----------------------------------------------------------------------------*/
class c_includes_semantic
{
    private: 
        void init(void);        
    protected:
    public:        
        c_includes_semantic();
        ~c_includes_semantic();

		void include_process(char * f);
};
/*----------------------------------------------------------------------------*/
extern c_includes_semantic includes_semantic;
/*----------------------------------------------------------------------------*/
#endif

/*----------------------------------------------------------------------------*/
