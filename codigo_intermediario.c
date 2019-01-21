#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "arvore.h"
#include "codigo_intermediario.h"
#include "y.tab.h"

int temp_ctr = 1;

char * gerar_lexema_temp() {
	char buffer[256];
	sprintf(buffer, "t%d", temp_ctr++);
	return strdup(buffer);
}

simbolo * gerar_simbolo_temp(int tipo) {
	char buffer[256];
	sprintf(buffer, "t%d", temp_ctr++);

	simbolo *s = criar_simbolo(strdup(buffer), tipo);

	return s;
}

instrucao * gerar_instrucao(int opcode, simbolo *result, void *first, void *second) {
	instrucao *novo = (instrucao *)  malloc(sizeof(instrucao));
	novo->opcode = opcode;
	novo->result = result;
	novo->first = first;
	novo->second = second;

	return novo;
}

void add_instrucao(fila_instrucoes *fila, instrucao *i) {
	no_instrucao *novo = (no_instrucao *)  malloc(sizeof(no_instrucao));
	novo->inst = i;
	novo->proximo = NULL;

	// ATUALIZAR FILA
	if(fila->ultimo == NULL) {
		fila->primeiro = novo;
		fila->ultimo = novo;
	} else {
		fila->ultimo->proximo = novo;
		fila->ultimo = novo;
	}
}

simbolo * gerar_codigo_conversao_tipo(fila_instrucoes *fila, int tipo, simbolo *s) {
	int opcode;
	instrucao *i;

	if(tipo == INT)
		opcode = CAST_INT;
	else
		opcode = CAST_REAL;

	simbolo *result = gerar_simbolo_temp(tipo);

	i = gerar_instrucao(opcode, result, s, NULL);
	add_instrucao(fila, i);

	return result;
}

simbolo * gerar_codigo_expr_simples(fila_instrucoes *fila, int opcode, t_expr *expr) {
	instrucao *i;

	simbolo *first = gerar_codigo_expr(fila, (no_arvore *) expr->esq);
	if( first->tipo != expr->tipo )
		first = gerar_codigo_conversao_tipo(fila, expr->tipo, first);

	simbolo *second = gerar_codigo_expr(fila, (no_arvore *) expr->dir);
	if( second->tipo != expr->tipo ){
		second = gerar_codigo_conversao_tipo(fila, expr->tipo, second);
	}

	simbolo *result = gerar_simbolo_temp(expr->tipo);

	i = gerar_instrucao(opcode, result, first, second);
	add_instrucao(fila, i);

	return result;
}

simbolo * gerar_codigo_expr(fila_instrucoes *fila, no_arvore *no) {
	simbolo *result;
	void *first, *second;
	instrucao *i = NULL;

	t_expr *expr = no->dado.expr;
	switch(expr->op){
		case NUMERO:
			result = gerar_simbolo_temp(expr->tipo);
			first = expr->dir;
			if(((simbolo *)first)->tipo == INT)
				i = gerar_instrucao(NUM_INT, result, first, NULL);
			else
				i = gerar_instrucao(NUM_REAL, result, first, NULL);
			break;
		case ID:
			return (simbolo *) expr->dir;
		case SOMA:
			result = gerar_codigo_expr_simples(fila, SOMA, expr);
			break;
		case SUB:
			result = gerar_codigo_expr_simples(fila, SUB, expr);
			break;
		case MULT:
			result = gerar_codigo_expr_simples(fila, MULT, expr);
			break;
		case DIV:
			result = gerar_codigo_expr_simples(fila, DIV, expr);
			break;
		case UMINUS:
			first = gerar_codigo_expr(fila, expr->dir);
			result = gerar_simbolo_temp(expr->tipo);
			i = gerar_instrucao(UMINUS, result, first, NULL);
			break;
		case MOD:
			result = gerar_codigo_expr_simples(fila, MOD, expr);
			break;
		case CHAMADA_FUNCAO:
			result = gerar_codigo_chamada_funcao(fila, (no_arvore *) expr->dir);
			break;
		case INDICE_ARRAY:
			result = gerar_codigo_indice_array(fila, (no_arvore *) expr->dir);
			break;
	}

	if(i != NULL)
		add_instrucao(fila, i);

	return result;
}

