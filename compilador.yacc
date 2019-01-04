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

void mostrar_erro_e_parar(char *s, char *var);

no_arvore * buscar_variavel_declarada(char *lexema);
no_arvore * buscar_ou_add_numero(char *lexema, int tipo);
void verificar_simbolo_duplicado(simbolo *s);
void verificar_existencia_lexema(char *lexema);

pilha_contexto *pilha;
tabela_numero t_numeros;
fila_buffer fila;
tabela_lexemas_usados t_lexemas_usados;
tabela t_funcoes;

// variavel para auxiliar na declaracao multiplas de variavel
// long tipo_attr_multiplas = 0;

// int yydebug = 1;
%}

%token PROGRAMA TIPO VAZIO INT REAL NUM_INT NUM_REAL ID EXPR ATTR OU E NAO SE SENAO ENQUANTO FUNCAO ESCREVA LEIA CADEIA MAIOR_IGUAL MENOR_IGUAL DIFERENTE IGUAL_COMP VERDADEIRO FALSO BOOLEANO

// Constantes que são usadas para construir a arvore sintatica
%token EXPR_LOGICA MAIOR NUMERO MENOR SOMA SUB MULT DIV MOD NO_ARVORE NULO LISTA_ATTR LISTA_ARG PARAMETRO LISTA_PARAMETRO CHAMADA_FUNCAO DECL_ARRAY LISTA ATTR_ARRAY INDICE_ARRAY IF_ELSE WHILE

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
	PROGRAMA '{' criar_contexto
	corpo_programa
	'}'	fechar_contexto							{ imprimir_pos_ordem((no_arvore *) $4); }
	;

corpo_programa:
	corpo_programa componente_programa			{ $$ = (long) criar_no_t_lista((void *) $2, (void *) $1); }
	|											{ $$ = (long) NULL; }
	;

componente_programa:
	decl										{ $$ = $1; }
	| funcao 									{ $$ = $1; }
	;

decl:
	TIPO lista_var								{
													while(fila.primeiro != NULL){
														simbolo *s = pop_buffer(&fila);
														s->tipo = $1;
													}

													$$ = $2;
												}
	;

lista_var:
	lista_var ',' variavel 						{ $$ = (long) criar_no_t_lista((void *) $3, (void *) $1); }
	| variavel 									{ $$ = $1; }
	;

