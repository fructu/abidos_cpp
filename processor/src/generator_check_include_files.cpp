/*------------------------------------------------------------------------------

    Proyecto            : abidos
    Codigo              : generator_class_diagram.cpp
    Descripcion         :
    Version             : 0.1
    Autor               : F. Manuel Hevia Garcia
    Fecha creacion      : 23/12/2011
    Fecha modificacion  :

    Observaciones :


------------------------------------------------------------------------------*/
#include "generator_check_include_files.h"
#include <string.h>

c_generator_check_include_files generator_check_include_files;

void get_constant(const char * file, char * constant)
{
    unsigned len = strlen(file);
    if (0 == len) {
        return;
    }
    unsigned i = 0;

    constant[i] = '\0';

    while (file[i] != '\0') {
        if (isalnum(file[i])) {
            constant[i] = toupper(file[i]);
        } else {
            constant[i] = '_';
        }

        ++i;
    }

    constant[i] = '\0';
}
/*----------------------------------------------------------------------------*/
void c_generator_check_include_files::push(const char *file)
{
    char constant[4000] = {'\0'};
    get_constant(file,constant);

    map_check_include_files[file] = constant;
}
/*----------------------------------------------------------------------------*/
void c_generator_check_include_files::run(char *p_file_out)
{
    FILE * f_out = NULL;;
    printf("## void c_generator_check_include_files::run(char * p_file_out [%s])\n",
           p_file_out);

    f_out = fopen(p_file_out, "w");
    if (NULL == f_out) {
        printf("  c_generator_check_include_files::run() cant fopen()\n");
        return;
    }

    t_map_check_include_files::iterator i_map = map_check_include_files.begin();

    for (i_map = map_check_include_files.begin();
            i_map != map_check_include_files.end(); ++i_map) {
        fprintf(f_out
                , "CHECK_INCLUDE_FILES(%s %s)\n"
                , (*i_map).first.c_str()
                , (*i_map).second.c_str()
               );
    }

    if (NULL != f_out) {
        fclose(f_out);
        f_out = NULL;
    }
}
/*----------------------------------------------------------------------------*/
