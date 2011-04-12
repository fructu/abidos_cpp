/*------------------------------------------------------------------------------

    Proyecto            : abidos
    Codigo              : generator_original.cpp
    Descripcion         :
    Version             : 0.1
    Autor               : F. Manuel Hevia Garcia
    Fecha creacion      : 06/04/2011
    Fecha modificacion  :

    Observaciones :


------------------------------------------------------------------------------*/
#include "generator_original.h"
#include "lex_yacc.h"
#include "tokens.h"

#include <stdio.h>
/*----------------------------------------------------------------------------*/
/*
 * B->Animal;
 */
void
c_generator_original::inheritance(c_symbol & symbol)
{
  if (CLASS_NAME != symbol.type)
    {
      return;
    }

  unsigned i_base = 0;

  if (symbol.vector_base_class.size() > 0)
    {
      fprintf(f_out, " : \n");
    }

  for (i_base = 0; i_base < symbol.vector_base_class.size(); ++i_base)
    {
      switch (symbol.vector_base_class[i_base].access_specifier)
        {
        case PUBLIC:
          fprintf(f_out, " public");
          break;
        case PRIVATE:
          fprintf(f_out, " private");
          break;
        case PROTECTED:
          fprintf(f_out, " protected");
          break;
        }

      fprintf(f_out, " %s",
              symbol.vector_base_class[i_base].text.c_str());

      if ((i_base + 1) < symbol.vector_base_class.size())
        {
          fprintf(f_out, ",");
        }

      fprintf(f_out, "\n");
    }
}

/*----------------------------------------------------------------------------*/
void
c_generator_original::
members(t_vector_class_member & vector_class_member)
{
  unsigned i_member = 0;
  int access=0;
  for (i_member = 0; i_member < vector_class_member.size(); ++i_member)
    {
      unsigned i_decl = 0;
      if( access != vector_class_member[i_member]->access_specifier )
        {
          access = vector_class_member[i_member]->access_specifier;
          switch( access )
            {
              case PUBLIC:
                fprintf(f_out, "public:\n");
                break;
              case PRIVATE:
                fprintf(f_out, "private:\n");
                break;
              case PROTECTED:
                fprintf(f_out, "protected:\n");
                break;
              default:
                fprintf(f_out, "//bad access_specifier\n");
            }
        }

      fprintf(f_out, " ");

      for (i_decl = 0;
           i_decl <
           vector_class_member[i_member]->vector_decl_specifier.size();
           ++i_decl)
        {
          fprintf(f_out, "%s ",
                  vector_class_member[i_member]->vector_decl_specifier
                  [i_decl].token.text.c_str());
        }

      fprintf(f_out, "%s;",
              vector_class_member[i_member]->get_full_name().c_str());

      fprintf(f_out, "\n");
    }
}

/*----------------------------------------------------------------------------*/
void c_generator_original::classes(c_symbol & symbol)
{
  // first[B] id[258]->[IDENTIFIER] text[B] type[265]->[CLASS_NAME]
  // class_key[300]->[CLASS]
  if (CLASS_NAME != symbol.type)
    {
      return;
    }
  switch (symbol.class_key)
    {
    case CLASS:
      fprintf(f_out, "class ");
      break;
    case STRUCT:
      fprintf(f_out, "struct ");
      break;
    default:
      printf
      ("  error c_generator_original::classes() bad symbol.class_key\n");
      // exit(-1);
    }

  fprintf(f_out, "  %s ", symbol.token.text.c_str());
  inheritance(symbol);
  fprintf(f_out, "{\n");
  members(symbol.members.vector_class_member);
  fprintf(f_out, "};\n");
}

/*----------------------------------------------------------------------------*/
void c_generator_original::run(char *p_file_out)
{
  printf("## void c_generator_original::run(char * p_file_out [%s])\n",
         p_file_out);

  f_out = NULL;

  f_out = fopen(p_file_out, "w");
  if (NULL == f_out)
    {
      printf("  c_generator_original::run() cant fopen()\n");
      return;
    }

  unsigned i_stack = 0;

  t_symbols::iterator i_map;

  for (i_stack = 0; i_stack < ts.stack.size(); ++i_stack)
    {
      printf("  stack level[%d]\n", i_stack);
      for (i_map = ts.stack[i_stack].begin();
           i_map != ts.stack[i_stack].end(); ++i_map)
        {
          classes((*i_map).second);
        }
    }

  if (NULL != f_out)
    {
      fclose(f_out);
      f_out = NULL;
    }
}

/*----------------------------------------------------------------------------*/
