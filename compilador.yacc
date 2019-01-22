%{
#ifndef YYSTYPE
#define YYSTYPE long int
#endif
#include <stdio.h>
#include <stdlib.h>
#include "tabela.h"
#include "arvore.h"
#include "codigo_intermediario.h"

int yylex(void);
void yyerror(char *);

void mostrar_erro_e_parar(char *s, char *var);

simbolo * buscar_variavel_declarada(char *lexema);
void verificar_simbolo_duplicado(simbolo *s);
void verificar_existencia_funcao(char *lexema);
void verificar_parametros(t_funcao *f, no_arvore *lista_args);

pilha_contexto *pilha;
tabela_numero t_numeros;
fila_buffer fila;
lista_funcoes l_funcoes;

fila_instrucoes codigo_intermediario;

// variavel para auxiliar na declaracao multiplas de variavel
// long tipo_attr_multiplas = 0;

// int yydebug = 1;
%}

%token PROGRAMA TIPO VAZIO INT REAL NUM_INT NUM_REAL ID EXPR ATTR OU E NAO SE SENAO ENQUANTO FUNCAO ESCREVA LEIA CADEIA MAIOR_IGUAL MENOR_IGUAL DIFERENTE IGUAL_COMP VERDADEIRO FALSO BOOLEANO RETORNE

// Constantes que são usadas para construir a arvore sintatica
%token EXPR_LOGICA MAIOR NUMERO MENOR SOMA SUB MULT DIV MOD NO_ARVORE NULO LISTA_ATTR LISTA_ARG PARAMETRO LISTA_PARAMETRO CHAMADA_FUNCAO DECL_ARRAY LISTA ATTR_ARRAY INDICE_ARRAY IF_ELSE WHILE UMINUS

// Constantes que serão usadas na geração de código intermediário
%token CAST_INT CAST_REAL LABEL JUMPER SE_FALSO LER_PARAM JMP_RETORNO SALVAR_PARAM

%nonassoc REDUCE
%nonassoc '('
%nonassoc SENAO
%nonassoc '['

%left OU
%left E
%left DIFERENTE IGUAL_COMP
%left '<' '>' MAIOR_IGUAL MENOR_IGUAL
%left NAO                                    // O portugol studio avalia o nao antes dos operadores logicos
%left '+' '-'
%left '*' '/' '%'
%%

program:
	PROGRAMA '{' criar_contexto
	corpo_programa
	'}'	fechar_contexto							{ 
													/* imprimir_pos_ordem((no_arvore *) $4); */
													gerar_codigo(&t_numeros, &codigo_intermediario, (no_arvore *) $4);
													imprimir_codigo(&codigo_intermediario);
												}
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
	bloco_funcao								{
													no_arvore *no = (no_arvore *) $7;
													verificar_existencia_funcao((char *) $3);
													simbolo *s = criar_simbolo((char *) $3, $2);
													no->dado.funcao->tipo = (int) $2;
													no->dado.funcao->nome = s;
													no->dado.funcao->params = (void *) $5;

													// verificar o tipo da funcao e do retorno (SEMANTICO)
													if($2 == VAZIO && no->dado.funcao->retorno != NULL)
														mostrar_erro_e_parar("retorno de funcao invalido.", s->lexema);
													if($2 != VAZIO && no->dado.funcao->retorno == NULL)
														mostrar_erro_e_parar("retorno de funcao invalido.", s->lexema);

													inserir_funcao(&l_funcoes, no->dado.funcao);
													$$ = (long) no;
												}
	;

lista_parametros_vazio:
	lista_parametros 							{ $$ = $1; }
	|											{ $$ = (long) NULL; }
	;

lista_parametros:
	lista_parametros ',' TIPO ID 				{
													simbolo *s = criar_simbolo((void *) $4, $3);
													add_buffer(&fila, s);
													no_arvore *no = criar_no_param($3, s);
													$$ = (long) criar_no_t_lista(no, (void *) $1);
												}
	| TIPO ID 									{
													simbolo *s = criar_simbolo((void *) $2, $1);
													add_buffer(&fila, s);
													no_arvore *no = criar_no_param($1, s);
													$$ = (long) criar_no_t_lista(no, NULL);
												}
	;

tipo_retorno:
	TIPO 										{ $$ = (long) $1; }
	|											{ $$ = VAZIO; }
	;

bloco_funcao:
	'{' criar_contexto verificar_buffer
	stmts
	retorno_funcao
	'}' fechar_contexto							{ $$ = (long) criar_no_funcao(0, NULL, NULL, (void *) $4, (void *) $5); }
	;

verificar_buffer:								{
													while(fila.primeiro != NULL){
														simbolo *param = pop_buffer(&fila);
														inserir_simbolo(topo_pilha(pilha), param);
													}
												}
retorno_funcao:
	RETORNE expr 								{ $$ = $2; }
	| RETORNE									{ $$ = (long) NULL; }
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
													no_arvore *no = (no_arvore *) $1;
													t_attr *attr = no->dado.attr;
													attr->resultado = buscar_variavel_declarada(attr->resultado->lexema);
													
													// o portugol faz cast implicito de inteiro para real, e vice-versa
													// logo nao existe a necessidade de verificar se o tipo da expressao eh compativel com o tipo da variavel

													$$ = (long) no;
												}
	| decl_array								{ $$ = $1; }
	| atr_array									{ $$ = $1; }
	| chamar_funcao								{ $$ = $1; }
	| leia 										{ $$ = $1; }
	| escreva 									{ $$ = $1; }
	| se_senao 									{ $$ = $1; }
	| enquanto 									{ $$ = $1; }
	;

