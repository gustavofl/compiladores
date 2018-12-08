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
tabela_numero t_numeros;

%}

%token PROGRAMA TIPO VAZIO INT REAL NUM_INT NUM_REAL ID EXPR ATTR OU E NAO SE SENAO ENQUANTO FUNCAO ESCREVA LEIA CADEIA MAIOR_IGUAL MENOR_IGUAL DIFERENTE IGUAL_COMP VERDADEIRO FALSO BOOLEANO

%left OU
%left E
%left DIFERENTE IGUAL_COMP
%left '<' '>' MAIOR_IGUAL MENOR_IGUAL
%left NAO                                    // O portugol studio avalia o nao antes dos operadores logicos
%left '+' '-'
%left '*' '/' '%'

%nonassoc REDUCE
%nonassoc '('
%nonassoc SENAO
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
	ID '=' expr 								{}
	;

funcao:
	FUNCAO tipo_retorno ID 
	'(' 
	lista_parametros_vazio 
	')' 
	bloco_composto								{}
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

bloco_composto:
	'{' stmts '}'								{}
	;

bloco:
	bloco_composto								{}
	| stmt 										{}
	;

stmts:
	stmts stmt 									{}
	|
	;

stmt:
	decl										{}
	| atribuicao								{}
	| expr 										{}
	| exprlogica								{}
	| leia 										{}
	| escreva 									{}
	| se_senao 									{}
	| enquanto 									{}
	;

expr:
	NUM_INT										{
													char *lexema = (char *) $1;
													numero *n = localizar_numero(&t_numeros, lexema, INT); 
													if(n == NULL){
														n = criar_numero(lexema, INT);
														inserir_numero(&t_numeros, n);
													}
													$$ = (long) n;
												}
	| ID 	%prec REDUCE						{}		// testar se está realmente funcionando...
	| NUM_REAL									{
													char *lexema = (char *) $1;
													numero *n = localizar_numero(&t_numeros, lexema, REAL); 
													if(n == NULL){
														n = criar_numero(lexema, REAL);
														inserir_numero(&t_numeros, n);
													}
													$$ = (long) n;
												}
	| chamar_funcao								{}
	| decl_array								{}
	| expr '*' expr								{}
	| expr '/' expr								{}
	| expr '%' expr								{}
	| expr '+' expr								{}
	| expr '-' expr								{}
	| '(' expr ')'								{}
	;

exprlogica:
	BOOLEANO									{}
	| exprlogica OU exprlogica 					{}
	| exprlogica E exprlogica 					{}
	| NAO exprlogica 							{}
	| expr '>' expr								{}
	| expr '<' expr								{}
	| expr MAIOR_IGUAL expr						{}
	| expr MENOR_IGUAL expr						{}
	| expr IGUAL_COMP expr						{}
	| expr DIFERENTE expr						{}
	;

decl_array:
	TIPO ID '[' NUM_INT ']'										{}
	|  TIPO ID '[' NUM_INT ']' '=' '{' lista_argumentos '}'		{}	
	|  TIPO ID '[' ']' '=' '{' lista_argumentos '}'				{}
	;

leia:
	LEIA 
	'(' 
	ID 
	')'											{}
	;

escreva:
	ESCREVA 
	'(' 
	lista_argumentos 
	')'											{}
	;

chamar_funcao:
	ID
	'(' 
	lista_argumentos 
	')'											{}
	;

lista_argumentos:
	lista_argumentos ',' expr 					{}
	| expr 										{}
	;

se_senao:
	SE '(' exprlogica ')' bloco %prec REDUCE	{}
	| SE '(' exprlogica ')' bloco SENAO bloco	{}
	;

enquanto:
	ENQUANTO '(' exprlogica ')' bloco			{}
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
