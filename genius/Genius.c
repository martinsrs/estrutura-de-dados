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
#define ACERTO 10
#define LIMITE_TEMPO 9

typedef struct {          // criando tipo de estrutura
	int pontos;  // total de pontos
	char nome[30];
} JOGADOR;

typedef struct list {
	JOGADOR jogador;   // dados do jogador
	struct list* prox; // ponteiro para o proximo registro
} JOGADORES;

typedef struct jogada {
	int cor;   // cor da jogada
	struct jogada* prox; // ponteiro para o proximo registro
} JOGADAS;

/***********************************************/
/* Definicao das Funcoes                       */
/***********************************************/
int sortear_cor();

void jogar(JOGADORES** j);   // inicializa novo jogo com NULL
void placar(JOGADORES *l);   // informa recordes do jogo
void info_jogo();   // tutorial do jogo
void inicia_jogadas(JOGADAS ** pc);
int imprime_jogadas(JOGADAS* lista_jogadas, int segundos);

void cor(char * cor) {
	system(cor);
}

void inicia_jogadas(JOGADAS ** pc) {
	*pc = NULL;
}

/***********************************************/
/* Programa Principal                          */
/***********************************************/
int main(void) {
	int op;       // opcao do menu
	JOGADORES* l; // declaracao da JOGADORES // variavel do tipo JOGADORES = JOGADORES de ponteiros

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
			jogar(&l);
			break;

		case 2:  // informa recordes das jogadas
				 //placar( &l );
			break;

		case 3:  // tutorial do jogo
			info_jogo();
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

/**
 * Imprime as cores das jogadas
 */
int imprime_jogadas(JOGADAS* lista_jogadas, int segundos) {

	int total = 0;

	if (lista_jogadas == NULL) // caso a lista esteja vazia
		printf("\n sem jogadas!");
	else {
		clear_screen();
		int cont = 1;
		while (lista_jogadas != NULL) {    // ponteiro auxiliar para a lista
			printf("%d - [ %d ]", cont++, lista_jogadas->cor);
			lista_jogadas = lista_jogadas->prox;  // aponta para o proximo registro da lista
			fflush(stdout);
			wait(1);
			clear_screen();
			fflush(stdout);
			total++;
		} // fim while( aux != NULL )
	} // fim if( aux == NULL )

	clear_screen();

	return total;
}

int sortear_cor() {
	int cor_sorteada = 0; // armazena o codigo da cor randomica
	srand(time(NULL)); // inicia o random e nao faz sempre buscar a mesma sequencia
	cor_sorteada = rand() % MAXNUMCORES + 1; // sorteia uma cor, evita o 0

	return cor_sorteada;
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

/************************************************
 * jogar                                        *
 * objetivo: rotina para inicializar o jogo     *
 * entrada : JOGADORES                          *
 * saida   : NULL (inicializa JOGADORES)        *
 ************************************************/
void jogar(JOGADORES** j) {

	JOGADAS* lista_computador = NULL;
	JOGADAS* p = NULL;

	int pontos = 0;
	int errou = 0;
	while (!errou) {
		inclui_jogada(&lista_computador, sortear_cor());
		int num_jogadas = imprime_jogadas(lista_computador, 1);

		p = lista_computador;
		int c = 1;
		int cor_jogador;



		while (p != NULL) {

			int tempoIni = time(NULL);
			printf("Cor num %d: ", c++);
			scanf("%d", &cor_jogador);
			int tempoFim = time(NULL);
			int seg = tempoFim - tempoIni;

			if (seg >= LIMITE_TEMPO) {
				printf("TIME IS UP!!! %d ", seg);
				getchar();
				getchar();
				errou = 1;
				break;
			} else {

				if (cor_jogador == p->cor) {
					pontos = pontos + ACERTO;
					p = p->prox;
				} else {
					printf("\n\nERROOOU...");
					printf("\nFIM DE JOGO, PONTUACAO: %d PONTOS!", pontos);
					printf("\nTEMPO TOTAL: %d", seg);

					getchar();
					getchar();

					errou = 1;
					break;
				}

			}
			clear_screen();
		}

		getchar();


	}
}

/************************************************
 * recordes                                    *
 * objetivo: rotina para inicializar novo jogo  *
 * entrada : JOGADORES                              *
 * saida   : NULL (inicializa JOGADORES)            *
 ************************************************/
void placar(JOGADORES* l) {
	//l = NULL; // JOGADORES criada, inicio nao aponta
}

/************************************************
 * tutorial                                     *
 * objetivo: rotina para inicializar novo jogo  *
 * saida   : NULL (inicializa JOGADORES)        *
 ************************************************/
void info_jogo() {
	printf("\n Bem vindo ao jogo Genius! ");
	printf("\n\n TUTORIAL: ");
	printf(
			"\n\n O jogo consiste em o jogador acertar o mximo de cores que sao apresentadas ");
	printf("\n\n na tela. ");
	printf("\n\n O jogador tem 9 segundos para cada jogada. ");
	printf(
			"\n\n Cada cor acertada acumula pontos para um ranking mostrado ao final do jogo. ");
	printf("\n\n As cores sao definidas por nome / numero como: ");
	printf("\n\n Tecla [1] Amarelo ");
	printf("\n\n Tecla [2] Azul ");
	printf("\n\n Tecla [3] Verde ");
	printf("\n\n Tecla [4] Vermelho ");
	printf("\n\n Divirta-se e boa sorte! ");
}

/*************************************************** 
 * ordena_JOGADORES modo selection                     *
 * objetivo: ordenar ranking por pontuacao         *
 * entrada : lista                                 *
 * saida   : lista ordenada por pontuacao          *
 *************************************************** 
 void ordena_selecao ( LISTA ** l)
 {

 /*	LISTA* p; // posicao na lista
 LISTA* q; // posicao para comparacao da lista
 LISTA* menor;  // auxiliar para a guardar menor valor

 JOGADAS aux;  // auxiliar para a troca de dados

 if( *l == NULL ) // verifica se a lista esta vazia
 printf( "\n Lista vazia!" );
 else {
 p = *l;
 while (p->prox != NULL) {
 menor = p;
 q = p->prox;
 while (q != NULL) {
 if (q->info.codigo < menor->info.codigo) {
 menor = q;
 }
 q = q->prox;
 }

 if (p->info.codigo != menor->info.codigo) {
 aux = p->info;
 p->info = menor->info;
 menor->info = aux;
 }

 p = p->prox;
 }
 }

 }*/
