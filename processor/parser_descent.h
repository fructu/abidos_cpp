/*------------------------------------------------------------------------------

    Proyecto            : abidos
    Codigo              : parser_descent.h
    Descripcion         :
    Version             : 0.1
    Autor               : F. Manuel Hevia Garcia
    Fecha creacion      : 03/03/2011
    Fecha modificacion  :

    Observaciones :


------------------------------------------------------------------------------*/
#ifndef parser_descent_h
#define parser_descent_h

#include "lex_yacc.h"

class c_parser_descent
{
  private:
    int yy_actual;
    
    void token_print(void);
    int token_get(void);
    void token_next(void);
/*
	rules from yacc
*/
	//Translation unit.
	//this is the start rule in yacc
	int translation_unit(void);

 	//Epsilon (optional) definitions.
	int declaration_seq_opt(void);

	//declarations
	int declaration_seq(void);
	int declaration(void);
	int block_declaration(void);
	int simple_declaration(void);
	int decl_specifier_seq(void);
	int decl_specifier(void);
	int type_specifier(void);
	int class_specifier(void);
	int class_head(void);
	int class_name(void);

  public:    
    c_parser_descent();
    ~c_parser_descent();
    void yyparse(char * file_name);
};

#endif
