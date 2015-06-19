/*
 * main.c
 *
 *  Created on: Jun 18, 2015
 *      Author: eduardo
 */

#include <stdio.h>
#include <stdlib.h>

#if defined(_WIN64)
#include <conio.h>
#elif defined(_WIN32)
#include <conio.h>
#endif

typedef struct info {
	int codigo;
} INFORMACAO;

typedef struct arv {
	INFORMACAO info;
	struct arv* sube;
	struct arv* subd;
} ARVORE;

void insere(ARVORE** r) {
	ARVORE* p;
	ARVORE* a;
	int valor;

	printf("digite um valor: ");

}

void main() {
	printf("hello world");
}
