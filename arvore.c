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

void imprimir_pos_ordem(no_arvore *no) {
	if(no == NULL)
		return;

	t_expr_logica *exprlogica;
	t_expr * expr;
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
				case EXPR_LOGICA:
					imprimir_pos_ordem((no_arvore *) exprlogica->dir);
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
			}
			break;
	}

	printf(" ");
}