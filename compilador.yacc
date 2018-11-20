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

%token PROGRAMA TIPO VAZIO INT REAL NUM_INT NUM_REAL ID EXPR ATTR OU E NAO SE SENAO ENQUANTO FUNCAO ESCREVA LEIA CADEIA PULALINHA MAIOR_IGUAL
%left '+' '-'
%left '*' '/' '%'
%%

program:
	PROGRAMA 
	PULALINHA '{'
	PULALINHA funcoes			{}
	'}'
	PULALINHA					
	;

decls:
	decls decl					{}
	|
	;
	
decl:
	TIPO ID PULALINHA			{}
	;

funcoes:
	funcoes funcao 				{}
	|
	;

funcao:
	FUNCAO TIPO ID 				{}
	'(' listaparams ')' 
	PULALINHA bloco				{}

bloco:
	'{'	
	PULALINHA stmts
	PULALINHA '}'
	PULALINHA					{}
	;

listaparams:
	TIPO ID 					{}
	|
	;

stmts: 
	stmts stmt
	| 	
	;

stmt:
	NUM_INT 					{}
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
