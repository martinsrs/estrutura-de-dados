/***********************************************/
/* Projeto Genius - Trabalho G1                */
/* objetivo: desenvolver jogo interativo Genius*/
/* programadores: Marcio Bandarra Soares       */
/*                Eduardo Martins              */
/* criado em: 01/06/2015                       */
/* data da ultima alteracao: 08/06/2015        */
/***********************************************/

#include <stdio.h>

#if defined(_WIN64)
#include <conio.h>
#elif defined(_WIN32)
#include <conio.h>
#endif

#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

//Libs adicionais criadas
#include "lib/cls.h"
#include "lib/sleep.h"

/***********************************************/
/* Definicao dos Registros                     */
/***********************************************/
#define MAXNUMCORES 4 // jogo com 4 cores

#define AMARELO "color 0E"
#define AZUL "color 09"
#define VERDE "color 0A"
#define VERMELHO "color 0C"
#define BRANCO "color 0F"

#define COLOR_YELLOW "\e[1;33m"

#define ACERTO 1
#define MAX_RECORDES 5
#define LIMITE_TEMPO 9

#define STR "%[^\n]%*c"
#define MAX_STR 50

typedef struct { // criando tipo de estrutura
	int pontos;  // total de pontos
	char nome[MAX_STR];
} JOGADOR;

typedef struct list {
	JOGADOR info;   // dados do jogador
	struct list* prox; // ponteiro para o proximo registro
} RECORDES;

typedef struct jogada {
	int cor;   // cor da jogada
	struct jogada* prox; // ponteiro para o proximo registro
} JOGADAS;

char * colorName[5];

/***********************************************/
/* Definicao das Funcoes                       */
/***********************************************/


// funcoes de jogo
int sortear_cor();
void salva_jogada(JOGADAS ** jogadas, int cor); // salva jogadas feitas pelo computador
int jogar(RECORDES** j); // inicia novo jogo
void imprime_colorido(int id, int color); // imprime as jogadas com cores
void imprime_jogada(JOGADAS* lista_jogadas, int segundos); // imprime a lista de jogadas ja sorteadas

// funcoes de game over
void compara_jogada(JOGADAS* computador, JOGADAS* jogador); // compara as jogadas ao fim do jogo
void sumario_fim_de_jogo(JOGADAS* computador, JOGADAS* jogador, int pontos, int tempo);

//funcoes para controlar os recordes
void entrada_dados(RECORDES* recordes, int pontos);
void inclui_recorde(RECORDES ** recordes, int pontos); //adiciona um recorde
void mostra_recordes(RECORDES * jogadores); // informa recordes do jogo

void tutorial();   // tutorial do jogo

void cor(char * cor) {
	system(cor);
}

/*
 * Programa Principal
 */
