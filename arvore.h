#ifndef ARVORE_H
#define ARVORE_H

#include <string.h>
#include "tabela.h"
#include "y.tab.h"

typedef struct t_expr_logica {
	int op;
	void *dir, *esq; 
} t_expr_logica;

typedef struct t_expr {
	int op; // + - / * UMINUS ID NUMERO
	int tipo; //float ou int
	void *dir, *esq;
} t_expr;

//Cada construção da linguagem que será 
//compilada deverá ter um struct
typedef struct t_attr {
	simbolo *resultado;
	void *expressao;
} t_attr;

typedef struct t_lista_attr {
	void *dir;
	void *esq;
} t_lista_attr;

typedef struct t_lista_arg {
	void *dir;
	void *esq;
} t_lista_arg;

typedef struct t_funcao {
	simbolo *nome;
	void *args;
} t_funcao;

typedef struct t_escreva {
	void *args;
} t_escreva;

typedef struct t_leia {
	simbolo *variavel;
} t_leia;

//Simula a superclasse abstrata 
typedef union valor_sintatico {
	t_expr *expr;
	t_expr_logica *exprlogica;
	t_attr *attr;
	t_lista_arg *lista_arg;
	t_funcao *funcao;
	t_lista_attr *t_attrlista;
	t_escreva *escreva;
	t_leia *leia;
} valor_sintatico;

typedef struct no_arvore {
	int tipo; //expr, attr, stmt, attrlista, arglista, funcao
	valor_sintatico dado;
	//ponteiro para tabela de símbolos
} no_arvore;

no_arvore * criar_no_expressao_logica(int op, void *dir, void *esq);
t_expr_logica * criar_expressao_logica(int op, void *dir, void *esq);

no_arvore * criar_no_expressao(int op, void *dir, void *esq);
t_expr * criar_expressao(int op, void *dir, void *esq);

no_arvore * criar_no_atribuicao(simbolo *resultado, void *expressao);
t_attr * criar_atribuicao(simbolo *resultado, void *expressao);

no_arvore * criar_no_t_lista_attr(void *dir, void *esq);
t_lista_attr * criar_t_lista_attr(void *dir, void *esq);

no_arvore * criar_no_lista_arg(void *dir, void *esq);
t_lista_arg * criar_lista_arg(void *dir, void *esq);

no_arvore * criar_no_funcao(simbolo *nome, void *args);
t_funcao * criar_funcao(simbolo *nome, void *args);

no_arvore * criar_no_escreva(void *args);
t_escreva * criar_escreva(void *args);

no_arvore * criar_no_leia(simbolo *variavel);
t_leia * criar_leia(simbolo *variavel);

void imprimir_pos_ordem(no_arvore *no);

#endif
