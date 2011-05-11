/*------------------------------------------------------------------------------

    Proyecto            : abidos
    Codigo              : generator_class_diagram.cpp
    Descripcion         :
    Version             : 0.1
    Autor               : F. Manuel Hevia Garcia
    Fecha creacion      : 05/04/2011
    Fecha modificacion  :

    Observaciones :


------------------------------------------------------------------------------*/
#include <stdio.h>
#include "generator_class_diagram.h"
#include "lex_yacc.h"

/*----------------------------------------------------------------------------*/

const char * colon_colon_substitution(string source)
{
  size_t j;
  string str = source;
  for ( ; (j = str.find( "::" )) != string::npos ; )
    {
      str.replace( j, 2, "__" );
    }

  return str.c_str();
}

/*----------------------------------------------------------------------------*/
void c_generator_class_diagram::members_url(t_vector_class_member &
    vector_class_member)
{
  unsigned i_member = 0;
  for (i_member = 0; i_member < vector_class_member.size(); ++i_member)
    {
      fprintf(f_out, "%s",
              vector_class_member[i_member]->token.text.c_str());

      if ( 0 < vector_class_member[i_member]->token_definition.file.size())
        {
          fprintf(f_out, "[%s:%d]",
                  vector_class_member[i_member]->token_definition.file.c_str(),
                  vector_class_member[i_member]->token_definition.line);
        }
      else
        {
          fprintf(f_out, "[%s:%d]",
                  vector_class_member[i_member]->token.file.c_str(),
                  vector_class_member[i_member]->token.line);
        }

      if ((i_member + 1) < vector_class_member.size())
        {
          fprintf(f_out, ";");
        }
    }
}

/*----------------------------------------------------------------------------*/
void c_generator_class_diagram::members_label(t_vector_class_member &
    vector_class_member)
{
  unsigned i_member = 0;
  for (i_member = 0; i_member < vector_class_member.size(); ++i_member)
    {
      unsigned i_decl = 0;

      switch ( vector_class_member[i_member]->access_specifier )
        {
        case PUBLIC:
          fprintf(f_out, "+ ");
          break;
        case PRIVATE:
          fprintf(f_out, "- ");
          break;
        case PROTECTED:
          fprintf(f_out, "# ");
          break;
        default:
          fprintf(f_out, "? ");
        }

      for (i_decl = 0;
           i_decl <
           vector_class_member[i_member]->vector_decl_specifier.size();
           ++i_decl)
        {
          fprintf(f_out, "%s ",
                  vector_class_member[i_member]->vector_decl_specifier
                  [i_decl].token.text.c_str());
        }

      fprintf(f_out, "%s",
              vector_class_member[i_member]->get_full_name().c_str());

      fprintf(f_out, "\\l");
    }
}

/*----------------------------------------------------------------------------*/
/*
 * A [ URL="A;a1;age;die", label = "{A|+ a1 : string\l+ age : int\l|+
 * die() : void\l}" ]
 */
void c_generator_class_diagram::classes(c_symbol & symbol)
{
  fprintf(f_out, "/* c_generator_class_diagram::classes() */\n");
  // first[B] id[258]->[IDENTIFIER] text[B] type[265]->[CLASS_NAME]
  // class_key[300]->[CLASS]
  if (CLASS_NAME != symbol.type)
    {
      return;
    }

  fprintf(f_out, "  %s [\n", colon_colon_substitution(symbol.token.text));
  fprintf(f_out, "    URL=\"%s[%s:%d];", colon_colon_substitution(symbol.token.text),
          symbol.token.file.c_str(), symbol.token.line);
  fprintf(f_out, "%s[%s:%d];", symbol.token.text.c_str(),
          symbol.token.file.c_str(), symbol.token.line);
  members_url(symbol.members.vector_class_member);
  fprintf(f_out, "\",\n");

  fprintf(f_out, "    label=\"{%s|", symbol.token.text.c_str());
  members_label(symbol.members.vector_class_member);
  fprintf(f_out, "}\"\n");
  fprintf(f_out, "  ]\n");

}

/*----------------------------------------------------------------------------*/
/*
 * B->Animal;
 */
void c_generator_class_diagram::inheritance(c_symbol & symbol)
{
  fprintf(f_out, "/* c_generator_class_diagram::inheritance() */\n");
  if (CLASS_NAME != symbol.type)
    {
      return;
    }

  t_map_base_class::iterator i_map_base = symbol.map_base_class.begin();

  for (i_map_base = symbol.map_base_class.begin();
       i_map_base != symbol.map_base_class.end(); ++i_map_base)
    {
      // C1->B [dir = "back"];
      fprintf(f_out, "  /*%s->%s*/", symbol.token.text.c_str(),
              ((*i_map_base).second).text.c_str());

      fprintf(f_out, "  %s->%s [dir = \"back\", color = \"black\", arrowtail = \"empty\"];\n"
              ,((*i_map_base).second).text.c_str()
              , symbol.token.text.c_str() );
    }
}

