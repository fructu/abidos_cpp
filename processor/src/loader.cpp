/*------------------------------------------------------------------------------

    Proyecto            : abidos
    Codigo              : loader.h
    Descripcion         :
    Version             : 0.1
    Autor               : F. Manuel Hevia Garcia
    Fecha creacion      : 01/12/2011
    Fecha modificacion  :

    Observaciones :

------------------------------------------------------------------------------*/
#include <stdio.h>
#include "loader.h"
#include "options.h"

c_loader loader;
/*
  typedef vector<string> t_vector_directories;

struct c_loader_file
{
  string name;
  t_vector_directories vector_directories;
};

typedef vector<c_loader_file> t_vector_files;
*/

/*----------------------------------------------------------------------------*/
void c_loader::process_line(char * line)
{
    string file_name = "";

    int i = 0;
    if (line[i] == '#') {
        return;
    }

    //process name
    while (line[i] != '\0') {
        if (line[i] == ':') {
            ++i;
            break;
        }
        if (line[i] == '\n') {
            ++i;
            continue;
        }

        file_name += line[i];
        ++i;
    }

    c_loader_file loader_file;

    loader_file.name = file_name;

    if (1 == options.no_repeat_files_flag) {
        if (map_files.find(loader_file.name) != map_files.end() ) {
            // key exist
            if (1 == options.verbose_flag) {
                printf("warning %s repeated\n",loader_file.name.c_str());
            }
            return;
        }
    }

    map_files[loader_file.name] = 1;

    //process dirs
    string dir = "";

    while (line[i] != '\0') {
        if (line[i] == ':') {
            loader_file.vector_directories.push_back(dir);
            ++i;
            dir = "";
            continue;
        }

        if (line[i] == '\n') {
            ++i;
            continue;
        }

        dir += line[i];
        ++i;
    }

    if ( 0 < dir.length()) {
        loader_file.vector_directories.push_back(dir);
    }

    vector_files.push_back(loader_file);
}
/*----------------------------------------------------------------------------*/
void c_loader::print(void)
{
    size_t i = 0;
    size_t j = 0;

    printf("void c_loader::print() {\n");
    for ( i = 0; i < vector_files.size(); ++i) {
        printf("  [%s]\n", vector_files[i].name.c_str());
        for ( j = 0; j < vector_files[i].vector_directories.size(); ++j) {
            printf("    [%s]\n", vector_files[i].vector_directories[j].c_str());
        }
    }
    printf("}\n");
}
/*----------------------------------------------------------------------------*/
void c_loader::process_file(char * file_loader)
{
    printf("## c_loader::process_file(char file_loader) [%s]\n", file_loader);
    FILE *f;

    if (!(f = fopen(file_loader,"r") )) {
        printf("\nERROR: c_loader::process_file did not can open [%s]\n", file_loader);
        return;
    }

    char line[LINE_LONG] = {0};

    while ( NULL != fgets(line, LINE_LONG, f) ) {
        process_line(line);
    }

    print();

    fclose(f);
}
/*----------------------------------------------------------------------------*/
void c_loader::begin(void)
{
    position_actual = 0;
}
/*----------------------------------------------------------------------------*/
void c_loader::next(void)
{
    ++position_actual;
}
/*----------------------------------------------------------------------------*/
int c_loader::file_get(char * file_name)
{
    if ( !(position_actual < vector_files.size()) ) {
        return 0;
    }

    sprintf(file_name,"%s",vector_files[position_actual].name.c_str() );

    return 1;
}
/*----------------------------------------------------------------------------*/
void string_to_lower(char str[LINE_LONG])
{
    if (1 != options.incasitive_includes_flag) {
        return;
    }

    char *p = str;
    while ( *p != '\0' ) {
        *p = tolower(*p);
        ++p;
    }
}
/*----------------------------------------------------------------------------*/
int c_loader::include_file_get(char * file_name)
{
    FILE * f = NULL;

    if (options.no_repeat_files_flag &&
            map_files.find(file_name) != map_files.end() ) {
        // key exist
        if (1 == options.verbose_flag) {
            printf("warning %s repeated\n",file_name);
        }
        return 0;
    } else if ((f = fopen(file_name, "r"))) {
        fclose(f);
        char file_name_lower[LINE_LONG];
        sprintf(file_name_lower,"%s",file_name);
        string_to_lower(file_name_lower);
        map_files[file_name] = 1;
        map_files[file_name_lower] = 1;
        return 1;
    } else if ( 1 == try_open_file_lowercase(NULL, file_name)) {
        map_files[file_name] = 1;
        return 1;
    }

    if ( !(position_actual < vector_files.size()) ) {
        return 0;
    }

    size_t j = 0;
    string dir_file = "";
    string file = file_name;

    for ( j = 0; j < vector_files[position_actual].vector_directories.size(); ++j) {
        dir_file = vector_files[position_actual].vector_directories[j] + "/" + file;
        sprintf(file_name,"%s",dir_file.c_str());

        if (1 == options.no_repeat_files_flag) {

            if (map_files.find(file_name) != map_files.end() ) {
                // key exist
                if (1 == options.verbose_flag) {
                    printf("warning %s repeated\n",file_name);
                }
                continue;
            }

            map_files[file_name] = 1;
        }

        if ((f = fopen(file_name, "r"))) {
            fclose(f);
            return 1;
        }

        char directory[LINE_LONG] = {'\0'};
        sprintf(directory,"%s",vector_files[position_actual].vector_directories[j].c_str());
        if ( 1 == try_open_file_lowercase(directory, file_name)) {
            return 1;
        }
    }

    return 0;
}
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
#include <dirent.h>
#include <sys/types.h>
#include <sys/param.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
/*----------------------------------------------------------------------------*/
/*
  do a map
  d/H1.h

  map d -> map
    h1.h -> H1.h
*/
void c_loader::do_map_files_lowercase( char  * directory )
{
    if (1 != options.incasitive_includes_flag) {
        return;
    }

    char result[LINE_LONG];
    DIR           *d;
    struct dirent *dir;

    if ( map_directory_files.find(directory) != map_directory_files.end() ) {
        return;
    }

    d = opendir( directory );

    if ( d == NULL ) {
        return;
    }

    t_map_files_lowercase map_files_lowercase;
    while ( ( dir = readdir( d ) ) ) {
        if ( strcmp( dir->d_name, "." ) == 0 ||
                strcmp( dir->d_name, ".." ) == 0 ) {
            continue;
        }
        if ( dir->d_type == DT_DIR ) {
            continue;
        }

        snprintf( result, MAXPATHLEN, "%s", dir->d_name );

        string_to_lower(result);
        string str_lower = result;
        string str_file = dir->d_name;
        map_files_lowercase[str_lower] = str_file;
    }

    map_directory_files[directory] = map_files_lowercase;

    closedir( d );

    return;
}
/*----------------------------------------------------------------------------*/
string c_loader::search_file_lowercase( char  * directory, char  * file )
{
    string str_file = "";
    string str_dir = "";

    if (1 != options.incasitive_includes_flag) {
        return str_file;
    }

    char file_lower[LINE_LONG];
    sprintf(file_lower,"%s",file);
    string_to_lower(file_lower);
    string str_lower_file = file_lower;

    if ( NULL == directory ) {
        do_map_files_lowercase((char *)".");
        str_dir = ".";
    } else {
        str_dir = directory;
        do_map_files_lowercase(directory);
    }

    if ( map_directory_files.find(str_dir) == map_directory_files.end() ) {
        return str_file;
    }

    if ( map_directory_files[str_dir].find(str_lower_file) == map_directory_files[str_dir].end() ) {
        return str_file;
    }

    str_file = map_directory_files[str_dir][str_lower_file];

    return str_file;
}
/*----------------------------------------------------------------------------*/
int c_loader::try_open_file_lowercase( char  * directory, char  * file )
{
    string str_file = search_file_lowercase(directory,file);
    FILE * f = NULL;

    if ( 0 == str_file.size() ) {
        return 0;
    }

    string str_dir_file = "";
    if ( NULL != directory ) {
        str_dir_file = directory;
        str_dir_file += "/" + str_file;
    } else {
        str_dir_file = str_file;
    }

    if (map_files.find(str_dir_file) != map_files.end() ) {
        // key exist
        if (1 == options.verbose_flag) {
            printf("warning %s repeated\n",str_dir_file.c_str());
        }
        return 0;
    }

    if ((f = fopen(str_dir_file.c_str(), "r"))) {
        printf("###2#incasitive %s->%s\n",file, str_dir_file.c_str());
        sprintf(file,"%s",str_file.c_str());
        fclose(f);

        map_files[str_dir_file] = 1;
        return 1;
    }

    return 0;
}
/*----------------------------------------------------------------------------*/

