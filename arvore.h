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

typedef struct t_param {
	int tipo;
	simbolo *variavel;
} t_param;

//Cada construção da linguagem que será 
//compilada deverá ter um struct
typedef struct t_attr {
	simbolo *resultado;
	void *expressao;
} t_attr;

typedef struct t_lista {
	void *dir;
	void *esq;
} t_lista;

typedef struct t_chamada_funcao {
	simbolo *nome;
	void *args;
} t_chamada_funcao;

typedef struct t_escreva {
	void *args;
} t_escreva;

typedef struct t_leia {
	simbolo *variavel;
} t_leia;

typedef struct t_funcao {
	int tipo;
	simbolo *nome;
	t_lista *params;
	void *bloco;
} t_funcao;

typedef struct t_decl_array {
	int tipo;
	simbolo *nome;
	void *tamanho;
	void *valores_iniciais;
} t_decl_array;

typedef struct t_attr_array {
	simbolo *nome;
	void *indice;
	void *valor;
} t_attr_array;

//Simula a superclasse abstrata 
typedef union valor_sintatico {
	t_expr *expr;
	t_expr_logica *exprlogica;
	t_param *param;
	t_attr *attr;
	t_chamada_funcao *chamadafuncao;
	t_escreva *escreva;
	t_leia *leia;
	t_funcao *funcao;
	t_decl_array *declarray;
	t_lista *lista;
	t_attr_array *attrarray;
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

no_arvore * criar_no_t_lista(void *dir, void *esq);
t_lista * criar_t_lista(void *dir, void *esq);

no_arvore * criar_no_chamada_funcao(simbolo *nome, void *args);
t_chamada_funcao * criar_chamada_funcao(simbolo *nome, void *args);

no_arvore * criar_no_escreva(void *args);
t_escreva * criar_escreva(void *args);

no_arvore * criar_no_leia(simbolo *variavel);
t_leia * criar_leia(simbolo *variavel);

no_arvore * criar_no_param(int tipo, simbolo *variavel);
t_param * criar_param(int tipo, simbolo *variavel);

no_arvore * criar_no_funcao(int tipo, simbolo *nome, t_lista *params, void *bloco);
t_funcao * criar_funcao(int tipo, simbolo *nome, t_lista *params, void *bloco);

no_arvore * criar_no_decl_array(int tipo, simbolo *nome, void *tamanho, void *valores_iniciais);
t_decl_array * criar_decl_array(int tipo, simbolo *nome, void *tamanho, void *valores_iniciais);

no_arvore * criar_no_attr_array(simbolo *nome, void *indice, void *valor);
t_attr_array * criar_attr_array(simbolo *nome, void *indice, void *valor);

void imprimir_pos_ordem(no_arvore *no);

#endif