/*----------------------------------------------------------------------------*/
/*
 * B->Animal;
 */
void c_generator_class_diagram::friends(c_symbol & symbol)
{
  fprintf(f_out, "/* c_generator_class_diagram::friends() */\n");
  if (CLASS_NAME != symbol.type)
    {
      return;
    }

  t_map_friend_class::iterator i_map_friend = symbol.map_friend_class.begin();

  for (i_map_friend = symbol.map_friend_class.begin();
       i_map_friend != symbol.map_friend_class.end(); ++i_map_friend)
    {
      // C1->B [dir = "back"];
      fprintf(f_out, "  /*%s->%s*/", symbol.token.text.c_str(),
              ((*i_map_friend).second).text.c_str());

      fprintf(f_out, "  %s->%s [dir = \"back\", color = \"gray\", arrowtail = \"open\"];\n"
              ,((*i_map_friend).second).text.c_str()
              , symbol.token.text.c_str() );
    }
}

/*----------------------------------------------------------------------------*/
void c_generator_class_diagram::members_compositions_aggregations(
  c_symbol & symbol,
  t_vector_class_member & vector_class_member)
{
  int is_ptr = 0;
  string class_name = "";
  unsigned i_member = 0;
  for (i_member = 0; i_member < vector_class_member.size(); ++i_member)
    {
      unsigned i_decl = 0;

      c_class_member * p_member = vector_class_member[i_member];

      if ( 1 == p_member->is_function)
        {
          continue;
        }

      is_ptr=0;

      for ( i_decl = 0;
            i_decl < vector_class_member[i_member]->vector_decl_specifier.size();
            ++i_decl)
        {
          c_decl_specifier * p_decl_specifier = & vector_class_member[i_member]->vector_decl_specifier[i_decl];

          if ( 0 != class_name.size() )
            {
              if (
                ('*' == p_decl_specifier->token.id) ||
                ('&' == p_decl_specifier->token.id)
              )
                {
                  is_ptr = 1;
                }
            }

          if ( CLASS_NAME != p_decl_specifier->token.id )
            {
              continue;
            }

          class_name = p_decl_specifier->token.text;
        }

      if ( 0 == class_name.size() )
        {
          continue;
        }

      fprintf(f_out, "  /*%s -> %s*/"
              , symbol.token.text.c_str()
              , class_name.c_str()
             );

      if ( 0 == is_ptr )
        {
          fprintf(f_out, "  %s->%s [dir = \"back\", color = \"gray\", arrowtail = \"diamond\"];\n"
                  , symbol.token.text.c_str()
                  , class_name.c_str()
                 );
        }
      else
        {
          fprintf(f_out, "  %s->%s [dir = \"back\", color = \"gray\", arrowtail = \"odiamond\"];\n"
                  , symbol.token.text.c_str()
                  , class_name.c_str()
                 );
        }

      class_name = "";
    }
}

/*----------------------------------------------------------------------------*/

void c_generator_class_diagram::compositions_aggregations(c_symbol & symbol)
{
  if (CLASS_NAME != symbol.type)
    {
      return;
    }
  fprintf(f_out, "/* compositions_aggregations */\n");

  members_compositions_aggregations(symbol, symbol.members.vector_class_member);
}

/*----------------------------------------------------------------------------*/
void c_generator_class_diagram::run(char *p_file_out)
{
  printf
  ("## void c_generator_class_diagram::run(char * p_file_out [%s])\n",
   p_file_out);

  f_out = NULL;

  f_out = fopen(p_file_out, "w");
  if (NULL == f_out)
    {
      printf("  c_generator_class_diagram::run() cant fopen()\n");
      return;
    }

#include "out/generate_begin_graph.cpp"

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

  for (i_stack = 0; i_stack < ts.stack.size(); ++i_stack)
    {
      printf("  stack level[%d]\n", i_stack);
      for (i_map = ts.stack[i_stack].begin();
           i_map != ts.stack[i_stack].end(); ++i_map)
        {
          inheritance((*i_map).second);
          friends((*i_map).second);
          compositions_aggregations((*i_map).second);
        }
    }


  fprintf(f_out, "}\n");
  if (NULL != f_out)
    {
      fclose(f_out);
      f_out = NULL;
    }
}

/*----------------------------------------------------------------------------*/
