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
#include <map>

using namespace std;

typedef vector<string> t_vector_directories;

struct c_loader_file {
    string name;
    t_vector_directories vector_directories;
};

typedef vector<c_loader_file> t_vector_files;
typedef map<string,int> t_map_files;

typedef map<string,string> t_map_files_lowercase;
typedef map<string,t_map_files_lowercase> t_map_directory_files;

class c_loader
{
private:
    t_vector_files vector_files;
    t_map_files map_files;

    void process_line(char * line);

    size_t position_actual;

    t_map_directory_files map_directory_files;
public:
    void print(void);
    void process_file(char * file_loader);

    void begin(void);
    void next(void);
    int  file_get(char * file_name);

    //change file_name too
    int include_file_get(char * file_name);

    //open files incasitive way
    void do_map_files_lowercase( char  * directory );
    string search_file_lowercase( char  * directory, char  * file );
    int try_open_file_lowercase( char  * directory, char  * file );
};

//typedef vector<c_trace_node> t_vector_trace_nodes;

extern c_loader loader;
#endif

