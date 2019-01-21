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
		case SOMA: case SUB: case MULT: case DIV: case MOD:
			result = gerar_codigo_expr_simples(fila, SOMA, expr);
			break;
		case UMINUS:
			first = gerar_codigo_expr(fila, expr->dir);
			result = gerar_simbolo_temp(expr->tipo);
			i = gerar_instrucao(UMINUS, result, first, NULL);
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

void gerar_codigo_attr(fila_instrucoes *fila, no_arvore *no) {
	simbolo *result;
	void *first;
	instrucao *i;

	result = no->dado.attr->resultado;

	first = gerar_codigo_expr(fila, (no_arvore *) no->dado.attr->expressao);

	if( ((simbolo *)first)->tipo != result->tipo )
		first = gerar_codigo_conversao_tipo(fila, result->tipo, first);

	i = gerar_instrucao(ATTR, result, first, NULL);
	add_instrucao(fila, i);
}

simbolo * gerar_codigo_chamada_funcao(fila_instrucoes *fila, no_arvore *no){
	return NULL;
}

simbolo * gerar_codigo_indice_array(fila_instrucoes *fila, no_arvore *no) {
	simbolo *result;
	void *first, *second;
	instrucao *i = NULL;

	t_indice_array *indicearray = no->dado.indicearray;
	first = indicearray->nome;
	second = gerar_codigo_expr(fila, indicearray->indice);
	result = gerar_simbolo_temp(indicearray->tipo);
	i = gerar_instrucao(INDICE_ARRAY, result, first, second);

	add_instrucao(fila, i);

	return result;
}

void gerar_codigo_attr_array(fila_instrucoes *fila, no_arvore *no){
	simbolo *nome;
	void *indice, *expr;
	instrucao *i = NULL;

	t_attr_array *attrarray = no->dado.attrarray;
	nome = attrarray->nome;
	indice = gerar_codigo_expr(fila, attrarray->indice);
	expr = gerar_codigo_expr(fila, attrarray->valor);
	i = gerar_instrucao(ATTR_ARRAY, nome, indice, expr);

	add_instrucao(fila, i);
}

void gerar_codigo_decl_array(tabela_numero *t_numeros, fila_instrucoes *fila, no_arvore *no) {
	simbolo *nome;
	void *indice, *expr;
	instrucao *i = NULL;
	t_decl_array *declarray;

	no_arvore *no_valor = no->dado.declarray->valores_iniciais;
	int count = 0;
	while(no_valor != NULL) {
		declarray = no->dado.declarray;
		nome = declarray->nome;

		char buffer[256];
		sprintf(buffer, "%d", count);
		no_arvore *num = buscar_ou_add_numero(t_numeros, strdup(buffer), INT);
		indice = gerar_codigo_expr(fila, num);
		
		expr = gerar_codigo_expr(fila, no_valor);

		i = gerar_instrucao(ATTR_ARRAY, nome, indice, expr);
		add_instrucao(fila, i);

		no_valor = no_valor->dado.lista->esq;
		count++;
	}
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
	char *opcode, *second, *first;
	char buffer[256];

	while(no != NULL) {
		i = no->inst;

		switch(i->opcode) {
			case ATTR:
				opcode = "attr";
				break;
			case NUM_INT:
				opcode = "num_int";
				break;
			case NUM_REAL:
				opcode = "num_real";
				break;
			case CAST_INT:
				opcode = "cast_int";
				break;
			case CAST_REAL:
				opcode = "cast_real";
				break;
			case SOMA:
				opcode = "soma";
				break;
			case SUB:
				opcode = "sub";
				break;
			case MULT:
				opcode = "mult";
				break;
			case DIV:
				opcode = "div";
				break;
			case UMINUS:
				opcode = "uminus";
				break;
			case MOD:
				opcode = "mod";
				break;
			case BOOLEANO:
				opcode = "booleano";
				break;
			case E:
				opcode = "e";
				break;
			case OU:
				opcode = "ou";
				break;
			case MAIOR:
				opcode = "maior";
				break;
			case MAIOR_IGUAL:
				opcode = "maior_igual";
				break;
			case MENOR:
				opcode = "menor";
				break;
			case MENOR_IGUAL:
				opcode = "menor_igual";
				break;
			case IGUAL_COMP:
				opcode = "igual_comp";
				break;
			case DIFERENTE:
				opcode = "diferente";
				break;
			case NAO:
				opcode = "nao";
				break;
			case INDICE_ARRAY:
				opcode = "indice_array";
				break;
			case ATTR_ARRAY:
				opcode = "attr_array";
				break;
			default:
				opcode = "undefined";
				break;
		}

		// PRIMEIRO OPERANDO
		if(i->opcode == BOOLEANO) {
			if (((long)i->first) == FALSO)
				first = "falso";
			else
				first = "verdadeiro";
		}
		else if(i->opcode == NUM_INT) {
			sprintf(buffer, "%d", ((numero *)i->first)->valor_inteiro);
			first = strdup(buffer);
		}
		else if(i->opcode == NUM_REAL) {
			sprintf(buffer, "%.2f", ((numero *)i->first)->valor_real);
			first = strdup(buffer);
		}
		else {
			first = ((simbolo *)i->first)->lexema;
		}

		// SEGUNDO OPERANDO
		if(i->second != NULL)
			second = ((simbolo *)i->second)->lexema;
		else
			second = "-";

		printf("(%s,%s,%s,%s)\n", opcode, i->result->lexema, first, second);

		no = no->proximo;
	}
}

void gerar_codigo(tabela_numero *t_numeros, fila_instrucoes *fila, no_arvore *no) {
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
			gerar_codigo(t_numeros, fila, (no_arvore *)((t_lista *) no->dado.lista)->esq);
			gerar_codigo(t_numeros, fila, (no_arvore *)((t_lista *) no->dado.lista)->dir);
			break;
		case EXPR_LOGICA:
			gerar_codigo_expr_logica(fila, no);
			break;
		case ATTR_ARRAY:
			gerar_codigo_attr_array(fila, no);
			break;
		case DECL_ARRAY:
			gerar_codigo_decl_array(t_numeros, fila, no);
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