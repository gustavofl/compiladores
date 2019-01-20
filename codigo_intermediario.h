#ifndef CODIGO_INTERMEDIARIO_H
#define CODIGO_INTERMEDIARIO_H

#include <string.h>
#include "arvore.h"

typedef struct instrucao{
	int opcode;
	simbolo *result;
	void *first;
	void *second;
} instrucao;

typedef struct no_instrucao {
	instrucao *inst;
	struct no_instrucao *proximo;
} no_instrucao;

typedef struct fila_instrucoes{
	no_instrucao *primeiro;
	no_instrucao *ultimo;
} fila_instrucoes;

char * gerar_lexema_temp();
simbolo * gerar_simbolo_temp(int tipo);

instrucao * gerar_instrucao(int opcode, simbolo *result, void *first, void *second);
void add_instrucao(fila_instrucoes *fila, instrucao *i);

fila_instrucoes * gerar_codigo_intermediario(fila_instrucoes *fila, no_arvore *no);
void gerar_codigo(fila_instrucoes *fila, no_arvore *no);
simbolo * gerar_codigo_conversao_tipo(fila_instrucoes *fila, int tipo, simbolo *s);
simbolo * gerar_codigo_expr_simples(fila_instrucoes *fila, int opcode, t_expr *expr);
simbolo * gerar_codigo_expr(fila_instrucoes *fila, no_arvore *no);
simbolo * gerar_codigo_attr(fila_instrucoes *fila, no_arvore *no);

void imprimir_codigo(fila_instrucoes *fila);

#endif