simbolo * gerar_codigo_attr(fila_instrucoes *fila, no_arvore *no) {
	simbolo *result;
	void *first;
	instrucao *i;

	result = no->dado.attr->resultado;

	first = gerar_codigo_expr(fila, (no_arvore *) no->dado.attr->expressao);

	if( ((simbolo *)first)->tipo != result->tipo )
		first = gerar_codigo_conversao_tipo(fila, result->tipo, first);

	i = gerar_instrucao(ATTR, result, first, NULL);
	add_instrucao(fila, i);

	return result;
}

simbolo * gerar_codigo_chamada_funcao(fila_instrucoes *fila, no_arvore *no){
	return NULL;
}

simbolo * gerar_codigo_indice_array(fila_instrucoes *fila, no_arvore *no){
	return NULL;
}

simbolo * gerar_codigo_expr_logica(fila_instrucoes *fila, no_arvore *no) {
	simbolo *result;
	void *first, *second;
	instrucao *i = NULL;

	t_expr_logica *exprlogica = no->dado.exprlogica;
	switch(exprlogica->op){
		case BOOLEANO:
			result = gerar_simbolo_temp(BOOLEANO);
			first = exprlogica->dir;
			i = gerar_instrucao(BOOLEANO, result, first, NULL);
			break;
		case OU: case E:
			first = gerar_codigo_expr_logica(fila, (no_arvore *) exprlogica->esq);
			second = gerar_codigo_expr_logica(fila, (no_arvore *) exprlogica->dir);
			result = gerar_simbolo_temp(BOOLEANO);
			i = gerar_instrucao(exprlogica->op, result, first, second);
			break;
		case MAIOR: case MENOR: case MAIOR_IGUAL: case MENOR_IGUAL:
			first = gerar_codigo_expr(fila, (no_arvore *) exprlogica->esq);
			second = gerar_codigo_expr(fila, (no_arvore *) exprlogica->dir);
			result = gerar_simbolo_temp(BOOLEANO);
			i = gerar_instrucao(exprlogica->op, result, first, second);
			break;
		case IGUAL_COMP: case DIFERENTE:
			if(((no_arvore *) exprlogica->esq)->tipo == EXPR) {
				first = gerar_codigo_expr(fila, (no_arvore *) exprlogica->esq);
				second = gerar_codigo_expr(fila, (no_arvore *) exprlogica->dir);
				result = gerar_simbolo_temp(BOOLEANO);
				i = gerar_instrucao(exprlogica->op, result, first, second);
			} else {
				first = gerar_codigo_expr_logica(fila, (no_arvore *) exprlogica->esq);
				second = gerar_codigo_expr_logica(fila, (no_arvore *) exprlogica->dir);
				result = gerar_simbolo_temp(BOOLEANO);
				i = gerar_instrucao(exprlogica->op, result, first, second);
			}
			break;
		case NAO:
			first = gerar_codigo_expr_logica(fila, exprlogica->dir);
			result = gerar_simbolo_temp(BOOLEANO);
			i = gerar_instrucao(NAO, result, first, NULL);
			break;
	}

	add_instrucao(fila, i);

	return result;
}

