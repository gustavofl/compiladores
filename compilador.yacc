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

// Constantes que são usadas para construir a arvore sintatica
%token EXPR_LOGICA MAIOR NUMERO MENOR SOMA SUB MULT DIV MOD

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
	| expr 										{ imprimir_pos_ordem((no_arvore *) $1); }
	| exprlogica								{ imprimir_pos_ordem((no_arvore *) $1); }
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
													no_arvore *no = criar_no_expressao(NUMERO, n, NULL);
													((numero *) no->dado.exprlogica->dir)->tipo = INT;
													$$ = (long) no;
												}
	| ID 	%prec REDUCE						{}		// testar se está realmente funcionando...
	| NUM_REAL									{
													char *lexema = (char *) $1;
													numero *n = localizar_numero(&t_numeros, lexema, REAL); 
													if(n == NULL){
														n = criar_numero(lexema, REAL);
														inserir_numero(&t_numeros, n);
													}
													no_arvore *no = criar_no_expressao(NUMERO, n, NULL);
													((numero *) no->dado.exprlogica->dir)->tipo = REAL;
													$$ = (long) no;
												}
	| chamar_funcao								{}
	| decl_array								{}
	| atr_array									{}
	| expr '*' expr								{ $$ = (long) criar_no_expressao(MULT, (void *) $3, (void *) $1); }
	| expr '/' expr								{ $$ = (long) criar_no_expressao(DIV, (void *) $3, (void *) $1); }
	| expr '%' expr								{ $$ = (long) criar_no_expressao(MOD, (void *) $3, (void *) $1); }
	| expr '+' expr								{ $$ = (long) criar_no_expressao(SOMA, (void *) $3, (void *) $1); }
	| expr '-' expr								{ $$ = (long) criar_no_expressao(SUB, (void *) $3, (void *) $1); }
	| '(' expr ')'								{ $$ = $2; }
//	| '-' expr 	%prec '*'						{}		// NAO ESTA FUNCIONANDO O %prec
//	| '+' expr 	%prec '*'						{}		// NAO ESTA FUNCIONANDO O %prec
	;


exprlogica:
	BOOLEANO									{ $$ = (long) criar_no_expressao_logica(BOOLEANO, (void *) $1, NULL); }
	| exprlogica OU exprlogica 					{ $$ = (long) criar_no_expressao_logica(OU, (void *) $3, (void *) $1); }
	| exprlogica E exprlogica 					{ $$ = (long) criar_no_expressao_logica(E, (void *) $3, (void *) $1); }
	| NAO exprlogica 							{ $$ = (long) criar_no_expressao_logica(NAO, (void *) $2, NULL); }
	| expr '>' expr								{ $$ = (long) criar_no_expressao_logica(MAIOR, (void *) $3, (void *) $1); }
	| expr '<' expr								{ $$ = (long) criar_no_expressao_logica(MENOR, (void *) $3, (void *) $1); }
	| expr MAIOR_IGUAL expr						{ $$ = (long) criar_no_expressao_logica(MAIOR_IGUAL, (void *) $3, (void *) $1); }
	| expr MENOR_IGUAL expr						{ $$ = (long) criar_no_expressao_logica(MENOR_IGUAL, (void *) $3, (void *) $1); }
	| expr IGUAL_COMP expr						{ $$ = (long) criar_no_expressao_logica(IGUAL_COMP, (void *) $3, (void *) $1); }
	| expr DIFERENTE expr						{ $$ = (long) criar_no_expressao_logica(DIFERENTE, (void *) $3, (void *) $1); }
	| exprlogica IGUAL_COMP exprlogica			{ $$ = (long) criar_no_expressao_logica(IGUAL_COMP, (void *) $3, (void *) $1); }
	| exprlogica DIFERENTE exprlogica			{ $$ = (long) criar_no_expressao_logica(DIFERENTE, (void *) $3, (void *) $1); }
	| '(' exprlogica ')'						{ $$ = $2; }
	;

decl_array:
	TIPO ID '[' NUM_INT ']'										{}
	|  TIPO ID '[' NUM_INT ']' '=' '{' lista_argumentos '}'		{}	
	|  TIPO ID '[' ']' '=' '{' lista_argumentos '}'				{}
	;

atr_array:
	ID '[' NUM_INT ']' '=' NUM_INT				{}
	| ID '[' NUM_INT ']' '=' NUM_REAL			{}

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
	printf("\n");
	return 0;
}
