#ifndef funcoes_h
#define funcoes_h



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// declaração de constantes que representem o tipo da embarcação, o jogador e a orientação da jogada
#define porta_avioes  1
#define destroyer 	  2
#define fragata 	    3
#define submarino 	  4
#define jangada 	    5

#define humano 		    1
#define computador 	  2

#define horizontal    0
#define vertical      1

// criação da estrutura referente à embarcação, que armazena seu tipo, jogador e quantidade de vidas restantes
typedef struct embarcacao{
  int tipo;
  int jogador;
  int vida;
}embarcacao;

// criação da estrutura referente a uma casa do tabuleiro, com ponteiros para as casas aos seus lados, os caracteres que serão exibidos em ambos os tabuleiros e os ponteiros que apontam para as embarcações de ambos os jogadores
typedef struct casa{
  char info_hum;
  char info_com;
  struct embarcacao *emb_hum;
  struct embarcacao *emb_com;
  struct casa *cima;
  struct casa *baixo;
  struct casa *esq;
  struct casa *dir;
}casa;

casa **criar_tabuleiro();

void printar_tabuleiro(casa**, casa**, casa**, char []);

int criar_embarcacao(int, int, casa*, int);

void distribuir_embarcacoes(casa**, casa**, casa**);

// Funcao que trata a entrada do humano
int tratar_entrada (char [], int []);

int ataque(casa*, int, casa**, casa**, casa **, casa **);

int jogada_computador(casa **, casa **, casa **, casa **, casa **, int *, char []);

int jogada_humano(casa **, casa **, casa **, casa **, casa **, char [], int);

// Funcao que identifica a ultima casa atacada por cada jogador
void ultima_jogada(casa **, casa **, char jogada[4]);

#endif