int main(void) {
	int op;       // opcao do menu
	int pontos = 0;

	colorName[1] = "Amarelo";
	colorName[2] = "Azul";
	colorName[3] = "Verde";
	colorName[4] = "Vermelho";

	RECORDES* recordes = NULL; // lista de recordes

	while (1) {
		clear_screen();

		printf("\n      ___           ___           ___                        ___           ___      ");
		printf("\n     /  /\\\         /  /\\\         /  /\\\           ___        /  /\\\         /  /\\\     ");
		printf("\n    /  /::\\\       /  /::\\\       /  /::|         /__/\\\      /  /:/        /  /::\\\    ");
		printf("\n   /  /:/\\\:\\\     /  /:/\\\:\\\     /  /:|:|         \\\__\\\:\\\    /  /:/        /__/:/\\\:\\\   ");
		printf("\n  /  /:/  \\\:\\\   /  /::\\\ \\\:\\\   /  /:/|:|__       /  /::\\\  /  /:/        _\\\_ \\\:\\\ \\\:\\\  ");
		printf("\n /__/:/_\\\_ \\\:\\\ /__/:/\\\:\\\ \\\:\\\ /__/:/ |:| /\\\   __/  /:/\\\/ /__/:/     /\\\ /__/\\\ \\\:\\\ \\\:\\\ ");
		printf("\n \\\  \\\:\\\__/\\\_\\\/ \\\  \\\:\\\ \\\:\\\_\\\/ \\\__\\\/  |:|/:/  /__/\\\/:/~~  \\\  \\\:\\\    /:/ \\\  \\\:\\\ \\\:\\\_\\\/ ");
		printf("\n  \\\  \\\:\\\ \\\:\\\    \\\  \\\:\\\ \\\:\\\       |  |:/:/   \\\  \\\::/      \\\  \\\:\\\  /:/   \\\  \\\:\\\_\\\:\\\   ");
		printf("\n   \\\  \\\:\\\/:/     \\\  \\\:\\\_\\\/       |__|::/     \\\  \\\:\\\       \\\  \\\:\\\/:/     \\\  \\\:\\\/:/   ");
		printf("\n    \\\  \\\::/       \\\  \\\:\\\         /__/:/       \\\__\\\/        \\\  \\\::/       \\\  \\\::/    ");
		printf("\n     \\\__\\\/         \\\__\\\/         \\\__\\\/                      \\\__\\\/         \\\__\\\/     ");


		printf("\n\n--------------------------------------------\n");
		printf("| %-40s |\n", "[1]   Jogar");
		printf("| %-40s |\n", "[2]   Recordes");
		printf("| %-40s |\n", "[3]   Tutorial");
		printf("| %-40s |\n", "[0]   Sair do jogo");
		printf("--------------------------------------------\n");

		printf("\n Opcao: ");
		scanf("%d", &op); // tecla de opcao do menu

		switch (op) {
		case 1: // nova jogada
			pontos = jogar(&recordes); // faz as jogadas e retorna o total de pontos
			inclui_recorde(&recordes, pontos); // salva os recordes
			break;

		case 2:  // informa recordes das jogadas
			mostra_recordes( recordes );
			break;

		case 3:  // tutorial do jogo
			tutorial();
			break;

		case 0: // sair
			exit(1);
			break;

		default:
			printf("\n Digite uma opcao:");
			break;
		}

		fflush( stdin); // limpa buffer do teclado, funciona junto com entrada de dados
		getchar();       // parada da tela

		printf("\n");
	}
}

/*
 * Sorteia uma cor aleatoria para a jogada do computador
 */
int sortear_cor() {
	int cor_sorteada = 0; // armazena o codigo da cor randomica
	srand(time(NULL)); // inicia o random e nao faz sempre buscar a mesma sequencia
	cor_sorteada = rand() % MAXNUMCORES + 1; // sorteia uma cor, evita o 0

	return cor_sorteada;
}

/*
 * Salva as jogadas feitas pelo computador na lista para exibir
 */
void salva_jogada(JOGADAS ** jogadas, int cor) {
	JOGADAS* p = NULL;
	JOGADAS* rodada = (JOGADAS *) malloc(sizeof(JOGADAS)); // aloca novo espaco em memoria para rodadas

	if (rodada != NULL) {
		rodada->cor = cor;
		rodada->prox = NULL;

		if (*jogadas == NULL) {
			*jogadas = rodada;
		} else {
			p = *jogadas;
			while (p->prox != NULL) {
				p = p->prox;
			}
			p->prox = rodada; // ultimo aponta para o novo registro
		}
	}
}

/************************************************
 * jogar                                        *
 * objetivo: rotina para inicializar o jogo     *
 * entrada : RECORDES                          *
 * saida   : INT - total de pontos              *
 ************************************************/
