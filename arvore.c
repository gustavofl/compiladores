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

no_arvore * criar_no_lista_attr(lista_attr * lista) {
	if((long) lista == NULO)
		return NULL;

	no_arvore *novo = (no_arvore *) malloc(sizeof(no_arvore));
	novo->tipo = LISTA_ATTR;
	novo->dado.attrlista = lista;
	return novo;
}

lista_attr * lista_atribuicao_add(lista_attr *lista, lista_attr *attr) {
	if((long) lista == NULO && (long) attr == NULO)
		return NULL;
	else if((long) lista == NULO) // logo (long) attr != NULO
		return attr;
	else if((long) attr == NULO) // logo (long) lista != NULO
		return lista;
	
	// se nao entrou em nenhum if, entao (long) lista != NULO && (long) attr != NULO

	lista_attr *novo = (lista_attr *) malloc(sizeof(lista_attr));
	novo->dado = attr->dado;
	novo->proximo = lista;
	return novo;
}

lista_attr * criar_lista_atribuicao(no_arvore *no) {
	lista_attr *novo = (lista_attr *) malloc(sizeof(lista_attr));
	novo->dado = no;
	novo->proximo = NULL;
	return novo;
}

void imprimir_pos_ordem(no_arvore *no) {
	if(no == NULL)
		return;

	t_expr_logica *exprlogica;
	t_expr * expr;
	lista_attr *attrlista;
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
			}
			break;
		case ATTR:
			printf("%s ", no->dado.attr->resultado->lexema);
			imprimir_pos_ordem((no_arvore *) no->dado.attr->expressao);
			printf("=");
			break;
		case LISTA_ATTR:
			if(no == NULL)
				printf("=NULL");
			else {
				if(no->dado.attrlista == NULL)
					printf("->DADO=NULL");
			}
			
			attrlista = no->dado.attrlista;
			while(attrlista != NULL) {
				imprimir_pos_ordem((no_arvore *) attrlista->dado);
				attrlista = attrlista->proximo;
			}
			break;
	}

	printf(" ");
}