void imprimir_codigo(fila_instrucoes *fila) {
	no_instrucao *no = fila->primeiro;
	instrucao *i;

	while(no != NULL) {
		i = no->inst;

		switch(i->opcode) {
			case ATTR:
				printf("(attr,%s,%s,-)\n", i->result->lexema, ((simbolo *)i->first)->lexema);
				break;
			case NUM_INT:
				printf("(num_int,%s,%d,-)\n", i->result->lexema, ((numero *)i->first)->valor_inteiro);
				break;
			case NUM_REAL:
				printf("(num_real,%s,%.2f,-)\n", i->result->lexema, ((numero *)i->first)->valor_real);
				break;
			case CAST_INT:
				printf("(cast_int,%s,%s,-)\n", i->result->lexema, ((simbolo *)i->first)->lexema);
				break;
			case CAST_REAL:
				printf("(cast_real,%s,%s,-)\n", i->result->lexema, ((simbolo *)i->first)->lexema);
				break;
			case SOMA:
				printf("(soma,%s,%s,%s)\n", i->result->lexema, ((simbolo *)i->first)->lexema, ((simbolo *)i->second)->lexema);
				break;
			case SUB:
				printf("(sub,%s,%s,%s)\n", i->result->lexema, ((simbolo *)i->first)->lexema, ((simbolo *)i->second)->lexema);
				break;
			case MULT:
				printf("(mult,%s,%s,%s)\n", i->result->lexema, ((simbolo *)i->first)->lexema, ((simbolo *)i->second)->lexema);
				break;
			case DIV:
				printf("(div,%s,%s,%s)\n", i->result->lexema, ((simbolo *)i->first)->lexema, ((simbolo *)i->second)->lexema);
				break;
			case UMINUS:
				printf("(uminus,%s,%s,-)\n", i->result->lexema, ((simbolo *)i->first)->lexema);
				break;
			case MOD:
				printf("(mod,%s,%s,%s)\n", i->result->lexema, ((simbolo *)i->first)->lexema, ((simbolo *)i->second)->lexema);
				break;
			case BOOLEANO:
				printf("(booleano,%s,%ld,-)\n", i->result->lexema, (long) i->first);
				break;
			case E:
				printf("(e,%s,%s,%s)\n", i->result->lexema, ((simbolo *)i->first)->lexema, ((simbolo *)i->second)->lexema);
				break;
			case OU:
				printf("(ou,%s,%s,%s)\n", i->result->lexema, ((simbolo *)i->first)->lexema, ((simbolo *)i->second)->lexema);
				break;
			case MAIOR:
				printf("(maior,%s,%s,%s)\n", i->result->lexema, ((simbolo *)i->first)->lexema, ((simbolo *)i->second)->lexema);
				break;
			case MAIOR_IGUAL:
				printf("(maior_igual,%s,%s,%s)\n", i->result->lexema, ((simbolo *)i->first)->lexema, ((simbolo *)i->second)->lexema);
				break;
			case MENOR:
				printf("(menor,%s,%s,%s)\n", i->result->lexema, ((simbolo *)i->first)->lexema, ((simbolo *)i->second)->lexema);
				break;
			case MENOR_IGUAL:
				printf("(menor_igual,%s,%s,%s)\n", i->result->lexema, ((simbolo *)i->first)->lexema, ((simbolo *)i->second)->lexema);
				break;
			case IGUAL_COMP:
				printf("(igual_comp,%s,%s,%s)\n", i->result->lexema, ((simbolo *)i->first)->lexema, ((simbolo *)i->second)->lexema);
				break;
			case DIFERENTE:
				printf("(diferente,%s,%s,%s)\n", i->result->lexema, ((simbolo *)i->first)->lexema, ((simbolo *)i->second)->lexema);
				break;
			case NAO:
				printf("(nao,%s,%s,-)\n", i->result->lexema, ((simbolo *)i->first)->lexema);
				break;
		}

		no = no->proximo;
	}
}

