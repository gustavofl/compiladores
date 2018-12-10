#include <stdlib.h>
#include <stdio.h>
#include "arvore.h"
#include "y.tab.h"

no_arvore * criar_no_expressao_logica(int op, void *dir, void *esq) {
	no_arvore *novo = (no_arvore *)  malloc(sizeof(no_arvore));
	novo->tipo = EXPR_LOGICA;
	novo->dado.exprlogica = criar_expressao_logica(op, dir, esq);
	return novo;
}

t_expr_logica * criar_expressao_logica(int op, void *dir, void *esq) {
	t_expr_logica * novo = (t_expr_logica *) malloc(sizeof(t_expr_logica));
	novo->op = op;
	novo->dir = dir;
	novo->esq = esq;
	return novo;
}

no_arvore * criar_no_expressao(int op, void *dir, void *esq) {
	no_arvore *novo = (no_arvore *)  malloc(sizeof(no_arvore));
	novo->tipo = EXPR;
	novo->dado.expr =  criar_expressao(op, dir, esq);
	return novo;
}

t_expr * criar_expressao(int op, void *dir, void *esq) {
	t_expr * novo = (t_expr *) malloc(sizeof(t_expr));
	novo->op = op;
	novo->dir = dir;
	novo->esq = esq;
	return novo;
}

no_arvore * criar_no_atribuicao(simbolo *resultado, void *expressao) {
	no_arvore *novo = (no_arvore *)  malloc(sizeof(no_arvore));
	novo->tipo = ATTR;
	novo->dado.attr =  criar_atribuicao(resultado, expressao);
	return novo;
}

t_attr * criar_atribuicao(simbolo *resultado, void *expressao){ 
	t_attr * novo = (t_attr *) malloc(sizeof(t_attr));
	novo->resultado = resultado;
	novo->expressao = expressao;
	return novo;
}

no_arvore * criar_no_t_lista_attr(void *dir, void *esq){
	no_arvore *novo = (no_arvore *) malloc(sizeof(no_arvore));
	novo->tipo = LISTA_ATTR;
	novo->dado.t_attrlista = criar_t_lista_attr(dir, esq);
	return novo;
}

t_lista_attr * criar_t_lista_attr(void *dir, void *esq){
	t_lista_attr *novo = (t_lista_attr *) malloc(sizeof(t_lista_attr));
	novo->dir = dir;
	novo->esq = esq;
	return novo;
}

no_arvore * criar_no_lista_arg(void *dir, void *esq) {
	no_arvore *novo = (no_arvore *) malloc(sizeof(no_arvore));
	novo->tipo = LISTA_ARG;
	novo->dado.lista_arg = criar_lista_arg(dir, esq);
	return novo;
}

t_lista_arg * criar_lista_arg(void *dir, void *esq) {
	t_lista_arg *novo = (t_lista_arg *) malloc(sizeof(t_lista_arg));
	novo->dir = dir;
	novo->esq = esq;
	return novo;
}

no_arvore * criar_no_funcao(simbolo *nome, void *args){
	no_arvore *novo = (no_arvore *) malloc(sizeof(no_arvore));
	novo->tipo = FUNCAO;
	novo->dado.funcao = criar_funcao(nome, args);
	return novo;
}

t_funcao * criar_funcao(simbolo *nome, void *args){
	t_funcao * novo = (t_funcao *) malloc(sizeof(t_funcao));
	novo->nome = nome;
	novo->args = args;
	return novo;
}

no_arvore * criar_no_escreva(void *args){
	no_arvore *novo = (no_arvore *) malloc(sizeof(no_arvore));
	novo->tipo = ESCREVA;
	novo->dado.escreva = criar_escreva(args);
	return novo;
}

t_escreva * criar_escreva(void *args){
	t_escreva * novo = (t_escreva *) malloc(sizeof(t_escreva));
	novo->args = args;
	return novo;
}

no_arvore * criar_no_leia(simbolo *variavel){
	no_arvore *novo = (no_arvore *) malloc(sizeof(no_arvore));
	novo->tipo = LEIA;
	novo->dado.leia = criar_leia(variavel);
	return novo;
}

t_leia * criar_leia(simbolo *variavel){
	t_leia * novo = (t_leia *) malloc(sizeof(t_leia));
	novo->variavel = variavel;
	return novo;
}

