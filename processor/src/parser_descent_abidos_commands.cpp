/*------------------------------------------------------------------------------

    Proyecto            : show_includes
    Codigo              : is_banned
    Descripcion         : parser_descent_abidos_commands.cpp
    Version             : 0.1
    Autor               : F. Manuel Hevia Garcia
    Fecha creacion      : 30/12/2011
    Fecha modificacion  :

    Observaciones :


------------------------------------------------------------------------------*/
#include <stdlib.h>
#include <string.h>
#include <sys/param.h>

#include "parser_descent.h"
#include "options.h"
#include "tokens.h"
#include "generator_class_diagram.h"
#include "generator_original.h"
#include "trace.h"
#include "lex_yacc.h"

#include "preprocessor_ts.h"
#include "loader.h"
#include "generator_check_include_files.h"
/*----------------------------------------------------------------------------*/
int c_parser_descent::check_abidos_command(int t)
{
  switch(t){
    case ABIDOS_BAN_SYMBOLS_ON:
      ban_symbols_on();
      return 1;
      break;

    case ABIDOS_BAN_SYMBOLS_OFF:
      ban_symbols_off();
      return 1;
      break;

    default:
      return 0;
  }

  return 0;
}
/*----------------------------------------------------------------------------*/
/*
  while options.ban_symbols_on == 1
  symbols will be saved in ts with is_banned = 1
*/
void c_parser_descent::ban_symbols_on(void)
{
  options.ban_symbols_on = 1;
}
/*----------------------------------------------------------------------------*/
void c_parser_descent::ban_symbols_off(void)
{
  options.ban_symbols_on = 0;
}
/*----------------------------------------------------------------------------*/

