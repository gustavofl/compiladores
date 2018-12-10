%{
#ifndef YYSTYPE
#define YYSTYPE long int
#endif
#include <stdio.h>
#include <stdlib.h>
#include "tabela.h"
#include "arvore.h"

int yylex(void);
void yyerror(char *);

pilha_contexto *pilha;
tabela_numero t_numeros;
fila_buffer fila;

// variavel para auxiliar na declaracao multiplas de variavel
// long tipo_attr_multiplas = 0;

%}

%token PROGRAMA TIPO VAZIO INT REAL NUM_INT NUM_REAL ID EXPR ATTR OU E NAO SE SENAO ENQUANTO FUNCAO ESCREVA LEIA CADEIA MAIOR_IGUAL MENOR_IGUAL DIFERENTE IGUAL_COMP VERDADEIRO FALSO BOOLEANO

// Constantes que são usadas para construir a arvore sintatica
%token EXPR_LOGICA MAIOR NUMERO MENOR SOMA SUB MULT DIV MOD NO_ARVORE NULO LISTA_ATTR LISTA_ARG

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
%nonassoc '['
%%

program:
	PROGRAMA '{' 								{ pilha = empilhar_contexto(pilha, criar_contexto(topo_pilha(pilha))); }
	corpo_programa 								{}
	'}'											{ /* imprimir_contexto(topo_pilha(pilha)); */ desempilhar_contexto(&pilha); }
	;

corpo_programa:
	corpo_programa componente_programa			{}
	|
	;

componente_programa:
	decl										{ imprimir_pos_ordem((no_arvore *) $1); }
	| funcao 									{}
	;

decl:
	TIPO lista_var								{
													while(fila.primeiro != NULL){
														buffer *b = pop_buffer(&fila);
														if(b->tipo == ID)
															((simbolo *) b->dado)->tipo = $1;
														else if(b->tipo == NO_ARVORE)
															((simbolo *) ((no_arvore *) b->dado)->dado.attr->resultado)->tipo = $1;
													}

													no_arvore *no = criar_no_lista_attr((lista_attr *) $2);
													$$ = (long) no;
												}
	;

lista_var:
	lista_var ',' variavel 						{
													lista_attr *lista = lista_atribuicao_add((lista_attr *) $1, (lista_attr *) $3);
													$$ = (long) lista;
												}
	| variavel 									{ $$ = $1; }
	;

variavel:
	atribuicao									{
													no_arvore *no = (no_arvore *) $1;
													simbolo *novo = no->dado.attr->resultado;
													simbolo *busca = localizar_simbolo_contexto(topo_pilha(pilha), novo->lexema);
													if(busca != NULL){
														yyerror("Multiplas declaracoes de variavel.");
														exit(0);
													}
													inserir_simbolo(topo_pilha(pilha), novo);
													add_buffer(&fila, NO_ARVORE, (void *) $1);
													
													lista_attr *lista = criar_lista_atribuicao(no);
													$$ = (long) lista;
												}
	| ID 										{
													simbolo *s = localizar_simbolo_contexto(topo_pilha(pilha), (char *) $1);
													if(s != NULL){
														yyerror("Multiplas declaracoes de variavel.");
														exit(0);
													}
													s = criar_simbolo((char *) $1, 0);
													inserir_simbolo(topo_pilha(pilha), s);
													add_buffer(&fila, ID, s);

													$$ = NULO;
												}
	;

atribuicao:
	ID '=' expr 								{
													simbolo *s = criar_simbolo((char *) $1, 0);
													$$ = (long) criar_no_atribuicao(s, (void *) $3);
												}
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
	'{' 										{ pilha = empilhar_contexto(pilha, criar_contexto(topo_pilha(pilha))); }
	stmts 										{}
	'}'											{ /* imprimir_contexto(topo_pilha(pilha)); */ desempilhar_contexto(&pilha); }
	;

bloco:
	bloco_composto								{}
	| 											{ pilha = empilhar_contexto(pilha, criar_contexto(topo_pilha(pilha))); } 		
		stmt 		
												{ /* imprimir_contexto(topo_pilha(pilha)); */ desempilhar_contexto(&pilha); }
	;

stmts:
	stmts stmt 									{ /* imprimir_pos_ordem((no_arvore *) $2); */ }
	|
	;

stmt:
	decl										{ imprimir_pos_ordem((no_arvore *) $1); }
	| atribuicao								{
													no_arvore *no = (no_arvore *) $1;
													simbolo *novo = no->dado.attr->resultado;
													simbolo *busca = localizar_simbolo(topo_pilha(pilha), novo->lexema);
													if(busca == NULL){
														yyerror("Variavel nao declarada.");
														exit(0);
													}
													imprimir_pos_ordem((no_arvore *) $1); 
												}
	| decl_array								{}
	| atr_array									{}
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
													no->dado.expr->tipo = INT;
													$$ = (long) no;
												}
	| ID 	%prec REDUCE						{ 
													simbolo *s = localizar_simbolo(topo_pilha(pilha), (char *) $1);
													if(s == NULL){
														yyerror("Variavel nao declarada.");
														exit(0);
													}
													no_arvore *no = criar_no_expressao(ID, s, NULL);
													no->dado.expr->tipo = s->tipo;
													$$ = (long) no;
												}
	| NUM_REAL									{
													char *lexema = (char *) $1;
													numero *n = localizar_numero(&t_numeros, lexema, REAL); 
													if(n == NULL){
														n = criar_numero(lexema, REAL);
														inserir_numero(&t_numeros, n);
													}
													no_arvore *no = criar_no_expressao(NUMERO, n, NULL);
													no->dado.expr->tipo = REAL;
													$$ = (long) no;
												}
	| chamar_funcao								{ $$ = $1; }
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
	ID '[' NUM_INT ']' '=' NUM_INT								{}
	| ID '[' NUM_INT ']' '=' NUM_REAL							{}

leia:
	LEIA 
	'(' 
	ID 
	')'											{}
	;

escreva:
	ESCREVA 
	'(' 
	lista_argumentos_vazio 
	')'											{}
	;

chamar_funcao:
	ID
	'(' 
	lista_argumentos_vazio 
	')'											{ $$ = (long) criar_no_funcao((void *) localizar_simbolo(topo_pilha(pilha), (char *) $1), (void *) $3); }
	;

lista_argumentos_vazio:
	lista_argumentos 							{ $$ = $1; }
	|											{ $$ = (long) NULL; }
	;

lista_argumentos:
	lista_argumentos ',' expr 					{ $$ = (long) criar_no_lista_arg((void *) $3, (void *) $1); }
	| expr 										{ $$ = (long) criar_no_lista_arg((void *) $1, NULL); }
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
