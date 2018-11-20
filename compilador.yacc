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

%token PROGRAMA TIPO VAZIO INT REAL NUM_INT NUM_REAL ID EXPR ATTR OU E NAO SE SENAO ENQUANTO FUNCAO ESCREVA LEIA CADEIA PULALINHA MAIOR_IGUAL MENOR_IGUAL DIFERENTE IGUAL_COMP VERDADEIRO FALSO BOOLEANO
%left OU E
%left NAO
%left '<' '>' MAIOR_IGUAL MENOR_IGUAL DIFERENTE IGUAL_COMP
%left '+' '-'
%left '*' '/' '%'
%%

program:
	PROGRAMA 
	PULALINHA '{'
	PULALINHA decls
	funcoes						
	'}'
	PULALINHA					{}			
	;

decls:
	decls decl PULALINHA		{}
	|
	;
	
decl:
	TIPO ID 					{}
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
	'}'
	PULALINHA					{}
	;

listaparams:
	TIPO ID paramadicional		{}
	|
	;

paramadicional:
	paramadicional ',' TIPO ID 	{}
	|
	;

stmts: 
	stmts stmt
	| 	
	;

stmt:
	attr PULALINHA				{}
	| decl PULALINHA			{}
	| expr PULALINHA			{}
	| exprlogica PULALINHA		{}
	| escreva PULALINHA			{}
	| leia PULALINHA			{}
	;

attr:
	ID '=' expr							{}
	;

chamarfuncao:
	ID '(' listaconteudo ')' 			{}
	;

escreva:
	ESCREVA '(' listaconteudo ')' 		{}
	;

listaconteudo:
	conteudo conteudoadicional			{}
	;

conteudoadicional:
	conteudoadicional ',' conteudo 		{}
	|
	;

conteudo:
	expr 						{}
	| exprlogica				{}
	;

leia:
	LEIA '(' ID ')'				{}

expr:
	NUM_INT						{}
	| ID						{}
	| NUM_REAL					{}
	| chamarfuncao				{}
	| expr '*' expr				{}
	| expr '/' expr				{}
	| expr '%' expr				{}
	| expr '+' expr				{}
	| expr '-' expr				{}
	| '(' expr ')'				{}
	;

exprlogica:
	BOOLEANO					{}
	| exprlogica OU exprlogica 	{}
	| exprlogica E exprlogica 	{}
	| NAO exprlogica 			{ printf("OK\n"); }
	| expr '>' expr				{}
	| expr '<' expr				{}
	| expr MAIOR_IGUAL expr		{}
	| expr MENOR_IGUAL expr		{}
	| expr IGUAL_COMP expr		{}
	| expr DIFERENTE expr		{}
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
