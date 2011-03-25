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

void trace(string & tab, string s)
{
  tab = tab + TAB;
  printf("%s%s\n",tab.c_str(),s.c_str());
}
/*----------------------------------------------------------------------------*/
c_context_tokens::c_context_tokens(c_context context_param)
{
    context = context_param;
}
/*----------------------------------------------------------------------------*/
void c_context_tokens::save(c_context context_param)
{
    context = context_param;
}
/*----------------------------------------------------------------------------*/
c_context c_context_tokens::restore(void)
{
/*
    if( 0 == context.i_token)
    {
      context.just_reloaded = 1;
    }
*/
	return context;
}
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
c_parser_descent::c_parser_descent()
{
	context.just_reloaded  = 1;
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
//	t_tokens::iterator i_token = tokens_vector.begin();

	unsigned n = 0;	

	printf("tokens_vector_print\n");
	printf("{\n");

	while( n < tokens_vector.size() )
	{
		c_token token;
		token = tokens_vector[n];

		printf(" [%3d] -> yytokens[%s] token.text[%s]\n"
			, token.id
			, yytokens[token.id]
			, token.text.c_str()
	  	);

		++n;
	}

	printf("}\n");
}
/*----------------------------------------------------------------------------*/
void c_parser_descent::tokens_vector_print_from_actual(void)
{
	t_tokens::iterator i_token = tokens_vector.begin() + context.i_token;

	printf("tokens_vector_print_from_actual\n");
	printf("{\n");

	while( i_token != tokens_vector.end() )
	{
		c_token token;
		token = (*i_token);

		printf(" [%3d] -> yytokens[%s] token.text[%s]\n"
			, token.id
			, yytokens[token.id]
			, token.text.c_str()
	  	);

		++i_token;
	}

	printf("}\n");
}
/*----------------------------------------------------------------------------*/
void c_parser_descent::token_print(void)
{
	if( tokens_vector.empty() )
	{
		printf("error c_parser_descent::token_print() -> tokens_vector.empty() \n");
		exit( -1 );
	}

	if( !( (0 <= context.i_token) &&
		   (context.i_token < tokens_vector.size())
		 )
	  )
	{
		printf("error c_parser_descent::token_print() -> (context.i_token out of vector) \n");
		exit( -1 );
	}

  printf(" line[%d] yy_actual=[%3d] "
	, yylineno , token_get()
  );
  printf("-> yytokens[%s] "
	, yytokens[token_get()]
  );
  printf(" token.text[%s]\n"
	, tokens_vector[context.i_token].text.c_str()
  );
}
/*----------------------------------------------------------------------------*/
void c_parser_descent::tokens_vector_reload(void)
{
	context.clear();
}
/*----------------------------------------------------------------------------*/
void c_parser_descent::tokens_vector_clear(void)
{
	tokens_vector.clear();
	
	if( !tokens_vector.empty() )
	{
		printf("########################## tokens_vector_clear COMOR  !tokens_vector.empty() !!! \n");
	}

    context.clear();
}
/*----------------------------------------------------------------------------*/
int c_parser_descent::token_get(void)
{
	if( tokens_vector.empty() )
	{
		printf("error c_parser_descent::token_get() -> tokens_vector.empty() \n");
		exit( -1 );
	}

	if( !( (0 <= context.i_token) &&
		   (context.i_token < tokens_vector.size())
		 )
	  )
	{
		printf("error c_parser_descent::token_get() -> (context.i_token out of vector) \n");
//		exit( -1 );
        context.clear();
	}

	return  tokens_vector[context.i_token].id;
}
/*----------------------------------------------------------------------------*/
c_token c_parser_descent::c_token_get(void)
{
    if( tokens_vector.empty() )
    {
        printf("error c_parser_descent::c_token_get() -> tokens_vector.empty() \n");
        exit( -1 );
    }

    if( !( (0 <= context.i_token) &&
           (context.i_token < tokens_vector.size())
         )
      )
    {
        printf("error c_parser_descent::c_token_get() -> (context.i_token out of vector) \n");
//      exit( -1 );
        context.clear();
    }

    return  tokens_vector[context.i_token];
}

/*----------------------------------------------------------------------------*/
/*
void c_parser_descent::token_previous(void)
{
	if( tokens_vector.empty() )
	{
		context.i_token = tokens_vector.begin();
		return;
	}

	if( context.i_token > tokens_vector.begin() )
	{
		--context.i_token;
		return;
	}
}
*/
/*----------------------------------------------------------------------------*/
void c_parser_descent::token_next(string tab)
{
	int t = 0;
	int get_from_lex = 0;

    printf("%s## token_next",tab.c_str());
    
	if( !( (0 <= context.i_token) &&
		   (context.i_token < tokens_vector.size())
		 )
	  )
	{
		printf(" c_parser_descent::token_next() -> (context.i_token out of vector)");
        context.clear();
	}


	if( tokens_vector.empty() )
	{
		get_from_lex = 1;
	}
	else
	{
		if( context.i_token < tokens_vector.size() )
		{
			if( 1 == context.just_reloaded )
			{
				context.just_reloaded = 0;
                printf("%s## just_reloaded == 1\n",tab.c_str());
				return;
			}

			if( context.i_token < ( tokens_vector.size() - 1 ) )
			{
				++context.i_token;
				token_print();

				return;
			}
		}

		get_from_lex = 1;
	}

	if( 1 == get_from_lex )
	{
		context.just_reloaded = 0;

		t = yylex();
        c_token token(t, yytext);
        /*
          i tried put this part in lex but did not compile
          maybe for the link C mode of lexical module
        */
        if( IDENTIFIER == t )
        {
          c_symbol * p_symbol = ts.search_symbol(yytext);
          if( p_symbol )
          {
            if(p_symbol->type != 0)
            {
              //return symbol.type;
              printf("%s## next_token found symbol [%s]",tab.c_str(),yytext);
              token.id = p_symbol->type;
            }
          }
        }

		tokens_vector.push_back(token);

		context.i_token = (tokens_vector.size() - 1);

		token_print();

		return;
	}

	printf("error c_parser_descent::token_next(void)\n");
	exit(-1);
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
  ts.set();
  translation_unit();

  printf("\n##---------------- rest of tokens has not been consumed -------------------------------\n");

  do
  {
//	tokens_vector_clear();
	token_next("");
  }
  while( token_get() != 0 );
  ts.print();
  ts.unset();

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
  token_next("");
  while( token_get() != 0 )
  {
    token_next("");
  }

  printf("reload\n");
  tokens_vector_reload();
  token_next("");
  while( token_get() != 0 )
  {
    token_next("");
  }

  tokens_vector_print();
  yylex_destroy();  

  printf("}\n");

  return 0;
}
/*----------------------------------------------------------------------------*/
