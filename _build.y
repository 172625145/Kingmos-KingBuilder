%{

#include "stdafx.h"

/*bison 1.25 bug!!, 1.35 ok*/
/*define MSDOS for VC++ happy*/
//#define MSDOS

int amboerror(char *s);

extern int ambolex(void);

#define CLASSPARSER(param)		((CMapStringToString *)param)

#define YYPARSE_PARAM ambParam

/* MSVC bug!!, must define this for MSVC happy */
#define yyerror amboerror
#define yylex ambolex
#define yyparse amboparse
#define yylval ambolval

struct words {
	int len;
	char *lpstr;
};

%}


%union {
unsigned int	uival;
char			*lpstr;
struct words	*lpwds;
}

/*+++++++++++Parse Assistant++++++++++++*/
/* newline */
%token <uival>	TOKEN_NEWLINE

/* = */
%token <uival>	TOKEN_EQUAL

/* word */
%token <lpstr>	TOKEN_VALUE

/* nothing or char we don't want */
%token <uival>	TOKEN_NULL
/*-----------Parse Assistant------------*/

%type <lpstr>	words
%type <lpwds>	word

%%

input	:
		| input word
		| input words
		| input express
		;

word	: word TOKEN_VALUE {
			int len = strlen ($2)+2;

			char *lpstr = $$->lpstr;
			$$->lpstr = (char *)realloc ($$->lpstr, $$->len+len);
			if ($$->lpstr == NULL)
			{
				printf ("no memory\n");
				return -1;
			}

			$$->len += len;
			strcat ($$->lpstr, " ");
			strcat ($$->lpstr, $2);

			free ($2);
		}
		| TOKEN_VALUE {
			$$ = (struct words *)malloc(sizeof(struct words));
			if ($$ == NULL)
			{
				printf ("no memory\n");
				return -1;
			}
			$$->len = strlen ($1);
			$$->lpstr = $1;
		}
		;

words	: word TOKEN_NEWLINE {
			$$ = $1->lpstr;

			free ($1);
		}
		;

express : TOKEN_VALUE TOKEN_EQUAL words {

//			CLASSPARSER(ambParam)->SetAt($1, $3);

			/* for unicode safe */
			CString	csKey ($1), csVal ($3);

			CLASSPARSER(ambParam)->SetAt(csKey, csVal);

			free ($1);
			free ($3);
		}
		| TOKEN_VALUE TOKEN_EQUAL TOKEN_NEWLINE {
		}
		| TOKEN_NEWLINE {
		}
		;

%%

int amboerror(char *s)
{
	return -1;
}

