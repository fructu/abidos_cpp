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
#define LINE_LONG (5000)
/*----------------------------------------------------------------------------*/
void c_loader::process_line(char * line)
{
  string file_name = "";

  int i = 0;
  if(line[i] == '#') {
    return;
  }

  //process name
  while(line[i] != '\0') {
    if(line[i] == ':') {
      ++i;
      break;
    }
    if(line[i] == '\n') {
      ++i;
      continue;
    }

    file_name += line[i];
    ++i;
  }

  c_loader_file loader_file;

  loader_file.name = file_name;

  //process dirs
  string dir = "";

  while(line[i] != '\0') {
    if(line[i] == ':') {
      loader_file.vector_directories.push_back(dir);
      ++i;
      dir = "";
      continue;
    }

    if(line[i] == '\n') {
      ++i;
      continue;
    }

    dir += line[i];
    ++i;    
  }

  if( 0 < dir.length()) {
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
  for( i = 0; i < vector_files.size(); ++i) {
    printf("  [%s]\n", vector_files[i].name.c_str());
    for( j = 0; j < vector_files[i].vector_directories.size(); ++j) {
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

  while( NULL != fgets(line, LINE_LONG, f) ){
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
  if( !(position_actual < vector_files.size()) ) {
    return 0;
  }

  sprintf(file_name,"%s",vector_files[position_actual].name.c_str() );

  return 1;
}
/*----------------------------------------------------------------------------*/
int c_loader::include_file_get(char * file_name)
{
    FILE * f = NULL;
  
    if ((f = fopen(file_name, "r"))) {
        fclose(f);
        return 1;
    }   

  if( !(position_actual < vector_files.size()) ) {
    return 0;
  }   

    size_t j = 0;
    string dir_file = "";
    string file = file_name;

    for( j = 0; j < vector_files[position_actual].vector_directories.size(); ++j) {
      dir_file = vector_files[position_actual].vector_directories[j] + "/" + file;
      sprintf(file_name,"%s",dir_file.c_str());
      if ((f = fopen(file_name, "r"))) {
        fclose(f);
        return 1;
      }      
    }

    return 0;
}
/*----------------------------------------------------------------------------*/


