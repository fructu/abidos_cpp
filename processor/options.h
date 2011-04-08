/*------------------------------------------------------------------------------

    Proyecto            : abidos
    Codigo              : options.h
    Descripcion         :
    Version             : 0.1
    Autor               : F. Manuel Hevia Garcia
    Fecha creacion      : 08/04/2011
    Fecha modificacion  :

    Observaciones :


------------------------------------------------------------------------------*/
#ifndef options_h
#define options_h

struct c_options
{
	int verbose_flag;
	int test_original_flag;
	int ts_show_flag;
	char file[1000];

	c_options();
	void proccess(int argc, char* argv[]);
};

extern c_options options;
#endif