int jogar(RECORDES** recordes) {

	JOGADAS* lista_computador = NULL;
	JOGADAS* p = NULL;

	int pontos = 0;
	while (1) {
		JOGADAS* lista_jogador    = NULL;
		salva_jogada(&lista_computador, sortear_cor());
		imprime_jogada(lista_computador, 1);

		p = lista_computador;
		int c = 1;
		int cor_jogador;

		while (p != NULL) {
			int tempoIni = time(NULL);

			printf("\nUtilize os numeros correspondentes para jogar");
			printf("\n\e[1;33m┌───┐   ┌─┐┌┬┐┌─┐┬─┐┌─┐┬  ┌─┐  \e[1;34m┌───┐   ┌─┐┌─┐┬ ┬┬    \e[1;32m┌───┐   ┬  ┬┌─┐┬─┐┌┬┐┌─┐  \e[1;31m┌───┐   ┬  ┬┌─┐┬─┐┌┬┐┌─┐┬  ┬ ┬┌─┐");
			printf("\n\e[1;33m│ 1 │───├─┤│││├─┤├┬┘├┤ │  │ │  \e[1;34m│ 2 │───├─┤┌─┘│ ││    \e[1;32m│ 3 │───└┐┌┘├┤ ├┬┘ ││├┤   \e[1;31m│ 4 │───└┐┌┘├┤ ├┬┘│││├┤ │  ├─┤│ │");
			printf("\n\e[1;33m└───┘   ┴ ┴┴ ┴┴ ┴┴└─└─┘┴─┘└─┘  \e[1;34m└───┘   ┴ ┴└─┘└─┘┴─┘  \e[1;32m└───┘    └┘ └─┘┴└──┴┘└─┘  \e[1;31m└───┘    └┘ └─┘┴└─┴ ┴└─┘┴─┘┴ ┴└─┘");
			printf("\e[0;37m");
			printf("\n");

			printf("\nCor num %d: ", c++);
			scanf("%d", &cor_jogador);
			int tempoFim = time(NULL);
			int seg = tempoFim - tempoIni;

			// verifica se estourou o tempo
			if (seg >= LIMITE_TEMPO) {
				sumario_fim_de_jogo(lista_computador, lista_jogador, pontos, seg);
				getchar();
				return pontos;
			} else {
				// se nao passar o tempo verifica a cor.
				salva_jogada(&lista_jogador, cor_jogador);
				if (cor_jogador == p->cor) {
					pontos++;
					p = p->prox;
				} else {
					// fim de jogo
					sumario_fim_de_jogo(lista_computador, lista_jogador, pontos, seg);
					getchar();
					return pontos;
				}
			}
			clear_screen();
		}
		getchar();
	}
}

/*
 * faz a impressão colorida dos numeros
 */
void imprime_colorido(int id, int color) {
	/*
	 * COLOR_GRAY  = \e[0;37m
	 * 1 COLOR_YELLOW =\e[1;33m
	 * 2 COLOR_BLUE   =\e[0;34m
	 * 3 COLOR_GREEN  ='\e[0;32m
	 * 4 COLOR_RED    =\e[0;31m
	 */

	printf("\n %d : ", id);
	switch (color) {
		case 1:

			printf("\e[1;33m");
			printf("\n ┌───┐  ┌─┐┌┬┐┌─┐┬─┐┌─┐┬  ┌─┐  ");
			printf("\n │ 1 │  ├─┤│││├─┤├┬┘├┤ │  │ │  ");
			printf("\n └───┘  ┴ ┴┴ ┴┴ ┴┴└─└─┘┴─┘└─┘  ");

			printf("\e[0;37m");
			break;
		case 2:
			printf("\e[1;34m");
			printf("\n ┌───┐  ┌─┐┌─┐┬ ┬┬   ");
			printf("\n │ 2 │  ├─┤┌─┘│ ││   ");
			printf("\n └───┘  ┴ ┴└─┘└─┘┴─┘ ");
			printf("\e[0;37m");
			break;
		case 3:
			printf("\e[1;32m");
			printf("\n ┌───┐  ┬  ┬┌─┐┬─┐┌┬┐┌─┐ ");
			printf("\n │ 3 │  └┐┌┘├┤ ├┬┘ ││├┤  ");
			printf("\n └───┘   └┘ └─┘┴└──┴┘└─┘ ");
			printf("\e[0;37m");
			break;
		case 4:
			printf("\e[1;31m");
			printf("\n ┌───┐  ┬  ┬┌─┐┬─┐┌┬┐┌─┐┬  ┬ ┬┌─┐  ");
			printf("\n │ 4 │  └┐┌┘├┤ ├┬┘│││├┤ │  ├─┤│ │  ");
			printf("\n └───┘   └┘ └─┘┴└─┴ ┴└─┘┴─┘┴ ┴└─┘  ");
			printf("\e[0;37m");
			break;
		default:
			break;
	}
}