void gerar_codigo(fila_instrucoes *fila, no_arvore *no) {
	if(no == NULL)
		return;

	switch(no->tipo){
		case ATTR:
			gerar_codigo_attr(fila, no);
			break;
		case EXPR:
			gerar_codigo_expr(fila, no);
			break;
		case LISTA:
			gerar_codigo(fila, (no_arvore *)((t_lista *) no->dado.lista)->esq);
			gerar_codigo(fila, (no_arvore *)((t_lista *) no->dado.lista)->dir);
			break;
		case EXPR_LOGICA:
			gerar_codigo_expr_logica(fila, no);
			break;
		case IF_ELSE:
			// testando somente a condicao
			gerar_codigo_expr_logica(fila, (no_arvore *)((t_if_else *) no->dado.ifelse)->condicao);
			break;
		// case CHAMADA_FUNCAO:
		// 	chamadafuncao = no->dado.chamadafuncao;

		// 	printf("%s", ((simbolo *) chamadafuncao->nome)->lexema);
		// 	printf(" (");
		// 	imprimir_pos_ordem((no_arvore *) chamadafuncao->args);
		// 	printf(") CHAMADA_FUNCAO");

		// 	break;
		// case ESCREVA:
		// 	escreva = no->dado.escreva;
		// 	printf(" (");
		// 	imprimir_pos_ordem((no_arvore *) escreva->args);
		// 	printf(") ESCREVA");
		// 	break;
		// case LEIA:
		// 	leia = no->dado.leia;
		// 	printf("%s LEIA", ((simbolo *) leia->variavel)->lexema);
		// 	break;
		// case FUNCAO:
		// 	funcao = no->dado.funcao;
		// 	printf("\nFUNCAO %s", ((simbolo *) funcao->nome)->lexema);
		// 	printf(" (");
		// 	imprimir_pos_ordem((no_arvore *) funcao->params);
		// 	printf(") { \n");
		// 	imprimir_pos_ordem((no_arvore *) funcao->bloco);
		// 	printf("\n}\n");
		// 	break;
		// case PARAMETRO:
		// 	param = no->dado.param;
		// 	switch(param->tipo){
		// 		case INT:
		// 			printf("INT ");
		// 			break;
		// 		case REAL:
		// 			printf("REAL ");
		// 			break;
		// 		default:
		// 			printf("UNDEFINED ");
		// 			break;
		// 	} 
		// 	printf("%s", ((simbolo *) param->variavel)->lexema);
		// 	break;
		// case DECL_ARRAY:
		// 	declarray = no->dado.declarray;
		// 	printf("%s ", ((simbolo *) declarray->nome)->lexema);
		// 	imprimir_pos_ordem((no_arvore *) declarray->tamanho);
		// 	printf("ARRAY_INDEX ");
		// 	printf("{ ");
		// 	imprimir_pos_ordem((no_arvore *) declarray->valores_iniciais);
		// 	printf("} ");
		// 	printf("=");
		// 	break;
		// case ATTR_ARRAY:
		// 	attrarray = no->dado.attrarray;
		// 	printf("%s ", ((simbolo *) attrarray->nome)->lexema);
		// 	imprimir_pos_ordem((no_arvore *) attrarray->indice);
		// 	printf("ARRAY_INDEX ");
		// 	imprimir_pos_ordem((no_arvore *) attrarray->valor);
		// 	printf("=");
		// 	break;
		// case INDICE_ARRAY:
		// 	indicearray = no->dado.indicearray;

		// 	printf("%s ", ((simbolo *) indicearray->nome)->lexema);
		// 	imprimir_pos_ordem((no_arvore *) indicearray->indice);
		// 	printf("ARRAY_INDEX");

		// 	break;
		// case IF_ELSE:
		// 	ifelse = no->dado.ifelse;
		// 	printf("{ ");
		// 	imprimir_pos_ordem((no_arvore *) ifelse->bloco_if);
		// 	printf("} { ");
		// 	imprimir_pos_ordem((no_arvore *) ifelse->bloco_else);
		// 	printf("} ( ");
		// 	imprimir_pos_ordem((no_arvore *) ifelse->condicao);
		// 	printf(") IF_ELSE");
		// 	break;
		// case WHILE:
		// 	twhile = no->dado.twhile;
		// 	printf("{ ");
		// 	imprimir_pos_ordem((no_arvore *) twhile->bloco);
		// 	printf("} ( ");
		// 	imprimir_pos_ordem((no_arvore *) twhile->condicao);
		// 	printf(") WHILE");
		// 	break;
	}
}