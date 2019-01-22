#ifndef CODIGO_MIPS_H
#define CODIGO_MIPS_H

#include <string.h>
#include "arvore.h"

typedef struct registrador {
	int usado;
	char *lexema;
} registrador;

typedef *registrador lista_regs;

void inicializar_regs(lista_regs lista);

char * obter_registrador_temp();
char * obter_registrador_perm();

void imprimir_mips(fila_instrucoes *fila);

#endif