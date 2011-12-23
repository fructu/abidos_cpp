/*------------------------------------------------------------------------------

    Proyecto            :
    Codigo              : main.cpp
    Descripcion         :
    Version             : 0.1
    Autor               : F. Manuel Hevia Garcia
    Fecha creacion      : 03/03/2011
    Fecha modificacion  :

    Observaciones:
        ->

------------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>

#include "parser_descent.h"
#include "options.h"
#include "loader.h"
#include "config.h"
#include "generator_check_include_files.h"

// included only to do tests
// #include "symbols_table.h"

void test_01(void)
{
    c_parser_descent parser;

    parser.test_01();
}

void process_file(char file_name[])
{
    c_parser_descent parser;

    parser.yyparse(file_name);
    // in this place there are some important thinks destroyed
    // like lex_file_name
    // generator.run((char *)"out.gv");
}

void process_file_loader(char file_name[])
{
    c_parser_descent parser;

    parser.yyparse_loader(file_name);
}

void print_version(void)
{
    printf("%s %d.%d.%02d_%s [%s]\n"
           ,EXECUTABLE
           ,VERSION_MAJOR
           ,VERSION_MINOR
           ,VERSION_PATCH
           ,CMAKE_BUILD_TYPE
           ,PROJECT_COMPILATION_TIME
          );
}

int main(int argc, char *argv[])
{
    options.proccess(argc, argv);

    if (1 == options.version_flag) {
        print_version();
        return 0;
    };

    // ts_tests();

    // test_01();

    if (argc < 2) {
        printf("error file expected\n");
        exit(1);
    }

    if ( 1 == options.loader_flag) {
        process_file_loader(options.file_loader);
    } else {
        process_file(options.file);
    }

    if ( 1 == options.check_include_files_flag ) {
        generator_check_include_files.run(".abidos/check_include_files.txt");
    }

    return 0;
}
