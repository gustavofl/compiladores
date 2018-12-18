#include <stdlib.h>
#include <stdio.h>
#include "tabela.h"
#include "y.tab.h"

void add_lexema_usado(tabela_lexemas_usados *t, char *lexema) {
	no_lexema_usado *no = buscar_lexema_usado(t, lexema);

	if(no == NULL) {
		no = (no_lexema_usado *) malloc(sizeof(no_lexema_usado));
		no->lexema = lexema;
		no->proximo = t->primeiro;
		t->primeiro = no;
	}
}

no_lexema_usado * buscar_lexema_usado(tabela_lexemas_usados *t, char *lexema) {
	no_lexema_usado *no = t->primeiro;

	while(no != NULL) {
		if(strcmp(no->lexema, lexema) == 0) {
			return no;
		}
		no = no->proximo;
	}

	return NULL;
}

void inserir_numero(tabela_numero *t, numero *n) {
	no_tabela_numero *no = (no_tabela_numero *) malloc(sizeof(no_tabela_numero));
	no->dado = n;
	no->proximo = t->primeiro;
	t->primeiro = no;
}

numero * localizar_numero (tabela_numero *t, char *lexema, int tipo){
	numero *novo_num = criar_numero(lexema, tipo);

	no_tabela_numero *temp = t->primeiro;
	while(temp != NULL) {
		if(numeros_iguais(novo_num, temp->dado)) {
			return temp->dado;
		}
		temp = temp->proximo;
	}

	return NULL;
}

numero * criar_numero (char *lexema, int tipo) {
	numero *novo = (numero *) malloc(sizeof(numero));
	novo->tipo = tipo;
	if(tipo == INT)
		novo->valor_inteiro = atoi(lexema);
	else if(tipo == REAL)
		novo->valor_real = atof(lexema);
	return novo;
}

int numeros_iguais(numero *n1, numero *n2) {
	int resultado;

	if(n1 == NULL || n2 == NULL)
		resultado = 0;
	if(n1->tipo != n2->tipo)
		resultado = 0;
	if(n1->tipo == INT)
		resultado = n1->valor_inteiro == n2->valor_inteiro;
	else if(n1->tipo == REAL)
		resultado = n1->valor_real == n2->valor_real;
	else
		resultado = 0;

	return resultado;
}

void imprimir_tabela_numeros(tabela_numero *t) {
	no_tabela_numero *no = t->primeiro;

	printf("\nTABELA DE NUMEROS:\n");
	while(no != NULL) {
		numero *n = no->dado;
		if(n->tipo == INT)
			printf("INTEIRO: %d\n", n->valor_inteiro);
		else if(n->tipo == REAL)
			printf("REAL: %f\n", n->valor_real);
		no = no->proximo;
	}
	printf("\n");
}

void inserir_simbolo(tabela *t, simbolo *s) {
	no_tabela *no = (no_tabela *) malloc(sizeof(no_tabela));
	no->dado = s;
	no->proximo = t->primeiro;
	t->primeiro = no;
}


simbolo * localizar_simbolo (tabela *contexto, char *lexema){
	tabela *temp = contexto;
	no_tabela *temp2;
	while(temp != NULL) {
		temp2 = temp->primeiro;
		while(temp2 != NULL) {
			if(strcmp(temp2->dado->lexema, lexema) == 0) {
				return temp2->dado;
			}
			temp2 = temp2->proximo;
		}
		temp = temp->pai;
	}
	return NULL;
}

simbolo * localizar_simbolo_contexto (tabela *contexto, char *lexema){
	no_tabela *temp = contexto->primeiro;
	while(temp != NULL) {
		if(strcmp(temp->dado->lexema, lexema) == 0) {
			return temp->dado;
		}
		temp = temp->proximo;
	}
	return NULL;
}

simbolo *  criar_simbolo (char *lexema, int tipo) {
	simbolo *novo = (simbolo *) malloc(sizeof(simbolo));
	novo->tipo = tipo;
	novo->lexema = strdup(lexema);
	return novo;
}

pilha_contexto* empilhar_contexto(pilha_contexto *pilha, tabela *contexto) {
	pilha_contexto *novo = (pilha_contexto *) malloc (sizeof(pilha_contexto));
	novo->anterior = pilha;
	novo->dado = contexto;
	return novo;
}

tabela * criar_contexto(tabela *pai) {
	tabela *novo = (tabela *) malloc(sizeof(tabela));
	novo->pai = pai;
	novo->primeiro = NULL;
	return novo;
}

void desempilhar_contexto(pilha_contexto **pilha) {
	if(*pilha != NULL)
		*pilha = (*pilha)->anterior;
}

tabela* topo_pilha(pilha_contexto *pilha) {
	if(pilha == NULL)
		return NULL;
	else
		return pilha->dado;

}

void add_buffer(fila_buffer *fila, simbolo *s){
	buffer *novo = (buffer *) malloc (sizeof(buffer));
	novo->s = s;
	novo->proximo = fila->primeiro;
	fila->primeiro = novo;
}

simbolo *pop_buffer(fila_buffer *fila){
	buffer *temp = fila->primeiro;
	fila->primeiro = temp->proximo;
	return temp->s;
}

void imprimir_contexto(tabela *t) {
	if(t == NULL)
		printf("a\n");
	no_tabela * temp = t->primeiro;
	printf("----------------------------------\n");

	while(temp != NULL) {
		if(temp->dado->tipo == INT) 
			printf("\t INT: %s\n", temp->dado->lexema);
		else if(temp->dado->tipo == REAL) 
			printf("\t FLOAT: %s\n", temp->dado->lexema);	
		else
			printf("\t UNDEFINED: %s\n", temp->dado->lexema);	
		temp = temp->proximo;
	}

	printf("==================================\n");
}

