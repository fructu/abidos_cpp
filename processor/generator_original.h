/*------------------------------------------------------------------------------

    Proyecto            : abidos
    Codigo              : generator_original.h
    Descripcion         :
    Version             : 0.1
    Autor               : F. Manuel Hevia Garcia
    Fecha creacion      : 06/04/2011
    Fecha modificacion  :

    Observaciones :


------------------------------------------------------------------------------*/
#ifndef generator_original_h
#define generator_original_h

#include "symbols_table.h"

class c_generator_original
{
private:
  FILE * f_out;
  void members(t_vector_class_member & vector_class_member);
  void classes(c_symbol & symbol);
  void inheritance(c_symbol & symbol);
public:
  void run(char *p_file_out);
};
#endif
