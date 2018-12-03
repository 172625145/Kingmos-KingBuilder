%{
/* lexical analyser
*/

#include <string.h>
//#include "build.tab.hpp"
#include "build.tab.cpp.h"

%}

space			[ \t]+
digital			[0-9]
alpha			[a-zA-Z]

word			({alpha})({alpha}|{digital}|_|".")*

%option noyywrap

%%

"#" { 
		char c;

		while ( (c = yyinput()) != '\n' && c != EOF )
			/*do null*/;
		return TOKEN_NEWLINE;
	}


{word}	{
			ambolval.lpstr = strdup (yytext);

			return TOKEN_VALUE;
		}

"\\\n" { }
"\\"{space}"\n" { }


"="		{ 
			return TOKEN_EQUAL;
		}

"\n"	{
			return TOKEN_NEWLINE;
		}

.		{
		}

%%
