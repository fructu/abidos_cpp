/*------------------------------------------------------------------------------

    Proyecto            : ClientSegmentation
    Codigo              : includes_lex_yacc.h
    Descripcion         :
    Version             : 0.1
    Autor               : F. Manuel Hevia Garcia
    Fecha creacion      : 30/12/2010
    Fecha modificacion  :

    Observaciones :


------------------------------------------------------------------------------*/
#ifndef includes_lex_yacc_h
#define includes_lex_yacc_h
#ifdef __cplusplus
extern "C" {
#include "parser.h"
#include <stdio.h>
#define ID_MAX_LEN (8000)

    extern int yylineno;
    extern FILE *yyin;

    /*
     * extern YYSTYPE yylval;
     */
    extern char lex_file_name[];

    int yylex(void);

    int lex_file_init(const char *f);
    int lex_file_push(const char *f);
    int lex_file_pop(void);

    /*
     * void yyerror (char *s);
     */
    /*
     * int yyparse();
     */

    extern char *yytext;

    int lex_stack_size(void);
    int lex_file_init(const char *f);
    int lex_file_push(const char *f);
    int lex_file_pop(void);
    void lex_file_end(void);
    int yylex_destroy(void);

    typedef union {
        int number;
        char iden[ID_MAX_LEN];
    } YYLVAL;

    /*
      lex must eat the rest of the line
      #define D 1
      ## todo: for the moment 1 is not parsing...
    */
    void skip_until_eol(void);
    /*
      designed to pass this things
       #define hi \
          bye
    */
    void skip_until_eol_preprocessor(void);
    void get_string_between_sharps(char * str);
}
#endif
#endif
/*----------------------------------------------------------------------------*/