numero_inteiro:
	NUM_INT										{ $$ = (long) buscar_ou_add_numero(&t_numeros, (char *) $1, INT); }
	;

expr:
	numero_inteiro								{ $$ = $1; }
	| ID 	%prec REDUCE						{
													simbolo *s = buscar_variavel_declarada((char *) $1);
													no_arvore *no = criar_no_expressao(ID, s, NULL);
													no->dado.expr->tipo = s->tipo;
													$$ = (long) no;
												}
	| NUM_REAL									{ $$ = (long) buscar_ou_add_numero(&t_numeros, (char *) $1, REAL); }
	| indice_array								{ $$ = (long) criar_no_expressao(INDICE_ARRAY, (void *) $1, NULL); }
	| chamar_funcao								{
													no_arvore *no = (no_arvore *) $1;
													if(no->dado.chamadafuncao->tipo == VAZIO)
														mostrar_erro_e_parar("Funcoes com retorno VAZIO nao podem ser usadas em expressoes.", no->dado.chamadafuncao->nome->lexema);
													$$ = (long) criar_no_expressao(CHAMADA_FUNCAO, no, NULL);
												}
	| expr '*' expr								{ $$ = (long) criar_no_expressao(MULT, (void *) $3, (void *) $1); }
	| expr '/' expr								{ $$ = (long) criar_no_expressao(DIV, (void *) $3, (void *) $1); }
	| expr '%' expr								{ $$ = (long) criar_no_expressao(MOD, (void *) $3, (void *) $1); }
	| expr '+' expr								{ $$ = (long) criar_no_expressao(SOMA, (void *) $3, (void *) $1); }
	| expr '-' expr								{ $$ = (long) criar_no_expressao(SUB, (void *) $3, (void *) $1); }
	| '(' expr ')'								{ $$ = $2; }
	| '-' expr 									{ $$ = (long) criar_no_expressao(UMINUS, (void *) $2, NULL); }
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
																			verificar_simbolo_duplicado(nome);
																			$$ = (long) criar_no_decl_array($1, nome, (void *) $4, NULL);
																		}
	|  TIPO ID '[' numero_inteiro ']' '=' '{' lista_argumentos '}'		{
																			simbolo *nome = criar_simbolo ((void *) $2, $1);
																			verificar_simbolo_duplicado(nome);
																			no_arvore *no_tam_indice = (no_arvore *) $4;

																			int tam_lista = contar_elementos_lista((no_arvore *) $8);
																			int tam_indice = ((numero *)no_tam_indice->dado.expr->dir)->valor_inteiro;
																			if(tam_lista != tam_indice)
																				mostrar_erro_e_parar("Quantidade incompativel de elementos na inicializacao do vetor.", nome->lexema);

																			$$ = (long) criar_no_decl_array($1, nome, no_tam_indice, (void *) $8);
																		}
	|  TIPO ID '[' ']' '=' '{' lista_argumentos '}'						{
																			simbolo *nome = criar_simbolo ((void *) $2, $1);
																			verificar_simbolo_duplicado(nome);

																			char buffer[256];
																			int tam = contar_elementos_lista((no_arvore *) $7);
																			sprintf(buffer, "%d", tam);

																			no_arvore *tam_array = buscar_ou_add_numero(&t_numeros, strdup(buffer), INT);

																			$$ = (long) criar_no_decl_array($1, nome, tam_array, (void *) $7);
																		}
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
	')'											{ $$ = (long) criar_no_leia((void *) buscar_variavel_declarada((char *) $3)); }
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
													t_funcao *f = localizar_funcao(&l_funcoes, (char *) $1);

													if(f == NULL)
														mostrar_erro_e_parar("Nao existe uma funcao com este nome.", (char *) $1);

													verificar_parametros(f, (no_arvore *) $3);

													$$ = (long) criar_no_chamada_funcao(f->nome, (void *) $3);
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

simbolo * buscar_variavel_declarada(char *lexema) {
	simbolo *s = localizar_simbolo(topo_pilha(pilha), lexema);

	if(s == NULL)
		mostrar_erro_e_parar("Variavel nao declarada.", lexema);

	return s;
	/*
	no_arvore *no = criar_no_expressao(ID, s, NULL);
	no->dado.expr->tipo = s->tipo;
	return no;
	*/
}

void verificar_simbolo_duplicado(simbolo *s) {
	simbolo *novo;

	if( localizar_funcao(&l_funcoes, s->lexema) != NULL )
		mostrar_erro_e_parar("Nao e permitido declarar variaveis com o mesmo nome de uma funcao.", s->lexema);

	novo = localizar_simbolo_contexto(topo_pilha(pilha), s->lexema);
	if(novo != NULL)
		mostrar_erro_e_parar("Multiplas declaracoes de variavel.", s->lexema);

	inserir_simbolo(topo_pilha(pilha), s);
}

void verificar_existencia_funcao(char *lexema) {
	if( localizar_funcao(&l_funcoes, lexema) != NULL )
		mostrar_erro_e_parar("Ja existe uma funcao com esse nome.", lexema);
}

void verificar_parametros(t_funcao *f, no_arvore *lista_args) {
	no_arvore *no_params = f->params;
	no_arvore *no_args = lista_args;

	int qnt_params = 0;
	while(no_params != NULL) {
		no_params = no_params->dado.lista->esq;
		qnt_params++;
	}

	int qnt_args = 0;
	while(no_args != NULL) {
		no_args = no_args->dado.lista->esq;
		qnt_args++;
	}

	if(qnt_params != qnt_args){
		printf("A funcao '%s' espera %d parametros, mas foram passados %d.\n", f->nome->lexema, qnt_params, qnt_args);
		exit(0);
	}
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
