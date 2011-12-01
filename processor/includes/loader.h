/*------------------------------------------------------------------------------

    Proyecto            : abidos
    Codigo              : loader.h
    Descripcion         :
    Version             : 0.1
    Autor               : F. Manuel Hevia Garcia
    Fecha creacion      : 01/12/2011
    Fecha modificacion  :

    Observaciones :
      this class has a file input when recive the files compilated in the project
      and his directories.
      example:
        options.cpp:../includes

      works in a more easy way than the preprocessor does.

------------------------------------------------------------------------------*/
#ifndef loader_h
#define loader_h

#include <string>
#include <vector>

using namespace std;

typedef vector<string> t_vector_directories;

struct c_loader_file
{
  string name;
  t_vector_directories vector_directories;
};

typedef vector<c_loader_file> t_vector_files;

class c_loader
{
private:
  t_vector_files vector_files;

  void process_line(char * line);
  
  size_t position_actual;
public:
  void print(void);
  void process_file(char * file_loader);
  
  void begin(void);
  void next(void);
  int  file_get(char * file_name);

  //change file_name too
  int include_file_get(char * file_name);
};

//typedef vector<c_trace_node> t_vector_trace_nodes;

extern c_loader loader;
#endif

