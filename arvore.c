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

no_arvore * criar_no_t_lista(void *dir, void *esq){
	no_arvore *novo = (no_arvore *) malloc(sizeof(no_arvore));
	novo->tipo = LISTA;
	novo->dado.lista = criar_t_lista(dir, esq);
	return novo;
}

t_lista * criar_t_lista(void *dir, void *esq){
	t_lista *novo = (t_lista *) malloc(sizeof(t_lista));
	novo->dir = dir;
	novo->esq = esq;
	return novo;
}

no_arvore * criar_no_chamada_funcao(simbolo *nome, void *args){
	no_arvore *novo = (no_arvore *) malloc(sizeof(no_arvore));
	novo->tipo = CHAMADA_FUNCAO;
	novo->dado.chamadafuncao = criar_chamada_funcao(nome, args);
	return novo;
}

t_chamada_funcao * criar_chamada_funcao(simbolo *nome, void *args){
	t_chamada_funcao * novo = (t_chamada_funcao *) malloc(sizeof(t_chamada_funcao));
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

no_arvore * criar_no_param(int tipo, simbolo *variavel){
	no_arvore *novo = (no_arvore *)  malloc(sizeof(no_arvore));
	novo->tipo = PARAMETRO;
	novo->dado.param =  criar_param(tipo, variavel);
	return novo;
}

t_param * criar_param(int tipo, simbolo *variavel){ 
	t_param * novo = (t_param *) malloc(sizeof(t_param));
	novo->tipo = tipo;
	novo->variavel = variavel;
	return novo;
}

no_arvore * criar_no_funcao(int tipo, simbolo *nome, t_lista *params, void *bloco){
	no_arvore *novo = (no_arvore *) malloc(sizeof(no_arvore));
	novo->tipo = FUNCAO;
	novo->dado.funcao = criar_funcao(tipo, nome, params, bloco);
	return novo;
}

t_funcao * criar_funcao(int tipo, simbolo *nome, t_lista *params, void *bloco){
	t_funcao *novo = (t_funcao *) malloc(sizeof(t_funcao));
	novo->tipo = tipo;
	novo->nome = nome;
	novo->params = params;
	novo->bloco = bloco;
	return novo;
}

no_arvore * criar_no_decl_array(int tipo, simbolo *nome, void *tamanho, void *valores_iniciais){
	no_arvore *novo = (no_arvore *) malloc(sizeof(no_arvore));
	novo->tipo = DECL_ARRAY;
	novo->dado.declarray = criar_decl_array(tipo, nome, tamanho, valores_iniciais);
	return novo;
}

t_decl_array * criar_decl_array(int tipo, simbolo *nome, void *tamanho, void *valores_iniciais){
	t_decl_array *novo = (t_decl_array *) malloc(sizeof(t_decl_array));
	novo->tipo = tipo;
	novo->nome = nome;
	novo->tamanho = tamanho;
	novo->valores_iniciais = valores_iniciais;
	return novo;
}

no_arvore * criar_no_attr_array(simbolo *nome, void *indice, void *valor){
	no_arvore *novo = (no_arvore *) malloc(sizeof(no_arvore));
	novo->tipo = ATTR_ARRAY;
	novo->dado.attrarray = criar_attr_array(nome, indice, valor);
	return novo;
}

t_attr_array * criar_attr_array(simbolo *nome, void *indice, void *valor){
	t_attr_array *novo = (t_attr_array *) malloc(sizeof(t_attr_array));
	novo->nome = nome;
	novo->indice = indice;
	novo->valor = valor;
	return novo;
}


void imprimir_pos_ordem(no_arvore *no) {
	if(no == NULL)
		return;

	t_expr_logica *exprlogica;
	t_expr * expr;
	t_chamada_funcao *chamadafuncao;
	t_escreva *escreva;
	t_leia *leia;
	t_funcao *funcao;
	t_param *param;
	t_decl_array *declarray;
	t_lista *lista;
	t_attr_array *attrarray;
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
		case LISTA:
			lista = no->dado.lista;
			imprimir_pos_ordem((no_arvore *) lista->esq);
			imprimir_pos_ordem((no_arvore *) lista->dir);
			break;
		case CHAMADA_FUNCAO:
			chamadafuncao = no->dado.chamadafuncao;
			printf("%s", ((simbolo *) chamadafuncao->nome)->lexema);
			printf(" (");
			imprimir_pos_ordem((no_arvore *) chamadafuncao->args);
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
		case FUNCAO:
			funcao = no->dado.funcao;
			printf("\nFUNCAO %s", ((simbolo *) funcao->nome)->lexema);
			printf(" (");
			imprimir_pos_ordem((no_arvore *) funcao->params);
			printf(") { \n");
			imprimir_pos_ordem((no_arvore *) funcao->bloco);
			printf("\n}\n");
			break;
		case PARAMETRO:
			param = no->dado.param;
			switch(param->tipo){
				case INT:
					printf("INT ");
					break;
				case REAL:
					printf("REAL ");
					break;
				default:
					printf("UNDEFINED ");
					break;
			} 
			printf("%s", ((simbolo *) param->variavel)->lexema);
			break;
		case DECL_ARRAY:
			declarray = no->dado.declarray;
			switch(declarray->tipo){
				case INT:
					printf("INT ");
					break;
				case REAL:
					printf("REAL ");
					break;
				default:
					printf("UNDEFINED ");
					break;
			} 
			printf("%s", ((simbolo *) declarray->nome)->lexema);
			printf(" [");
			imprimir_pos_ordem((no_arvore *) declarray->tamanho);
			printf("] <- { ");
			imprimir_pos_ordem((no_arvore *) declarray->valores_iniciais);
			printf("}");
			break;
		case ATTR_ARRAY:
			attrarray = no->dado.attrarray;
			printf("%s ", ((simbolo *) attrarray->nome)->lexema);
			imprimir_pos_ordem((no_arvore *) attrarray->indice);
			printf("INDEX ");
			imprimir_pos_ordem((no_arvore *) attrarray->valor);
			printf("=");
	}

	printf(" ");
}