void imprimir_pos_ordem(no_arvore *no) {
	if(no == NULL)
		return;

	t_expr_logica *exprlogica;
	t_expr * expr;
	t_lista_arg *arglista;
	t_funcao *funcao;
	t_lista_attr *t_attrlista;
	t_escreva *escreva;
	t_leia *leia;
	switch(no->tipo){
		case EXPR_LOGICA:
			exprlogica = no->dado.exprlogica;
			switch(exprlogica->op){
				case BOOLEANO:
					switch((long) exprlogica->dir){
						case 282:
							printf("verdadeiro");
							break;
						case 283:
							printf("falso");
							break;
						default:
							printf("%ld", (long) exprlogica->dir);
						}
					break;
				case OU:
					imprimir_pos_ordem((no_arvore *) exprlogica->esq);
					imprimir_pos_ordem((no_arvore *) exprlogica->dir);
					printf("OU");
					break;
				case E:
					imprimir_pos_ordem((no_arvore *) exprlogica->esq);
					imprimir_pos_ordem((no_arvore *) exprlogica->dir);
					printf("E");
					break;
				case NAO:
					imprimir_pos_ordem((no_arvore *) exprlogica->dir);
					printf("NAO");
					break;
				case MAIOR:
					imprimir_pos_ordem((no_arvore *) exprlogica->esq);
					imprimir_pos_ordem((no_arvore *) exprlogica->dir);
					printf(">");
					break;
				case MENOR:
					imprimir_pos_ordem((no_arvore *) exprlogica->esq);
					imprimir_pos_ordem((no_arvore *) exprlogica->dir);
					printf("<");
					break;
				case MAIOR_IGUAL:
					imprimir_pos_ordem((no_arvore *) exprlogica->esq);
					imprimir_pos_ordem((no_arvore *) exprlogica->dir);
					printf(">=");
					break;
				case MENOR_IGUAL:
					imprimir_pos_ordem((no_arvore *) exprlogica->esq);
					imprimir_pos_ordem((no_arvore *) exprlogica->dir);
					printf("<=");
					break;
				case IGUAL_COMP:
					imprimir_pos_ordem((no_arvore *) exprlogica->esq);
					imprimir_pos_ordem((no_arvore *) exprlogica->dir);
					printf("==");
					break;
				case DIFERENTE:
					imprimir_pos_ordem((no_arvore *) exprlogica->esq);
					imprimir_pos_ordem((no_arvore *) exprlogica->dir);
					printf("!=");
					break;
			}
			break;
		case EXPR:
			expr = no->dado.expr;
			switch(expr->op){
				case NUMERO:
					switch(((numero *) expr->dir)->tipo){
						case INT:
							printf("%d", ((numero *) expr->dir)->valor_inteiro);
							break;
						case REAL:
							printf("%.2f", ((numero *) expr->dir)->valor_real);
							break;
					}
					break;
				case SOMA:
					imprimir_pos_ordem((no_arvore *) expr->esq);
					imprimir_pos_ordem((no_arvore *) expr->dir);
					printf("+");
					break;
				case SUB:
					imprimir_pos_ordem((no_arvore *) expr->esq);
					imprimir_pos_ordem((no_arvore *) expr->dir);
					printf("-");
					break;
				case MULT:
					imprimir_pos_ordem((no_arvore *) expr->esq);
					imprimir_pos_ordem((no_arvore *) expr->dir);
					printf("*");
					break;
				case DIV:
					imprimir_pos_ordem((no_arvore *) expr->esq);
					imprimir_pos_ordem((no_arvore *) expr->dir);
					printf("/");
					break;
				case MOD:
					imprimir_pos_ordem((no_arvore *) expr->esq);
					imprimir_pos_ordem((no_arvore *) expr->dir);
					printf("%c",'%');
					break;
				case ID:
					printf("%s", ((simbolo *) expr->dir)->lexema);
					break;
			}
			break;
		case ATTR:
			printf("%s ", no->dado.attr->resultado->lexema);
			imprimir_pos_ordem((no_arvore *) no->dado.attr->expressao);
			printf("=");
			break;
		case LISTA_ATTR:
			t_attrlista = no->dado.t_attrlista;
			imprimir_pos_ordem((no_arvore *) t_attrlista->esq);
			imprimir_pos_ordem((no_arvore *) t_attrlista->dir);
			break;
		case LISTA_ARG:
			arglista = no->dado.lista_arg;
			imprimir_pos_ordem((no_arvore *) arglista->esq);
			imprimir_pos_ordem((no_arvore *) arglista->dir);
			break;
		case FUNCAO:
			funcao = no->dado.funcao;
			printf("%s", ((simbolo *) funcao->nome)->lexema);
			printf(" (");
			imprimir_pos_ordem((no_arvore *) funcao->args);
			printf(") CHAMADA_FUNCAO");
			break;
		case ESCREVA:
			escreva = no->dado.escreva;
			printf(" (");
			imprimir_pos_ordem((no_arvore *) escreva->args);
			printf(") ESCREVA");
			break;
		case LEIA:
			leia = no->dado.leia;
			printf("%s LEIA", ((simbolo *) leia->variavel)->lexema);
			break;
	}

	printf(" ");
}