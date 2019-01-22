#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "arvore.h"
#include "codigo_intermediario.h"
#include "codigo_mips.h"
#include "y.tab.h"

void inicializar_regs(lista_regs lista) {
	char buffer[3];
	char *var;
	var = "$s";
	for (int j = 0; j < 8; ++j) {
		sprintf(buffer, "%s%d", var, j);
		lista[j].lexema = buffer;
	}
}

char * obter_registrador_temp() {
	char buffer[256];
	sprintf(buffer, "$s0");

	return strdup(buffer);
}

char * obter_registrador_temp() {
	char buffer[256];
	sprintf(buffer, "$t0");

	return strdup(buffer);
}

void imprimir_mips(fila_instrucoes *fila) {
	no_instrucao *no = fila->primeiro;
	instrucao *i;
	char *opcode, *second, *first;
	char buffer[256];

	printf("main:\n\n");

	while(no != NULL) {
		i = no->inst;

		switch(i->opcode) {
			case NUM_INT:
				printf("addi %s, $zero, %d\n", obter_registrador_temp(), ((numero *)i->first)->valor_inteiro);
				break;
			case ATTR:
				printf("addi %s, $zero, %s\n", obter_registrador_perm(), ((simbolo *)i->first)->lexema);
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
			case SE_FALSO:
				opcode = "se_falso";
				break;
			case LABEL:
				opcode = "label";
				break;
			case JUMPER:
				opcode = "jmp";
				break;
			case LER_PARAM:
				opcode = "ler_param";
				break;
			case JMP_RETORNO:
				// printf("(jmp_retorno,-,-,-)\n");
				no = no->proximo;
				continue;
			case SALVAR_PARAM:
				opcode = "salvar_param";
				break;
			case CHAMADA_FUNCAO:
				opcode = "chamada_funcao";
				break;
			case ESCREVA:
				opcode = "escreva";
				break;
			case LEIA:
				opcode = "leia";
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
		else if(i->first == NULL) {
			first = "-";
		}
		else {
			first = ((simbolo *)i->first)->lexema;
		}

		// SEGUNDO OPERANDO
		if(i->second != NULL)
			second = ((simbolo *)i->second)->lexema;
		else
			second = "-";

		// printf("(%s,%s,%s,%s)\n", opcode, i->result->lexema, first, second);

		no = no->proximo;
	}

	printf("\n# finalizar programa\n");
	printf("li $v0, 10\n");
	printf("syscall\n");
}