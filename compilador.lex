%{
#ifndef YYSTYPE
#define YYSTYPE long int
#endif
#include <stdlib.h>
void yyerror(char *);
#include "y.tab.h"
%}

letra	[a-z|A-Z|_] 
digito	[0-9]
identificador	{letra}({letra}|{digito})*

%%

programa	{ return PROGRAMA; }

{digito}+	{ 
				yylval = (long) strdup(yytext);
				return NUM_INT;
			}

{digito}+\.{digito}+ {
				yylval = (long) strdup(yytext);
				return NUM_REAL;
			}

inteiro		{ 
				yylval = INT; 
				return TIPO;
			}

real		{ 
				yylval = REAL;
				return TIPO;
			}

vazio		{ 
				yylval = VAZIO;
				return TIPO;
			}

retorne		{ return RETORNE; }

ou			{ return OU; }

e			{ return E; }

nao			{ return NAO; }

se			{ return SE; }

senao		{ return SENAO; }

enquanto	{ return ENQUANTO; }

funcao		{ return FUNCAO; }

escreva		{ return ESCREVA; }

leia		{ return LEIA; }

cadeia		{ return CADEIA; }

verdadeiro	{ 
				yylval = VERDADEIRO;
				return BOOLEANO;
			}

falso		{ 
				yylval = FALSO;
				return BOOLEANO;
			}

>=			{ return MAIOR_IGUAL; }

\<=			{ return MENOR_IGUAL; }

!=			{ return DIFERENTE; }

==			{ return IGUAL_COMP; }

{identificador}	{
				yylval = (long) strdup(yytext);
				return ID;
			}


[-+*/%<>=(){},\[\]]	{ return *yytext; }




[ \t\n] 	; /* skip whitespace */
. 	yyerror("invalid character");
%%
int yywrap(void) {
return 1;
}
