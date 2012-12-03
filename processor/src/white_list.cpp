/*------------------------------------------------------------------------------

    Proyecto            : abidos
    Codigo              : white_list.cpp
    Descripcion         :
    Version             : 0.1
    Autor               : F. Manuel Hevia Garcia
    Fecha creacion      : 03/12/2012
    Fecha modificacion  :

    Observaciones :

------------------------------------------------------------------------------*/
#include <stdio.h>
#include "white_list.h"
#include "options.h"

c_white_list white_list;
/*----------------------------------------------------------------------------*/
c_white_list::c_white_list()
{
    active = 0;
}
/*----------------------------------------------------------------------------*/
void c_white_list::process_line(char * line)
{
    string file_name = "";

    int i = 0;
    if (line[i] == '#') {
        return;
    }

    //process name
    while (line[i] != '\0') {
        if (line[i] == '\n') {
            ++i;
            continue;
        }

        file_name += line[i];
        ++i;
    }

    map_files[file_name] = 1;
}
/*----------------------------------------------------------------------------*/
void c_white_list::print(void)
{
    t_map_files::iterator i_map_member = map_files.begin();

    printf("void c_white_list::print() {\n");
    for (i_map_member = map_files.begin();
            i_map_member != map_files.end(); ++i_map_member) {

        printf("  first[%s]->[%d]\n"
               //, yytokens[((*i_map_member).second).access_specifier]
               , ((*i_map_member).first).c_str()
               , ((*i_map_member).second));
    }

    printf("}\n");
}
/*----------------------------------------------------------------------------*/
void c_white_list::process_file(char * file_name)
{
    printf("## c_white_list::process_file(char file_name) [%s]\n", file_name);
    FILE *f;

    if (!(f = fopen(file_name,"r") )) {
        active = 0;
        printf("\nERROR: c_white_list::process_file did not can open [%s]\n", file_name);
        return;
    }

    active = 1;

    char line[LINE_LONG] = {0};

    while ( NULL != fgets(line, LINE_LONG, f) ) {
        process_line(line);
    }

    print();

    fclose(f);
}
/*----------------------------------------------------------------------------*/
int c_white_list::find(string s)
{
    int result = 0;

    if ( 0 == active) {
        return 1;
    }

    t_map_files::iterator i_map = map_files.find(s);
    if (i_map != map_files.end()) {
        result = (*i_map).second;
        return( result );
    }

    return 0;
}
/*----------------------------------------------------------------------------*/