/**
 * Imprime as cores das jogadas
 */
void imprime_jogada(JOGADAS* lista_jogadas, int segundos) {

	if (lista_jogadas != NULL) { // verificar se existem jogadas a serem listadas
		clear_screen();
		int cont = 1;
		while (lista_jogadas != NULL) {    // ponteiro auxiliar para a lista

			imprime_colorido(cont++, lista_jogadas->cor);

			lista_jogadas = lista_jogadas->prox;  // aponta para o proximo registro da lista
			fflush(stdout);
			wait(1);
			clear_screen();
			fflush(stdout);
		}
	}

	clear_screen();
}

/*
 * imprime as jogadas do computador X jogador
 */
void compara_jogada(JOGADAS* computador, JOGADAS* jogador) {

	int c1 = 1;

	printf("\nJogadas do Computador: \n");
	while (computador != NULL) {
		printf("%d-[ %s ] ", c1++, colorName[computador->cor]);
		computador = computador->prox;
	}

	int c2 = 1;
	printf("\n\nJogadas do Jogador: \n");
	while (jogador != NULL) {

		if ((jogador->cor >=1) && (jogador->cor <= 4)) {
			printf("%d-[ %s ] ", c2++, colorName[jogador->cor]);
		} else {
			printf("%d-[ %s ] ", c2++, "invalida");
		}

		jogador = jogador->prox;
	}
	fflush(stdout);
}

/*
 * imprime o sumario de final de jogo
 *  - jogadas do computador X jogador
 *  - pontuação
 */
void sumario_fim_de_jogo(JOGADAS* computador, JOGADAS* jogador, int pontos, int tempo) {
	clear_screen();
	printf("\n┌─┐┌─┐┌┬┐┌─┐  ┌─┐┬  ┬┌─┐┬─┐┬");
	printf("\n│ ┬├─┤│││├┤   │ │└┐┌┘├┤ ├┬┘│");
	printf("\n└─┘┴ ┴┴ ┴└─┘  └─┘ └┘ └─┘┴└─o");
    printf("\n-----------------------------");

    if (tempo >= LIMITE_TEMPO) {
    	printf("\n\n*** TEMPO EXCEDIDO!! %d s ***\n", tempo);
    }

    compara_jogada(computador, jogador);

	printf("\n\nPONTUACAO: %d PONTOS!", pontos);
	printf("\n-----------------------------");
}

/*
 * Entrada de dados na lista de recordes
 * entrada: Recordes, pontos
 */
void entrada_dados(RECORDES* recordes, int pontos) {
	fflush(stdin);
	printf("\n\nColoque seu nome no mural dos recordes! ");
	printf("\nDigite seu nome: ");
	scanf(STR, recordes->info.nome);
	recordes->info.pontos = pontos;
	recordes->prox = NULL;
}

/*
 * Inclui novo recorde no TOP 5
 * inclui ordenado - modo decrescente
 * entrada: recordes, pontos
 */
void inclui_recorde(RECORDES ** recordes, int pontos) {

	RECORDES* no = (RECORDES*) malloc(sizeof(RECORDES));
	RECORDES* pos = *recordes;
	RECORDES* ant;

	if (no != NULL) {
		entrada_dados(no, pontos);

		while ((pos != NULL) && (no->info.pontos < pos->info.pontos)) {
			ant = pos;
			pos = pos->prox;
		}

		if (pos == *recordes) {
			*recordes = no;
		} else {
			ant->prox = no;
		}
		no->prox = pos;
	}

	// limita em apenas 5 posições: MAX_RECORDES, excluindo as posições invalidas
	int count = 1;
	pos = *recordes;
	while ((count <= MAX_RECORDES) && (pos != NULL)) {
		if (count == MAX_RECORDES) {
			pos->prox = NULL;
		}
		pos = pos->prox;
		count++;
	}

}

