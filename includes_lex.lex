/*------------------------------------------------------------------------------

	Proyecto			: show_includes
	Codigo				: includes_lex.lex
	Descripcion			: 
	Version				: 0.1
	Autor				: F. Manuel Hevia Garcia
	Fecha creacion		: 30/12/2010
	Fecha modificacion	: 

	Observaciones :

			
------------------------------------------------------------------------------*/
%{
#include <ctype.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"includes_lex_yacc.h"    
    
//int yylineno = 1;
char lex_file_name[1024];

static void skip_comment(void);
static void skip_until_eol(void);static void skip_until_eol(void);
%}
/*
	if i want to continue scaning other file
	i must define yywrap and drop the next line
*/
%option noyywrap
%option yylineno 
stringtext				([^"])|(\\.)
stringtext_2			([^"<>])|(\\.)
%%
"/*" { 
		skip_comment(); 
	}

"//" {
		skip_until_eol(); 
	}

\r\n {
//		yylineno++;
		//return '\n';
	}
\n {
//		yylineno++;
		//return '\n';
	}

[\t\f\v\r ]+ { /* Ignore whitespace. */ 
	}

[#] {
		return '#';
	}

(include) {
		return TOKEN_INCLUDE;
	}

"\""{stringtext}*"\"" { 

//		strcpy(yylval.id,yytext);
		strncpy(yylval.id, yytext+1, strlen(yytext) -2 );

		printf(" ### lex yytext[%s] -> [%s]", yytext, yylval.id);
		
		return STRING_COMMILLAS; 
	}

"L\""{stringtext}*"\"" {
		strncpy(yylval.id, yytext+1, strlen(yytext) -2 );
		return STRING_COMMILLAS; 
	}

"\<"{stringtext_2}*"\>" { 
		printf(" ### lex  <> yytext[%s] -> [%s]", yytext, yylval.id);
		strncpy(yylval.id, yytext+1, strlen(yytext) -2 );
		return STRING_SHARP; 
	}

. { 
	/*fprintf(stderr, "%d: unexpected `%s'\n", yylineno, yytext); */
	}
%%

int yyerror(const char *msg) 
{
  printf("%d: %s at '%s'\n",yylineno, msg, yytext);
  return -1;
}

/*
 * We use this routine instead a lex pattern because we don't need
 * to save the matched comment in the `yytext' buffer.
 */
static void
skip_comment(void)
{
	int c1, c2;

	c1 = yyinput();
	c2 = yyinput();

	while (c2 != EOF && !(c1 == '*' && c2 == '/')) {
		if (c1 == '\n')
			++yylineno;
		c1 = c2;
		c2 = yyinput();
	}
}

/*
 * See the previous comment for an explanation.
 */
static void
skip_until_eol(void)
{
	int c;

	while ((c = yyinput()) != EOF && c != '\n')
		;
	++yylineno;
}


void lex_file_init(char *f)
{
	sprintf(lex_file_name,"%s",f);

	yyin=fopen(f,"r");
	if( NULL==yyin )
	{
		printf("  error open [%s]\n",f);
	}

	yylineno=1;
}

