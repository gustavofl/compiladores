#ifndef TABELA_H
#define TABELA_H

#include <string.h>

typedef struct numero {
	int tipo;
	int valor_inteiro;
	float valor_real;
} numero;

typedef struct no_tabela_numero {
	numero *dado;
	struct no_tabela_numero *proximo;
} no_tabela_numero;

typedef struct tabela_numero {
	no_tabela_numero *primeiro;
} tabela_numero;


typedef struct simbolo {
	int tipo;
	char *lexema;
} simbolo;

typedef struct no_tabela {
	simbolo *dado;
	struct no_tabela *proximo;
} no_tabela;

typedef struct tabela {
	no_tabela *primeiro;
	struct tabela *pai;
} tabela;

typedef struct pilha_contexto  {
	tabela *dado;
	struct pilha_contexto *anterior;
} pilha_contexto;

// usado na atribuicao de variaveis em declaracoes multiplas
typedef struct buffer{
	int tipo; // ID NO_ARVORE
	void *dado;
	struct buffer *proximo;
} buffer;

typedef struct fila_buffer{
	buffer *primeiro;
} fila_buffer;

void inserir_numero(tabela_numero *t, numero *n);
numero * localizar_numero (tabela_numero *t, char *lexema, int tipo);
numero * criar_numero (char *lexema, int tipo);
int numeros_iguais(numero *n1, numero *n2);

void inserir_simbolo(tabela *t, simbolo *s);
simbolo * localizar_simbolo (tabela *contexto, char *lexema);
simbolo * localizar_simbolo_contexto (tabela *contexto, char *lexema);
simbolo *  criar_simbolo (char *lexema, int tipo);

void add_buffer(fila_buffer *fila, int tipo, void *dado);
buffer *pop_buffer(fila_buffer *fila);

pilha_contexto* empilhar_contexto(pilha_contexto *pilha, tabela *contexto);
void desempilhar_contexto(pilha_contexto **pilha);
tabela* topo_pilha(pilha_contexto *pilha);
tabela * criar_contexto(tabela *pai);

void imprimir_contexto(tabela *t);
void imprimir_tabela_numeros(tabela_numero *t);

#endif