/*
 * Mostra a lista TOP 5 Recordes
 * objetivo: rotina para inicializar novo jogo
 * entrada : RECORDES
 */
void mostra_recordes(RECORDES * recordes) {

	clear_screen();

	int rank = 1;

	printf("\n┬─┐┌─┐┌─┐┌─┐┬─┐┌┬┐┌─┐┌─┐");
	printf("\n├┬┘├┤ │  │ │├┬┘ ││├┤ └─┐");
	printf("\n┴└─└─┘└─┘└─┘┴└──┴┘└─┘└─┘");

	printf("\nTOP %d RECORDES: \n", MAX_RECORDES);

	printf("\n------------------------------------------------------------\n");
	printf("| %-10s |", "POSICAO");
	printf(" %-30s |", "NOME");
	printf(" %-10s |", "PONTOS");
	printf("\n------------------------------------------------------------\n");

	while (recordes != NULL) {

		printf("| %-10d |", rank++);
		printf(" %-30s |", recordes->info.nome);
		printf(" %-10d |", recordes->info.pontos);
		printf("\n------------------------------------------------------------\n");

		recordes = recordes->prox;
	}

	getchar();

}

/*
 * tutorial do jogo
 * objetivo informar as regras do jogo
 */
void tutorial() {

	clear_screen();

	printf("\n\n┌┬┐┬ ┬┌┬┐┌─┐┬─┐┬┌─┐┬  ");
	printf("\n │ │ │ │ │ │├┬┘│├─┤│  ");
	printf("\n ┴ └─┘ ┴ └─┘┴└─┴┴ ┴┴─┘\n\n");

	printf("\nO Jogo consiste em o jogador repetir o maximo de vezes a mesma sequencia de cores apresentada na tela");
	printf("\nO jogador tera ate 9 segundos para informar cada cor da sequencia");
	printf("\nCada cor acertada na sequencia aculula 1 ponto");
	printf("\nApós terminar a jogada o jogador deve informar o seu Nome para adicionar ao ranking");
	printf("\n\nAs cores serao definidas através dos numeros:\n");

	printf("\n");
	printf("\e[1;33m");
	printf("\n ┌───┐  ┌─┐┌┬┐┌─┐┬─┐┌─┐┬  ┌─┐      ");
	printf("\n │ 1 │  ├─┤│││├─┤├┬┘├┤ │  │ │      ");
	printf("\n └───┘  ┴ ┴┴ ┴┴ ┴┴└─└─┘┴─┘└─┘      ");
	printf("\e[1;34m");
	printf("\n ┌───┐  ┌─┐┌─┐┬ ┬┬                 ");
	printf("\n │ 2 │  ├─┤┌─┘│ ││                 ");
	printf("\n └───┘  ┴ ┴└─┘└─┘┴─┘               ");
	printf("\e[1;32m");
	printf("\n ┌───┐  ┬  ┬┌─┐┬─┐┌┬┐┌─┐           ");
	printf("\n │ 3 │  └┐┌┘├┤ ├┬┘ ││├┤            ");
	printf("\n └───┘   └┘ └─┘┴└──┴┘└─┘           ");
	printf("\e[1;31m");
	printf("\n ┌───┐  ┬  ┬┌─┐┬─┐┌┬┐┌─┐┬  ┬ ┬┌─┐  ");
	printf("\n │ 4 │  └┐┌┘├┤ ├┬┘│││├┤ │  ├─┤│ │  ");
	printf("\n └───┘   └┘ └─┘┴└─┴ ┴└─┘┴─┘┴ ┴└─┘  ");
	printf("\e[0;37m");
	printf("\n\n\n");
	printf("\n  ┌┐ ┌─┐┌┬┐   ┬┌─┐┌─┐┌─┐┬");
	printf("\n  ├┴┐│ ││││   ││ ││ ┬│ ││");
	printf("\n  └─┘└─┘┴ ┴  └┘└─┘└─┘└─┘o");

	getchar();
}
