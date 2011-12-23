/*------------------------------------------------------------------------------

    Proyecto            : abidos
    Codigo              : generator_class_diagram.h
    Descripcion         :
    Version             : 0.1
    Autor               : F. Manuel Hevia Garcia
    Fecha creacion      : 23/12/2011
    Fecha modificacion  :

    Observaciones :


------------------------------------------------------------------------------*/
#ifndef generator_check_include_files_h
#define generator_check_include_files_h
/*
  this map is to generate the file
  check_includes_files.txt
  CHECK_INCLUDE_FILES(stdio.h HAVE_STDIO_H)
*/
#include<map>
#include<string>
#include<stdio.h>

using namespace std;

typedef map<string,string> t_map_check_include_files;

class c_generator_check_include_files
{
private:
    t_map_check_include_files map_check_include_files;
public:
    void push(const char *file);
    void run(char *p_file_out);
};

extern c_generator_check_include_files generator_check_include_files;

#endif
