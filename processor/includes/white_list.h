/*------------------------------------------------------------------------------

    Proyecto            : abidos
    Codigo              : white_list.h
    Descripcion         :
    Version             : 0.1
    Autor               : F. Manuel Hevia Garcia
    Fecha creacion      : 03/12/2012
    Fecha modificacion  :

    Observaciones :

------------------------------------------------------------------------------*/
#ifndef white_list_h
#define white_list_h

#include <string>
#include <map>

using namespace std;

//we only need a map to search classes and know if is in a white list or not
typedef map<string,int> t_map_files;

class c_white_list
{
private:
    t_map_files map_files;

    void process_line(char * line);

public:
    c_white_list();
    int active;

    void print(void);
    void process_file(char * file_loader);

    int find(string s);
};
/*----------------------------------------------------------------------------*/
extern c_white_list white_list;
/*----------------------------------------------------------------------------*/
#endif
/*----------------------------------------------------------------------------*/
