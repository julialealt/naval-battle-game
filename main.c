/* Grupo:
  Gabriel Lucas Silva Felix - 509804
  Júlia Teixeira - 514094
  Yasmin Dutra Pinheiro - 509894
*/




// Chamada de bibliotecas
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "funcoes.h"


int main(void){
  // Declaração de Variáveis
  casa **matriz;
  casa *pos_submarino_hum[] = {NULL, NULL}, *pos_submarino_com[] = {NULL, NULL};
  casa *jogadas_com[200], *jogadas_hum[200];
  char entrada_s[7], jogada[4];
  int linha, coluna, emb_humano[] = {1,2,2,3,3,3,4,4,5}, emb_computador[] = {1,2,2,3,3,3,4,4,5}, atacado, i=0, hum_ganhou=0, com_ganhou=0, retorno_jogada_com[] = {0,0,0,0,0}, retorno_ataque_com = 0, placar_com=0, placar_hum=0, continuar=1, emb_afundada_hum=0, emb_afundada_com=0;

  // Atribuindo NULL a todos os espacos do vetor que armazena as jogadas de ambos os jogadores
  for (int i=0; i<200; i++){
    jogadas_hum[i] = NULL;
    jogadas_com[i] = NULL;
  }

  // Criação da matriz que representa os tabuleiros
  matriz = criar_tabuleiro();
  // Preenchimento da matriz dos tabuleiros com as respectivas embarcacoes
  distribuir_embarcacoes(matriz, pos_submarino_hum, pos_submarino_com);
  
  // Enquanto o jogo estiver sendo rodado (Usuario nao optar por encerra-lo) o seguinte loop continuara a ser realizado
  while (continuar == 1){

    // Chama a funcao de jogada do humano e armazena o retorno do ataque na variavel 'atacado'
    atacado = jogada_humano(matriz, jogadas_hum, jogadas_com, pos_submarino_hum, pos_submarino_com, jogada, 0);

    // Enquanto o humano nao atacar agua, o seguinte loop continuara a ser realizado 
    while(atacado != 0 && hum_ganhou == 0 && com_ganhou == 0){
      i = 0;
      // Verifica se o ataque afundou uma embarcacao
      if (atacado != -1){
        // Armazena o retorno do ataque anterior na variavel 'emb_afundada_hum'
        emb_afundada_hum = atacado;
        // Busca no vetor de embarcacoes do computador a embarcacao afundada pelo humano 
        while (i<9 && emb_computador[i] != atacado){
          i+=1;
        }
        // Remove do vetor de embarcacoes do computador a embarcacao afundada pelo humano
        emb_computador[i] = 0;

        // Verifica no vetor de embarcacoes do computador se ha alguma embarcacao com excecao da jangada, se houver, significa que o humano ainda nao ganhou
        hum_ganhou = 1;
        for (int i=0; i<9; i++){
          if (emb_computador[i] != 0 && emb_computador[i] != 5){
            hum_ganhou = 0;
          }
        }
      }
      // Se o humano, no ataque anterior, nao acertou a jangada nem a agua e ainda nao ganhou o jogo, e solicitada uma nova jogada
      if (atacado != 5 && hum_ganhou == 0){
        atacado = jogada_humano(matriz, jogadas_hum, jogadas_com, pos_submarino_hum, pos_submarino_com, jogada, atacado);
      }
      // Se o humano acertou a jangada e ainda nao ganhou a partida, lhe e retirado um submarino, se ele ainda possuir, e printado na tela o feito e a variavel 'atacado' recebe zero, para que ele nao volte a jogar em seguida
      else if (hum_ganhou == 0){
        // Retirar um submarino do vetor de embarcações do Humano
        if (emb_humano[6] != 0){
          emb_humano[6] = 0;
        }
        // Retirar um submarino do vetor de embarcações do Humano
        else{
          emb_humano[7] = 0;
        }
        // Verifica no vetor de embarcações do humano se há alguma embarcação com exceção da jangada, se houver, significa que o computador ainda não ganhou
        com_ganhou = 1;
        for (int i=0; i<9; i++){
          if (emb_humano[i] != 0 && emb_humano[i] != 5){
            com_ganhou = 0;
          }
        }
        printf("O Humano acabou de afundar uma Jangada do Computador.\n");
        atacado = 0;
      }

    }
 
 

    if (hum_ganhou == 0 && com_ganhou == 0){
      // Chama a função de jogada do humano e armazena o retorno do ataque na variavel 'retorno_ataque_com' e na posição 4 do vetor de retornos de jogadas do computador
      retorno_ataque_com = retorno_jogada_com[4] = jogada_computador(matriz, jogadas_com, jogadas_hum, pos_submarino_hum, pos_submarino_com, retorno_jogada_com, jogada);
      //printf ("Retorno 1: %d\n", retorno_jogada_com[4]);

      // Enquanto o computador não atacar água, o seguinte loop continuará a ser realizado
      while(retorno_ataque_com != 0 && hum_ganhou == 0 && com_ganhou == 0){
        i = 0;
        // Armazena o retorno do ataque anterior na variável 'emb_afundada_com'
        emb_afundada_com = retorno_jogada_com[4];
        // Verifica se o ataque afundou uma embarcação
        if (retorno_jogada_com[4] != -1){
          // Busca no vetor de embarcações do humano a embarcação afundada pelo computador 
          while (i<9 && emb_humano[i] != retorno_jogada_com[4]){
            i+=1;
          }
          // Remove do vetor de embarcacoes do humano a embarcação afundada pelo computador
          emb_humano[i] = 0;

          // Verifica no vetor de embarcacoes do humano se há alguma embarcação com exceção da jangada, se houver, significa que o computador ainda não ganhou
          com_ganhou = 1;
          for (int i=0; i<9; i++){
            if (emb_humano[i] != 0 && emb_humano[i] != 5){
              com_ganhou = 0;
            }
          }
          
          // Zera o vetor de retornos de jogadas do computador
          for (int i=0; i<5; i++){
            retorno_jogada_com[i] = 0;
          }
        }

        // Se o computador, no ataque anterior, nao acertou a jangada nem a agua e ainda nao ganhou o jogo, e solicitada uma nova jogada
        if (emb_afundada_com != 5 && com_ganhou == 0){
          retorno_ataque_com = retorno_jogada_com[4] = jogada_computador(matriz, jogadas_com, jogadas_hum, pos_submarino_hum, pos_submarino_com, retorno_jogada_com, jogada);
          // Verifica se ele acertou alguma embarcacao na jogada anterior, se sim, ele imprime a mensagem referente à embarcacao afundada
          if (emb_afundada_com == porta_avioes){
            printf("O Computador acabou de afundar um Porta Aviões do Humano.\n");
          }
          else if (emb_afundada_com == destroyer){
            printf("O Computador acabou de afundar um Destroyer do Humano.\n");
          }
          else if (emb_afundada_com == fragata){
            printf("O Computador acabou de afundar uma Fragata do Humano.\n");
          }
          else if (emb_afundada_com == submarino){
            printf("O Computador acabou de afundar um Submarino do Humano.\n");
          }
        }
        // Se o humano acertou a jangada e ainda nao ganhou a partida, lhe é retirado um submarino, se ele ainda possuir, e printado na tela o feito e a variavel 'atacado' recebe zero, para que ele nao volte a jogar em seguida
        else if (com_ganhou == 0){
          // Retirar um submarino do vetor de embarcações do computador
          if (emb_computador[6] != 0){
            emb_computador[6] = 0;
          }
          else{
            emb_computador[7] = 0;
          }
          // Verifica no vetor de embarcacoes do computador se há alguma embarcação com exceção da jangada, se houver, significa que o humano ainda não ganhou
          hum_ganhou = 1;
          for (int i=0; i<9; i++){
            if (emb_computador[i] != 0 && emb_computador[i] != 5){
              hum_ganhou = 0;
            }
          }

          printf("O Computador acabou de afundar uma Jangada do Humano.\n");
          retorno_ataque_com = 0;
        }
      }
    }



    // Se o computador ganhar a partida, é printado na tela e colocado no placar
    if (com_ganhou == 1){
      printar_tabuleiro(matriz, jogadas_hum, jogadas_com, jogada);
      printf("\n\n------------------- O Computador Ganhou! --------------------\n");
      placar_com += 1;
    }
    // Se o humano ganhar a partida, é printado na tela e colocado no placar
    if (hum_ganhou == 1){
      printar_tabuleiro(matriz, jogadas_hum, jogadas_com, jogada);
      printf("\n\n----------------------- Você Ganhou! ------------------------\n");
      placar_hum += 1;
    }

    // Se algum jogador ganhou, é printado o placar e lhe é questionado se deseja reiniciar o jogo
    if (hum_ganhou || com_ganhou){
      printf("\n                    Humano       Computador                    \n");
      printf("\n                      %d              %d                        \n", placar_hum, placar_com);
      printf("\n-------------------------------------------------------------\n\n");

      printf("Deseja jogar novamente?\n (1) Sim\n (2) Nao\n");
      scanf("%d", &continuar);

      // Se ele desejar jogar novamente sera feito o seguinte
      if (continuar == 1){
        hum_ganhou = 0;
        com_ganhou = 0;
        
        // Desalocado matriz anterior
        free(matriz); 

        // Limpa o vetor de jogadas anteriores do humano e do computador
        for (int i=0; i<200; i++){
          jogadas_hum[i] = NULL;
          jogadas_com[i] = NULL;
        }

        // Limpa o vetor de retornos das jogadas do computador
        for (int i=0; i<5; i++){
          retorno_jogada_com[i] = 0;
        }

        // Cria novos tabuleiros
        matriz = criar_tabuleiro();

        // Poe NULL nos vetores de posicoes dos submarinos
        pos_submarino_com[0] = NULL;
        pos_submarino_com[1] = NULL;
        pos_submarino_hum[0] = NULL;
        pos_submarino_hum[1] = NULL;

        // Redistribui as embarcacoes
        distribuir_embarcacoes(matriz, pos_submarino_hum, pos_submarino_com);


        // Repoe o vetor de embarcacoes do humano e do computador
        for(int i=0; i<9;i++){
          if (i==0){
            emb_humano[0] = 1;
            emb_computador[0] = 1;
          }
          else if(i==1 || i==2){
            emb_humano[i] = 2;
            emb_computador[i] = 2;
          }
          else if(i>=3 && i<=5){
            emb_humano[i] = 3;
            emb_computador[i] = 3;
          }
          else if(i==6 || i==7){
            emb_humano[i] = 4;
            emb_computador[i] = 4;
          }
          else if(i==8){
            emb_humano[i] = 5;
            emb_computador[i] = 5;
          }
        }

      }
    
    }
 
  }
  printf("\nAté logo! :)\n");
  
  // Livra a posicao de memoria da matriz tabuleiro
  free(matriz);

  return EXIT_SUCCESS;
}

