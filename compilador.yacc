%{
#ifndef YYSTYPE
#define YYSTYPE long int
#endif
#include <stdio.h>
#include "tabela.h"
#include "arvore.h"

int yylex(void);
void yyerror(char *);

pilha_contexto *pilha;

%}

%token PROGRAMA TIPO VAZIO INT REAL NUM_INT NUM_REAL ID EXPR ATTR OU E NAO SE SENAO ENQUANTO FUNCAO ESCREVA LEIA CADEIA MAIOR_IGUAL MENOR_IGUAL DIFERENTE IGUAL_COMP VERDADEIRO FALSO BOOLEANO
%left OU
%left E
%left DIFERENTE IGUAL_COMP
%left '<' '>' MAIOR_IGUAL MENOR_IGUAL
%left NAO                                    // O portugol studio avalia o nao antes dos operadores logicos
%left '+' '-'
%left '*' '/' '%'
%%

program:
	PROGRAMA '{' corpo_programa '}'				{}
	;

corpo_programa:
	corpo_programa componente_programa			{}
	|
	;

componente_programa:
	decl										{}
	| funcao 									{}
	;

decl:
	TIPO lista_var								{}
	;

lista_var:
	lista_var ',' variavel 						{}
	| variavel 									{}
	;

variavel:
	atribuicao									{}
	| ID 										{}
	;

atribuicao:
	ID '=' NUM_INT 								{}
	;

funcao:
	FUNCAO tipo_retorno ID 
	'(' lista_parametros_vazio ')' 
	bloco										{}
	;

lista_parametros_vazio:
	lista_parametros 							{}
	|
	;

lista_parametros:
	lista_parametros ',' TIPO ID 				{}
	| TIPO ID 									{}
	;

tipo_retorno:
	TIPO 										{}
	|											{}
	;

bloco:
	'{' stmts '}'								{}
	;

stmts:
	stmts stmt 									{}
	|
	;

stmt:
	decl
	| atribuicao
	;
%%

void yyerror(char *s) {
	fprintf(stderr, "%s\n", s);
}

int main(void) {
	pilha = NULL;
	yyparse();
	return 0;
}
