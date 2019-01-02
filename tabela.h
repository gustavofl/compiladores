#ifndef TABELA_H
#define TABELA_H

#include <string.h>

typedef struct no_lexema_usado {
	char *lexema;
	struct no_lexema_usado *proximo;
} no_lexema_usado;

typedef struct tabela_lexemas_usados {
	no_lexema_usado *primeiro;
} tabela_lexemas_usados;


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
	simbolo *s;
	struct buffer *proximo;
} buffer;

typedef struct fila_buffer{
	buffer *primeiro;
} fila_buffer;

// usado para resolver conflitos de declaracoes de variaveis e funcoes
typedef struct id_error {
	int tipo;
	simbolo *lexema;
	// int linha;
	struct id_error *proximo;
} id_error;

typedef struct fila_id_error {
	id_error *primeiro;
} fila_id_error;

void add_lexema_usado(tabela_lexemas_usados *t, char *lexema);
no_lexema_usado * buscar_lexema_usado(tabela_lexemas_usados *t, char *lexema);

void inserir_numero(tabela_numero *t, numero *n);
numero * localizar_numero (tabela_numero *t, char *lexema, int tipo);
numero * criar_numero (char *lexema, int tipo);
int numeros_iguais(numero *n1, numero *n2);

void inserir_simbolo(tabela *t, simbolo *s);
simbolo * localizar_simbolo (tabela *contexto, char *lexema);
simbolo * localizar_simbolo_contexto (tabela *contexto, char *lexema);
simbolo *  criar_simbolo (char *lexema, int tipo);

void add_buffer(fila_buffer *fila, simbolo *s);
simbolo *pop_buffer(fila_buffer *fila);

pilha_contexto* empilhar_contexto(pilha_contexto *pilha, tabela *contexto);
void desempilhar_contexto(pilha_contexto **pilha);
tabela* topo_pilha(pilha_contexto *pilha);
tabela * criar_contexto(tabela *pai);

void add_id_error(int tipo, simbolo *lexema, /*int linha*/);

void imprimir_contexto(tabela *t);
void imprimir_tabela_numeros(tabela_numero *t);

#endif
