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

#define FILE_NAME_LEN (1000)

struct c_options {
    int verbose_flag;
    int test_original_flag;
    int ts_show_flag;
    int test_all_tokens_consumed_flag;
    int no_std_flag;
    int no_repeat_files_flag;
    int incasitive_includes_flag;
    int version_flag;
    int check_include_files_flag;

    char file[FILE_NAME_LEN];
    char file_loader[FILE_NAME_LEN];
    char includes[FILE_NAME_LEN];

    int loader_flag;

    c_options();
    void proccess(int argc, char *argv[]);
};

extern c_options options;
#endif
