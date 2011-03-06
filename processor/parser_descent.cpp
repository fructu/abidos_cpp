/*------------------------------------------------------------------------------

    Proyecto            : show_includes
    Codigo              : parser_descent.cpp
    Descripcion         :
    Version             : 0.1
    Autor               : F. Manuel Hevia Garcia
    Fecha creacion      : 03/03/2011
    Fecha modificacion  :

    Observaciones :


------------------------------------------------------------------------------*/
#include <stdlib.h>
#include "parser_descent.h"
#include "tokens.h"
/*----------------------------------------------------------------------------*/
c_parser_descent::c_parser_descent()
{
	just_reloaded  = 1;
	i_token_actual = tokens_vector.begin();
}
/*----------------------------------------------------------------------------*/
c_parser_descent::~c_parser_descent()
{
}
/*----------------------------------------------------------------------------*/
void yytname_print(void)
{
  int i = 0;
  while(yytokens[i] != 0)
  {
     printf(" i=[%d] -> [%s]\n", i, yytokens[i]);
     i++;
  }
};
/*----------------------------------------------------------------------------*/
void c_parser_descent::tokens_vector_print(void)
{
	t_tokens::iterator i_token = tokens_vector.begin();

	printf("tokens_vector_print\n");
	printf("{\n");

	while( i_token != tokens_vector.end() )
	{
		c_token token;
		token = (*i_token);

		printf(" yy_actual=[%3d] -> yytokens[%s] token.text[%s]\n"
			, token.token
			, yytokens[token.token]
			, token.text.c_str()
	  	);

		++i_token;
	}

	printf("}\n");
}
/*----------------------------------------------------------------------------*/
void c_parser_descent::token_print(void)
{
  printf(" yy_actual=[%3d] -> yytokens[%s] token.text[%s]\n"
	, token_get()
	, yytokens[token_get()]
	, (*i_token_actual).text.c_str()
  );
}
/*----------------------------------------------------------------------------*/
void c_parser_descent::tokens_vector_reload(void)
{
	just_reloaded = 1;
	i_token_actual = tokens_vector.begin();
}
/*----------------------------------------------------------------------------*/
void c_parser_descent::tokens_vector_clear(void)
{
	tokens_vector.clear();
	just_reloaded = 1;
	i_token_actual = tokens_vector.begin();
}
/*----------------------------------------------------------------------------*/
int c_parser_descent::token_get(void)
{
	if( tokens_vector.empty() )
	{
		printf("error c_parser_descent::token_get() -> tokens_vector.empty() \n");
		exit( -1 );
	}

	return (*i_token_actual).token;
}
/*----------------------------------------------------------------------------*/
void c_parser_descent::token_next(void)
{
  	int t = 0;
	int get_from_lex = 0;

	if( tokens_vector.empty() )
	{
		get_from_lex = 1;
	}
	else
	{
		if( i_token_actual < tokens_vector.end() )
		{
			if( 1 == just_reloaded )
			{
				just_reloaded = 0;
				token_print();

				return;
			}

			if( i_token_actual < ( tokens_vector.end() - 1 ) )
			{
				++i_token_actual;
				token_print();

				return;
			}
		}

		get_from_lex = 1;
	}

	if( 1 == get_from_lex )
	{
		just_reloaded = 0;

		t = yylex();
		c_token token(t, yytext);
		tokens_vector.push_back(token);

		i_token_actual = tokens_vector.end();
		--i_token_actual;

		token_print();
	}
}
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
void c_parser_descent::yyparse(char * file_name)
{
  if (!(yyin=fopen(file_name,"r")))
  {
    printf("\nERROR: yyparse did not can open [%s]\n",file_name);
    return;
  }

  translation_unit();

  yylex_destroy();  

}
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
int c_parser_descent::test_01(void)
{
  char file_name[] = "test/t1.cpp";

  printf("c_parser_descent::test_01()\n");
  printf("{\n");

  if (!(yyin=fopen(file_name,"r")))
  {
    printf("\nERROR: yyparse did not can open [%s]\n",file_name);
    return 0;
  }

  //yytname_print();
  tokens_vector_reload();
  token_next();
  while( token_get() != 0 )
  {
    token_next();
  }

  printf("reload\n");
  tokens_vector_reload();
  token_next();
  while( token_get() != 0 )
  {
    token_next();
  }

  tokens_vector_print();
  yylex_destroy();  

  printf("}\n");

  return 0;
}
/*----------------------------------------------------------------------------*/