variavel:
	atribuicao									{
													no_arvore *no = (no_arvore *) $1;
													simbolo *s = no->dado.attr->resultado;

													verificar_simbolo_duplicado(s);

													add_buffer(&fila, s);
													
													$$ = $1;
												}
	| ID 										{
													simbolo *s = criar_simbolo((char *) $1, 0);

													verificar_simbolo_duplicado(s);

													add_buffer(&fila, s);

													$$ = (long) NULL;
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
	bloco_composto								{
													verificar_existencia_lexema((char *) $3);
													simbolo *s = criar_simbolo((char *) $3, $2);
													inserir_simbolo(&t_funcoes, s);
													$$ = (long) criar_no_funcao($2, s, (void *) $5, (void *) $7);
												}
	;

lista_parametros_vazio:
	lista_parametros 							{ $$ = $1; }
	|											{ $$ = (long) NULL; }
	;

lista_parametros:
	lista_parametros ',' TIPO ID 				{
													no_arvore *no = criar_no_param($3, criar_simbolo ((void *) $4, $3));
													$$ = (long) criar_no_t_lista(no, (void *) $1);
												}
	| TIPO ID 									{
													no_arvore *no = criar_no_param($1, criar_simbolo ((void *) $2, $1));
													$$ = (long) criar_no_t_lista(no, NULL);
												}
	;

tipo_retorno:
	TIPO 										{ $$ = (long) $1; }
	|											{ $$ = (long) NULL; }
	;

bloco_composto:
	'{' criar_contexto
	stmts
	'}' fechar_contexto							{ $$ = $3; }
	;

criar_contexto: 								{ pilha = empilhar_contexto(pilha, criar_contexto(topo_pilha(pilha))); }

fechar_contexto:								{ /* imprimir_contexto(topo_pilha(pilha)); */ desempilhar_contexto(&pilha); }

bloco:
	bloco_composto								{ $$ = $1; }
	| criar_contexto stmt fechar_contexto 		{ $$ = $2; }
	;

stmts:
	stmts stmt 									{ $$ = (long) criar_no_t_lista((void*) $2, (void *) $1);}
	|											{ $$ = (long) NULL; }
	;

stmt:
	decl										{ $$ = $1; }
	| atribuicao								{
													buscar_variavel_declarada(((no_arvore *) $1)->dado.attr->resultado->lexema);
													$$ = $1;
												}
	| decl_array								{ $$ = $1; }
	| atr_array									{ $$ = $1; }
	| expr 										{ $$ = $1; }
	| exprlogica								{ $$ = $1; }
	| leia 										{ $$ = $1; }
	| escreva 									{ $$ = $1; }
	| se_senao 									{ $$ = $1; }
	| enquanto 									{ $$ = $1; }
	;

numero_inteiro:
	NUM_INT										{ $$ = (long) buscar_ou_add_numero((char *) $1, INT); }
	;

expr:
	numero_inteiro								{ $$ = $1; }
	| ID 	%prec REDUCE						{ $$ = (long) buscar_variavel_declarada((char *) $1); }
	| NUM_REAL									{ $$ = (long) buscar_ou_add_numero((char *) $1, REAL); }
	| indice_array								{ $$ = $1; }
	| chamar_funcao								{ $$ = $1; }
	| expr '*' expr								{ $$ = (long) criar_no_expressao(MULT, (void *) $3, (void *) $1); }
	| expr '/' expr								{ $$ = (long) criar_no_expressao(DIV, (void *) $3, (void *) $1); }
	| expr '%' expr								{ $$ = (long) criar_no_expressao(MOD, (void *) $3, (void *) $1); }
	| expr '+' expr								{ $$ = (long) criar_no_expressao(SOMA, (void *) $3, (void *) $1); }
	| expr '-' expr								{ $$ = (long) criar_no_expressao(SUB, (void *) $3, (void *) $1); }
	| '(' expr ')'								{ $$ = $2; }
//	| '-' expr 	%prec '*'						{}		// NAO ESTA FUNCIONANDO O %prec
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
	TIPO ID '[' numero_inteiro ']'										{
																			simbolo *nome = criar_simbolo ((void *) $2, $1);
																			$$ = (long) criar_no_decl_array($1, nome, (void *) $4, NULL);
																		}
	|  TIPO ID '[' numero_inteiro ']' '=' '{' lista_argumentos '}'		{ $$ = (long) criar_no_decl_array($1, criar_simbolo ((void *) $2, $1), (void *) $4, (void *) $8); }	
	|  TIPO ID '[' ']' '=' '{' lista_argumentos '}'						{ $$ = (long) criar_no_decl_array($1, criar_simbolo ((void *) $2, $1), NULL, (void *) $7); }
	;

atr_array:
	ID '[' expr ']' '=' expr					{ $$ = (long) criar_no_attr_array((void *) localizar_simbolo(topo_pilha(pilha), (char *) $1), (void *) $3, (void *) $6); }
	;

indice_array:
	ID '[' expr ']'								{ $$ = (long) criar_no_indice_array((void *) localizar_simbolo(topo_pilha(pilha), (char *) $1), (void *) $3); }
	;

leia:
	LEIA 
	'(' 
	ID 
	')'											{ $$ = (long) criar_no_leia((void *) localizar_simbolo(topo_pilha(pilha), (char *) $3)); }
	;

escreva:
	ESCREVA 
	'(' 
	lista_argumentos_vazio 
	')'											{ $$ = (long) criar_no_escreva((void *) $3); }
	;

chamar_funcao:
	ID
	'(' 
	lista_argumentos_vazio 
	')'											{
													simbolo *s = localizar_simbolo_contexto(&t_funcoes, (char *) $1);

													if(s == NULL)
														mostrar_erro_e_parar("Nao existe uma funcao com este nome.", (char *) $1);

													$$ = (long) criar_no_chamada_funcao((void *) localizar_simbolo_contexto(&t_funcoes, (char *) $1), (void *) $3);
												}
	;

lista_argumentos_vazio:
	lista_argumentos 							{ $$ = $1; }
	|											{ $$ = (long) NULL; }
	;

lista_argumentos:
	lista_argumentos ',' expr 					{ $$ = (long) criar_no_t_lista((void *) $3, (void *) $1); }
	| expr 										{ $$ = (long) criar_no_t_lista((void *) $1, NULL); }
	;

se_senao:
	SE '(' exprlogica ')' bloco %prec REDUCE	{ $$ = (long) criar_no_if_else((void *) $3, (void *) $5, NULL); }
	| SE '(' exprlogica ')' bloco SENAO bloco	{ $$ = (long) criar_no_if_else((void *) $3, (void *) $5, (void *) $7); }
	;

enquanto:
	ENQUANTO '(' exprlogica ')' bloco			{ $$ = (long) criar_no_while((void *) $3, (void *) $5); }
	;
%%

no_arvore * buscar_variavel_declarada(char *lexema) {
	simbolo *s = localizar_simbolo(topo_pilha(pilha), lexema);

	if(s == NULL)
		mostrar_erro_e_parar("Variavel nao declarada.", lexema);

	no_arvore *no = criar_no_expressao(ID, s, NULL);
	no->dado.expr->tipo = s->tipo;
	return no;
}

no_arvore * buscar_ou_add_numero(char *lexema, int tipo) {
	numero *n = localizar_numero(&t_numeros, lexema, tipo); 
	if(n == NULL){
		n = criar_numero(lexema, tipo);
		inserir_numero(&t_numeros, n);
	}
	no_arvore *no = criar_no_expressao(NUMERO, n, NULL);
	no->dado.expr->tipo = tipo;
	return no;
}

void verificar_simbolo_duplicado(simbolo *s) {
	simbolo *novo;

	novo = localizar_simbolo_contexto(&t_funcoes, s->lexema);
	if(novo != NULL)
		mostrar_erro_e_parar("Nao e permitido declarar variaveis com o mesmo nome de uma funcao.", s->lexema);

	novo = localizar_simbolo_contexto(topo_pilha(pilha), s->lexema);
	if(novo != NULL)
		mostrar_erro_e_parar("Multiplas declaracoes de variavel.", s->lexema);

	add_lexema_usado(&t_lexemas_usados, s->lexema);
	inserir_simbolo(topo_pilha(pilha), s);
}

void verificar_existencia_lexema(char *lexema) {
	if(buscar_lexema_usado(&t_lexemas_usados, lexema) != NULL)
		mostrar_erro_e_parar("Nao e permitido declarar variaveis com o mesmo nome de uma funcao.", lexema);
}

void mostrar_erro_e_parar(char *s, char *var) {
	printf("%s [%s]\n", s, var);
	exit(0);
}

void yyerror(char *s) {
	fprintf(stderr, "%s\n", s);
}

int main(void) {
	pilha = NULL;
	yyparse();
	printf("\n");
	return 0;
}
