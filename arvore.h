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

typedef struct no_decl {
	no_arvore *dado; // t_attr
	struct no_decl *proximo; 
} no_decl;

typedef struct t_lista_decl {
	int tipo; // int ou float
	no_decl *primeiro;
} t_lista_decl;

//Simula a superclasse abstrata 
typedef union valor_sintatico {
	t_expr *expr;
	t_expr_logica *exprlogica;
	t_attr *attr;
	t_lista_decl *decl;
} valor_sintatico;

typedef struct no_arvore {
	int tipo; //expr, attr, stmt, ...
	valor_sintatico dado;
	//ponteiro para tabela de símbolos
} no_arvore;

no_arvore * criar_no_expressao_logica(int op, void *dir, void *esq);
t_expr_logica * criar_expressao_logica(int op, void *dir, void *esq);

no_arvore * criar_no_expressao(int op, void *dir, void *esq);
t_expr * criar_expressao(int op, void *dir, void *esq);

no_arvore * criar_no_atribuicao(simbolo *resultado, void *expressao);
t_attr * criar_atribuicao(simbolo *resultado, void *expressao);

no_arvore * criar_no_declaracao(int tipo, lista_var *lista);
t_lista_decl * criar_declaracao(void *variavel);

void imprimir_pos_ordem(no_arvore *no);

#endif
