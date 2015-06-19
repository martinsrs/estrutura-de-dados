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


/***********************************************/
/* Definicao das Funcoes                       */
/***********************************************/

void tutorial();   // tutorial do jogo

int sortear_cor();
int jogar(RECORDES** j);   // inicia novo jogo
void imprime_jogada(JOGADAS* lista_jogadas, int segundos);
void compara_jogada(JOGADAS* computador, JOGADAS* jogador);
void sumario_fim_de_jogo(JOGADAS* computador, JOGADAS* jogador, int pontos);

//funcoes para controlar os recordes
void entrada_dados(RECORDES* recordes, int pontos);
void inclui_recorde(RECORDES ** recordes, int pontos); //adiciona um recorde
void mostra_recordes(RECORDES * jogadores); // informa recordes do jogo

void cor(char * cor) {
	system(cor);
}

/*
 * Programa Principal
 */
int main(void) {
	int op;       // opcao do menu
	int pontos = 0;

	RECORDES* recordes; // lista de recordes
	while (1) {
		clear_screen();
		printf("\n /----------------------------------------------------/");
		printf("\n Jogo Genius - Menu de opcoes                          ");
		printf("\n [1]   Jogar                                           ");
		printf("\n [2]   Recordes                                        ");
		printf("\n [3]   Tutorial                                        ");
		printf("\n [0]   Sair do jogo                                    ");
		printf("\n /----------------------------------------------------/");
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

		case 0: // fecha o programa
			exit(1);
			break;

		default:
			printf("\n Digite uma opcao!");
			break;
		} // switch( op )

		fflush( stdin); // limpa buffer do teclado, funciona junto com entrada de dados
		getchar();       // parada da tela

		printf("\n");
	} // fim do while( 1 )

} // fim do programa principal

/*
 * Sorteia uma cor aleatoria para a jogada do computador
 */
int sortear_cor() {
	int cor_sorteada = 0; // armazena o codigo da cor randomica
	srand(time(NULL)); // inicia o random e nao faz sempre buscar a mesma sequencia
	cor_sorteada = rand() % MAXNUMCORES + 1; // sorteia uma cor, evita o 0

	return cor_sorteada;
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
		inclui_jogada(&lista_computador, sortear_cor());
		imprime_jogada(lista_computador, 1);

		p = lista_computador;
		int c = 1;
		int cor_jogador;

		while (p != NULL) {
			int tempoIni = time(NULL);
			printf("Cor num %d: ", c++);
			scanf("%d", &cor_jogador);
			int tempoFim = time(NULL);
			int seg = tempoFim - tempoIni;

			// verifica se estourou o tempo
			if (seg >= LIMITE_TEMPO) {
				printf("TIME IS UP!!! %d ", seg);
				sumario_fim_de_jogo(lista_computador, lista_jogador, pontos);
				getchar();

				return pontos;
			} else {
				// se nao passar o tempo verifica a cor.
				inclui_jogada(&lista_jogador, cor_jogador);
				if (cor_jogador == p->cor) {
					pontos++;
					p = p->prox;
				} else {
					// fim de jogo
					sumario_fim_de_jogo(lista_computador, lista_jogador, pontos);
					getchar();
					return pontos;
				}
			}
			clear_screen();
		}
		getchar();
	}
}

/**
 * Imprime as cores das jogadas
 */
void imprime_jogada(JOGADAS* lista_jogadas, int segundos) {

	if (lista_jogadas == NULL) // caso a lista esteja vazia
		printf("\n sem jogadas!");
	else {
		clear_screen();
		int cont = 1;
		while (lista_jogadas != NULL) {    // ponteiro auxiliar para a lista
			printf("%d-[ %d ]", cont++, lista_jogadas->cor);
			lista_jogadas = lista_jogadas->prox;  // aponta para o proximo registro da lista
			fflush(stdout);
			wait(1);
			clear_screen();
			fflush(stdout);
		} // fim while( aux != NULL )
	} // fim if( aux == NULL )

	clear_screen();
}

/*
 * imprime as jogadas do computador X jogador
 */
void compara_jogada(JOGADAS* computador, JOGADAS* jogador) {

	int c1 = 1;

	printf("\nJogadas do Computador: \n");
	while (computador != NULL) {
		printf("%d-[ %d ] ", c1++, computador->cor);
		computador = computador->prox;
	}

	int c2 = 1;
	printf("\n\nJogadas do Jogador: \n");
	while (jogador != NULL) {
		printf("%d-[ %d ] ", c2++, jogador->cor);
		jogador = jogador->prox;
	}
	fflush(stdout);
}

/*
 * imprime o sumario de final de jogo
 *  - jogadas do computador X jogador
 *  - pontuação
 */
void sumario_fim_de_jogo(JOGADAS* computador, JOGADAS* jogador, int pontos) {
    printf("\n----------------------------------------------------");
	printf("\nFIM DE JOGO !!!\n\n");

	compara_jogada(computador, jogador);

	printf("\n\nPONTUACAO: %d PONTOS!", pontos);
	printf("\n----------------------------------------------------");
}


void inclui_jogada(JOGADAS ** jogadas, int cor) {
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



/*
 * tutorial
 * objetivo: rotina para inicializar novo jogo
 */
void tutorial() {
	printf("\n Bem vindo ao jogo Genius! ");
	printf("\n\n TUTORIAL: ");
	printf(
			"\n\n O jogo consiste em o jogador acertar o mximo de cores que sao apresentadas ");
	printf("\n\n na tela. ");
	printf("\n\n O jogador tem 9 segundos para cada jogada. ");
	printf("\n\n Cada cor acertada acumula pontos para um ranking mostrado ao final do jogo. ");
	printf("\n\n As cores sao definidas por nome / numero como: ");
	printf("\n\n Tecla [1] Amarelo ");
	printf("\n\n Tecla [2] Azul ");
	printf("\n\n Tecla [3] Verde ");
	printf("\n\n Tecla [4] Vermelho ");
	printf("\n\n Divirta-se e boa sorte! ");

	getchar();
}

/*
 * Entrada de dados na lista de recordes
 * entrada: Recordes, pontos
 */
void entrada_dados(RECORDES* recordes, int pontos) {
	fflush(stdin);
	printf("\nDigite seu nome: ");
	scanf(STR, recordes->info.nome);
	recordes->info.pontos = pontos;
	recordes->prox = NULL;
}

/*
 * Inclui novo recorde no TOP 5
 * entrada: recordes, pontos
 */
void inclui_recorde(RECORDES ** recordes, int pontos) {
	RECORDES* no = (RECORDES*) malloc(sizeof(RECORDES));
	RECORDES* pos = *recordes;
	RECORDES* ant;


	//ordena_selecao(recordes); // ordenar a lista antes de executar o metodo

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

	// limita em apenas 5 posições: MAX_RECORDES
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
	printf("TOP %d RECORDES: \n", MAX_RECORDES);

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
	getchar();

}
