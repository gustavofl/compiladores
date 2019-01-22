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

simbolo * gerar_simbolo_temp(int tipo);
simbolo * gerar_label();
simbolo * gerar_label_funcao(char *identificador);
simbolo * gerar_label_retorno(char *identificador, int tipo);

instrucao * gerar_instrucao(int opcode, simbolo *result, void *first, void *second);
void add_instrucao(fila_instrucoes *fila, instrucao *i);

fila_instrucoes * gerar_codigo_intermediario(fila_instrucoes *fila, no_arvore *no);
void gerar_codigo(tabela_numero *t_numeros, fila_instrucoes *fila, no_arvore *no);
simbolo * gerar_codigo_conversao_tipo(fila_instrucoes *fila, int tipo, simbolo *s);
simbolo * gerar_codigo_expr_simples(fila_instrucoes *fila, int opcode, t_expr *expr);
simbolo * gerar_codigo_expr(fila_instrucoes *fila, no_arvore *no);
void gerar_codigo_attr(fila_instrucoes *fila, no_arvore *no);
simbolo * gerar_codigo_expr_logica(fila_instrucoes *fila, no_arvore *no);
simbolo * gerar_codigo_indice_array(fila_instrucoes *fila, no_arvore *no);
void gerar_codigo_attr_array(fila_instrucoes *fila, no_arvore *no);
void gerar_codigo_decl_array(tabela_numero *t_numeros, fila_instrucoes *fila, no_arvore *no);
void gerar_codigo_if_else(tabela_numero *t_numeros, fila_instrucoes *fila, no_arvore *no);
void gerar_codigo_while(tabela_numero *t_numeros, fila_instrucoes *fila, no_arvore *no);
void gerar_codigo_funcao(tabela_numero *t_numeros, fila_instrucoes *fila, no_arvore *no);
simbolo * gerar_codigo_chamada_funcao(fila_instrucoes *fila, no_arvore *no);

void imprimir_codigo(fila_instrucoes *fila);

#endif