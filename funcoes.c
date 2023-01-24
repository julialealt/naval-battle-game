#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "funcoes.h"



// função de criação do tabuleiro
casa **criar_tabuleiro(){
  int i=0, linha=0, coluna=0;
  casa **matriz;

  // aloca 12 espaços de memória com ponteiro para casa (referente às 12 linhas)
  matriz = (casa**) malloc(12*sizeof(casa*));
  //para cada um desses ponteiros, aloca mais 12 espaços de memória (referente às 12 colunas) 
  for (i=0; i<12; i++){
    matriz[i] = (casa*) malloc(12*sizeof(casa));
  }

  for (linha=0; linha<12; linha++){
    for (coluna=0; coluna<12; coluna++){
      // coloca todas as embarcações como NULL no tabuleiro (humano e computador)
      matriz[linha][coluna].emb_hum = matriz[linha][coluna].emb_com = NULL;
      // preenche todas as casas do tabuleiro visíveis para o usuário como '-'
      matriz[linha][coluna].info_hum = matriz[linha][coluna].info_com = '-';

      // Exceto para as linhas das extremidades; atribui aos ponteiros cima, baixo, esquerda e direita as casas vizinhas. As demais são nulas.

      if (linha>0){
        matriz[linha][coluna].cima = &matriz[linha-1][coluna];
      }
      else{
        matriz[linha][coluna].cima = NULL;
      }

      if (linha<11){
        matriz[linha][coluna].baixo = &matriz[linha+1][coluna];
      }
      else{
        matriz[linha][coluna].baixo = NULL;
      }

      if (coluna>0){
        matriz[linha][coluna].esq = &matriz[linha][coluna-1];
      }
      else{
        matriz[linha][coluna].esq = NULL;
      }

      if (coluna<11){
        matriz[linha][coluna].dir = &matriz[linha][coluna+1];
      }
      else{
        matriz[linha][coluna].dir = NULL;
      }

    }
  }

  // retorna o ponteiro da primeira casa da matriz criada
  return matriz;
}

// função de impressão do tabuleiro
void printar_tabuleiro(casa **matriz, casa **jogadas_hum, casa **jogadas_com, char jogada[4]){
  int linha=0, coluna=0;

  printf("            Humano                         Computador\n");
	printf("    A B C D E F G H I J K L         A B C D E F G H I J K L\n");
	printf("  +-------------------------+     +-------------------------+\n");

  // imprime a matriz que contém os tabuleiros do humano e do computador, de linha por linha
  while(linha<12){
    printf("%.2d|", linha+1); //print do valor da linha
    for(coluna=0; coluna<12; coluna++){
      printf(" %c", matriz[linha][coluna].info_hum); //impressão da informação disponível para visualização da matriz 
    }
    // repete o processo para formar o tabuleiro do computador
    printf(" |   %.2d|", linha+1);
    for(coluna=0; coluna<12; coluna++){
      printf(" %c", matriz[linha][coluna].info_com);
    }
    printf(" |\n");
    linha +=1;
  }

  printf("  +-------------------------+     +-------------------------+\n");

  // print da última jogada realizada por ambos os jogadores (valores são recebidos como requisitos nessa função)
  ultima_jogada(matriz, jogadas_com, jogada);
  printf("Ultima jogada do Computador: %s\n", jogada);
  ultima_jogada(matriz, jogadas_hum, jogada);
  printf("Ultima jogada do Humano: %s\n", jogada);

}

// função de criação das embarcações
int criar_embarcacao(int tipo, int jogador, casa *no, int   orientacao){
  int nao_alocado = 1;
  // cria um ponteiro que aponta para uma estrutura do tipo embarcação 
  embarcacao *nova_embarcacao;

  // nova_embarcacao recebe o ponteiro da estrutura embarcação alocada pelo malloc
  nova_embarcacao = (embarcacao*) malloc(sizeof(embarcacao));

  // recebe e armazena o tipo de embarcação o de jogador dentro da struct
  nova_embarcacao->tipo = tipo;
  nova_embarcacao->jogador = jogador;

  // Checamos, na condicional, se as obrigatoriedades referentes a cada tipo de embarcação e seu direcionamento são satisfeitas antes que elas sejam posicionadas. Por exemplo, para o porta aviões na horizontal é necessário checar se as cinco casas a serem preenchidas estão vazias (a do meio, as duas a sua direita e as duas a sua esquerda) e também as casas a sua volta 

  //    N N N N N
  //  N * * * * * N
  //    N N N N N 

  if (jogador == humano){
    if (tipo == porta_avioes){
      if (orientacao == horizontal && no->emb_hum == NULL && no->esq->esq->emb_hum == NULL && no->dir->dir->emb_hum == NULL && no->esq->emb_hum == NULL && no->dir->emb_hum == NULL && no->esq->esq->esq->emb_hum == NULL && no->dir->dir->dir->emb_hum == NULL  && no->cima->emb_hum == NULL && no->esq->esq->cima->emb_hum == NULL && no->dir->dir->cima->emb_hum == NULL && no->esq->cima->emb_hum == NULL && no->dir->cima->emb_hum == NULL && no->baixo->emb_hum == NULL && no->esq->esq->baixo->emb_hum == NULL && no->dir->dir->baixo->emb_hum == NULL && no->esq->baixo->emb_hum == NULL && no->dir->baixo->emb_hum == NULL){
        // caso tais condições sejam satisfeitas, atualizamos a variável nao_alocado para 0 e criamos a nova embarcação com as características desejadas, ocupando cinco casas.
        nao_alocado = 0;
        no->esq->esq->emb_hum = nova_embarcacao;
        no->dir->dir->emb_hum = nova_embarcacao;
        no->esq->emb_hum = nova_embarcacao;
        no->dir->emb_hum = nova_embarcacao;
        
        //indica o que deve ser mostrado na tela do usuário para representar as casas ocupadas pelo porta-avião (#)
        no->info_hum = (no->esq)->info_hum = (no->dir)->info_hum = ((no->esq)->esq)->info_hum = ((no->dir)->dir)->info_hum = '#';
        
      }
      // repete o processo para porta-aviões verticais
      if (orientacao == vertical && no->emb_hum == NULL && no->cima->cima->emb_hum == NULL && no->baixo->baixo->emb_hum == NULL && no->cima->emb_hum == NULL && no->baixo->emb_hum == NULL && no->cima->cima->cima->emb_hum == NULL && no->baixo->baixo->baixo->emb_hum == NULL && no->esq->emb_hum == NULL && no->cima->cima->esq->emb_hum == NULL && no->baixo->baixo->esq->emb_hum == NULL && no->cima->esq->emb_hum == NULL && no->baixo->esq->emb_hum == NULL && no->dir->emb_hum == NULL && no->cima->cima->dir->emb_hum == NULL && no->baixo->baixo->dir->emb_hum == NULL && no->cima->dir->emb_hum == NULL && no->baixo->dir->emb_hum == NULL){
        nao_alocado = 0;
        no->cima->cima->emb_hum = nova_embarcacao;
        no->baixo->baixo->emb_hum = nova_embarcacao;
        no->cima->emb_hum = nova_embarcacao;
        no->baixo->emb_hum = nova_embarcacao;
        
        no->info_hum = (no->cima)->info_hum = (no->baixo)->info_hum = ((no->cima)->cima)->info_hum = ((no->baixo)->baixo)->info_hum = '#';
    
      }
      // armazena, na struct apontada por nova_embarcação, a quantidade de vidas do porta-avião (que deve ser atacado 5 vezes, em casas diferentes, para ser destruído)
      nova_embarcacao->vida = 5;
    }
    
    // repete o processo para o tipo destroyer, na vertical ou na horizontal

     //    N N N 
     //  N * * * N   (ex. horizontal)
     //    N N N 

    else if (tipo == destroyer){
      if (orientacao == horizontal && no->emb_hum == NULL && no->esq->emb_hum == NULL && no->dir->emb_hum == NULL && no->esq->esq->emb_hum == NULL && no->dir->dir->emb_hum == NULL && no->cima->emb_hum == NULL && no->baixo->emb_hum == NULL && no->esq->cima->emb_hum == NULL && no->dir->cima->emb_hum == NULL && no->esq->baixo->emb_hum == NULL && no->dir->baixo->emb_hum == NULL){
        nao_alocado = 0;
        no->esq->emb_hum = nova_embarcacao;
        no->dir->emb_hum = nova_embarcacao;
        
        no->info_hum = (no->esq)->info_hum = (no->dir)->info_hum = '#';
        
      }
      if (orientacao == vertical && no->emb_hum == NULL && no->cima->emb_hum == NULL && no->baixo->emb_hum == NULL && no->cima->cima->emb_hum == NULL && no->baixo->baixo->emb_hum == NULL && no->esq->emb_hum == NULL && no->dir->emb_hum == NULL && no->cima->esq->emb_hum == NULL && no->baixo->esq->emb_hum == NULL &&no->cima->dir->emb_hum == NULL && no->baixo->dir->emb_hum == NULL){
        nao_alocado = 0;
        no->cima->emb_hum = nova_embarcacao;
        no->baixo->emb_hum = nova_embarcacao;
        
        no->info_hum = (no->cima)->info_hum = (no->baixo)->info_hum = '#';
    
      } // armazena o número de vidas do destroyer (3)
      nova_embarcacao->vida = 3;
    }

     // repete o processo para o tipo fragata, na vertical ou na horizontal

     //    N N  
     //  N * * N   (ex. horizontal)
     //    N N

    else if (tipo == fragata){
      if (orientacao == horizontal && no->emb_hum == NULL && no->esq->emb_hum == NULL && no->dir->emb_hum == NULL && no->esq->esq->emb_hum == NULL && no->dir->dir->emb_hum == NULL && no->cima->emb_hum == NULL && no->baixo->emb_hum == NULL && no->esq->cima->emb_hum == NULL && no->esq->baixo->emb_hum == NULL && no->dir->cima->emb_hum == NULL && no->dir->baixo->emb_hum == NULL){
        srand(time(NULL));
        // escolhe aleatoriamente a direção da segunda casa da fragata
        if (rand() % 2 == 0){
          nao_alocado = 0;
          no->esq->emb_hum = nova_embarcacao;
          
          no->info_hum = (no->esq)->info_hum = '#';
        }
        else{ 
          nao_alocado = 0;
          no->dir->emb_hum = nova_embarcacao;
          
          no->info_hum = (no->dir)->info_hum = '#';
        }

      }
      
      //repetimos o processo para a vertical
      if (orientacao == vertical && no->emb_hum == NULL && no->cima->emb_hum == NULL && no->baixo->emb_hum == NULL && no->cima->cima->emb_hum == NULL && no->baixo->baixo->emb_hum == NULL && no->esq->emb_hum == NULL && no->dir->emb_hum == NULL && no->cima->esq->emb_hum == NULL && no->cima->dir->emb_hum == NULL && no->baixo->esq->emb_hum == NULL && no->baixo->dir->emb_hum == NULL){
        srand(time(NULL));
        if (rand() % 2 == 0){
          nao_alocado = 0;
          no->cima->emb_hum = nova_embarcacao;
          
          no->info_hum = (no->cima)->info_hum = '#';
        } 
        else{
          nao_alocado = 0;
          no->baixo->emb_hum = nova_embarcacao;
          
          no->info_hum = (no->baixo)->info_hum = '#';
        } 
    
      } // armazena o número de vidas da fragata (2)
      nova_embarcacao->vida = 2;
    }

    // caso dividido para os tipos de embarcação jangada e submarino, já que ambos ocupam apenas uma casa do tabuleiro

    //   N
    // N x N 
    //   N

    else if(no->emb_hum == NULL && no->esq->emb_hum == NULL && no->dir->emb_hum == NULL && no->cima->emb_hum == NULL && no->baixo->emb_hum == NULL && (tipo == submarino || tipo == jangada)){
      nao_alocado = 0;
      // armazena o número de vidas do submarino ou da jangada (apenas 1) na struct apontada pela nova embarcação
      nova_embarcacao->vida = 1;
      if (jogador == humano){
         // caso o tipo da embarcação seja submarino, deve mostrar no tabuleiro '@' ao usuário
        if (tipo == submarino){
          no->info_hum = '@';
        }
        else{
         //já se for jangada, deve mostrar o '&'
          no->info_hum = '&';
        }
      }
    }

    // caso a embarcação não tenha sido alocada com sucesso, devemos liberar o espaço de memoria de nova_embarcação e retornar 0
    if(nao_alocado){
      free(nova_embarcacao);
      return 0;
    }

    // o espaço que aponta para a embarcação do humano presente na casa vai passar a apontar para a struct apontada por nova_embarcação
    no->emb_hum = nova_embarcacao;

  }
  
  // Para criar as embarcações do tabuleiro do computador,não é necessário impedir o povoamento das extremidades das embarcações já existentes. Isso ocorre já que o usuário digita, por vontade própria, a casa que quer atacar após um acerto; diferentemente do computador, que segue uma ordem repetitiva de busca pelas extremidades da casa atacadada que pode ser atrapalhada com a proximidade entre duas ou mais embarcações)

  else if (jogador == computador){
    if (tipo == porta_avioes){
      // portanto, para o porta-avião apenas temos que checar se as cinco casas nas quais a embarcação será inserida estão vazias
      if (orientacao == horizontal && no->emb_com == NULL && no->esq->esq->emb_com == NULL && no->dir->dir->emb_com == NULL && no->esq->emb_com == NULL && no->dir->emb_com == NULL){
        nao_alocado = 0;
        // em caso positivo, as posições designadas vão receber a struct apontada por nova_embarcacao
        no->esq->esq->emb_com = nova_embarcacao;
        no->dir->dir->emb_com = nova_embarcacao;
        no->esq->emb_com = nova_embarcacao;
        no->dir->emb_com = nova_embarcacao;
      }
      if (orientacao == vertical && no->emb_com == NULL && no->cima->cima->emb_com == NULL && no->baixo->baixo->emb_com == NULL && no->cima->emb_com == NULL && no->baixo->emb_com == NULL){
        nao_alocado = 0;
        no->cima->cima->emb_com = nova_embarcacao;
        no->baixo->baixo->emb_com = nova_embarcacao;
        no->cima->emb_com = nova_embarcacao;
        no->baixo->emb_com = nova_embarcacao;
      }
      // armazena, na struct nova_embarcação, a quantidade de vidas do porta-avião (5)
      nova_embarcacao->vida = 5;
    }
    
    // repetimos o processo para as tres casas ocupadas pelo tipo de embarcação destroyer
    else if (tipo == destroyer){
      if (orientacao == horizontal && no->emb_com == NULL && no->esq->emb_com == NULL && no->dir->emb_com == NULL){
        nao_alocado = 0;
        no->esq->emb_com = nova_embarcacao;
        no->dir->emb_com = nova_embarcacao;
      }
      if (orientacao == vertical && no->emb_com == NULL && no->cima->emb_com == NULL && no->baixo->emb_com == NULL){
        nao_alocado = 0;
        no->cima->emb_com = nova_embarcacao;
        no->baixo->emb_com = nova_embarcacao;
      }
      nova_embarcacao->vida = 3;
    }

    // e novamente para o tipo de embarcação fragata, que ocupa duas casas
    else if (tipo == fragata){
      // verifica se a a casa da esquerda ou a da direita estão vazias, para dar sequencia ao processo
      if (orientacao == horizontal && no->emb_com == NULL && (no->esq->emb_com == NULL || no->dir->emb_com == NULL)){
        nao_alocado = 0;
        
        // cria a embarcação de acordo com a disponibilidade dos espaços ao redor da casa de referencia
        if (no->esq->emb_com == NULL){
          no->esq->emb_com = nova_embarcacao;
        } 
        else{
          no->dir->emb_com = nova_embarcacao;
        } 

        
      }
      if (orientacao == vertical && no->emb_com == NULL && (no->cima->emb_com == NULL || no->baixo->emb_com == NULL)){
        nao_alocado = 0;

        if (no->cima->emb_com == NULL){
          no->cima->emb_com = nova_embarcacao;
        } 
        else{
          no->baixo->emb_com = nova_embarcacao;
        } 
    
      }
      nova_embarcacao->vida = 2;
    }

    // por último; caso a embarcação ocupe apenas uma casa (sendo jangada ou submarino), também checamos se a sorteada estava vazia anteriormente e - em caso positivo - a criamos com 1 vida  
    else if(no->emb_com == NULL && (tipo == submarino || tipo == jangada)){
      nao_alocado = 0;
      nova_embarcacao->vida = 1;
    }

    // caso a casa não possa ser alocada, liberamos o espaço de memoria da nova embarcação e retornamos 0
    if(nao_alocado){
      free(nova_embarcacao);
      return 0;
    }

    // o atributo emb_com vai apontar para o que nova_embarcacao esta apontando
    no->emb_com = nova_embarcacao;

  }

 // retorna 1, indicando o sucesso na alocação
  return 1;
}

// função de distribuição aleatória de embarcações nos tabuleiros
void distribuir_embarcacoes(casa **matriz, casa **pos_submarino_hum, casa **pos_submarino_com){
  int jogador, alocado_porta_avioes = 0, alocado_destroyer = 0, alocado_fragata=0, alocado_submarino=0, alocado_jangada=0, a, num, aleatorio[7], orientacao, linha, coluna;

  // repetir o processo duas vezes, para os tabuleiros de cada jogador
  for(jogador = 1; jogador<=2; jogador ++){
    // inicializa o gerador de números aleatorios
    srand(time(NULL));

    // até que o contador de porta aviões alocados seja maior que zero (ou seja, quando o primeiro for distribuído), repetiremos
    while (alocado_porta_avioes == 0){
      for(a=0; a<=6;a++){
        num = rand();
        aleatorio[a] = num;
      }
      
      // escolhe aleatoriamente valores e, após o cálculo de seus módulos de 2 ou de 12 (variando conforme a quantidade de possibilidades de orientações, linhas e colunas), armazena-os de acordo com sua categoria

      // jogadores pegam numeros aleatorios diferentes 
      orientacao = aleatorio[1*jogador] % 2;
      linha = aleatorio[2*jogador] % 12;
      coluna = aleatorio[3*jogador] % 12;
      
      if (linha>0 && linha<11 && coluna>0 && coluna<11){
        // confere se o meio da embarcação foi colocado em uma posição na qual suas casas da direita e da esquerda (no tipo de orientação horizontal, por exemplo) estejam obrigatoriamente dentro do tabuleiro (entre 3 e 8, nesse caso; já que o porta-avião tem duas casas para a direita e duas para a esquerda)
        if (orientacao == horizontal && coluna >=3  && coluna<=8){
          // a função criar_embarcação retorna 1 se ela for devidamente criada e 0 se não, portanto armazenamos esses valores no contador de porta-aviões alocados após a tentativa de criação da embarcação
          alocado_porta_avioes = criar_embarcacao(porta_avioes, jogador, &matriz[linha][coluna], orientacao);
        } // repete o processo para embarcações na vertical
        else if (orientacao == vertical && linha>=3  && linha<=8){
          alocado_porta_avioes = criar_embarcacao(porta_avioes, jogador, &matriz[linha][coluna], orientacao);
        }
      }
      
    }

    // o processo é similar para os demais tipos de embarcação, variando apenas nas suas especificidades: para o destroyer, por exemplo, temos que repetir a distribuição até que dois deles sejam criados
    while (alocado_destroyer < 2){
      for(a=0; a<=6;a++){
        num = rand();
        aleatorio[a] = num;
      }

      orientacao = aleatorio[1*jogador] % 2;
      linha = aleatorio[2*jogador] % 12;
      coluna = aleatorio[3*jogador] % 12;

      if (linha>0 && linha<11 && coluna>0 && coluna<11){
        // verifica se o centro está entre 2 e 9, para que a embarcação não passe do tabuleiro
        if (orientacao == horizontal && coluna >=2  && coluna<=9){
          // cria o destoyer e já incrementa o seu contador
          alocado_destroyer += criar_embarcacao(destroyer, jogador, &matriz[linha][coluna], orientacao);
        }
        else if (orientacao == vertical && linha>=2  && linha<=9){
          alocado_destroyer += criar_embarcacao(destroyer, jogador, &matriz[linha][coluna], orientacao);
        }
      }

    }

    // repete até que sejam distribuídas 3 fragatas
    while (alocado_fragata < 3){
      for(a=0; a<=6;a++){
        num = rand();
        aleatorio[a] = num;
      }

      orientacao = aleatorio[1*jogador] % 2;
      linha = aleatorio[2*jogador] % 12;
      coluna = aleatorio[3*jogador] % 12;


      if (linha>0 && linha<11 && coluna>0 && coluna<11){
        // verifica se o centro está entre 2 e 9, para que a embarcação não passe do tabuleiro
        if (orientacao == horizontal && coluna >=2  && coluna<=9){
          // cria a fragata e já incrementa seu contador 
          alocado_fragata += criar_embarcacao(fragata, jogador, &matriz[linha][coluna], orientacao);
        }
        else if (orientacao == vertical && linha>=2  && linha<=9){
          alocado_fragata += criar_embarcacao(fragata, jogador, &matriz[linha][coluna], orientacao);
        }
      }

    }

    // repete até que sejam distribuídos 2 submarinos
    while (alocado_submarino < 2){
      for(a=0; a<=6;a++){
        num = rand();
        aleatorio[a] = num;
      }

      orientacao = aleatorio[1*jogador] % 2;
      linha = aleatorio[2*jogador] % 12;
      coluna = aleatorio[3*jogador] % 12;

      // cria independentemente da posição, uma vez que ocupa apenas uma casa
      if (linha>0 && linha<11 && coluna>0 && coluna<11){
        alocado_submarino += criar_embarcacao(submarino, jogador, &matriz[linha][coluna], orientacao);
      }

      // precisaremos da posição do submarino para que ele seja perdido caso a jangada do oponente seja acertada, então o armazenaremos no vetor pos_submarino_hum ou pos_submarino_com
      if (jogador == humano){
        // caso a primeira posição do vetor esteja vazia, colocaremos a posição do submarino alocado lá. senão, tentaremos colocar na segunda (repetimos o processo para ambos os jogadores)
        if (alocado_submarino == 1 && pos_submarino_hum[0] == NULL){
          pos_submarino_hum[0] = &matriz[linha][coluna];
        }
        else if (alocado_submarino == 2 && pos_submarino_hum[1] == NULL){
          pos_submarino_hum[1] = &matriz[linha][coluna];
        }
      }
      else{
        if (alocado_submarino == 1 && pos_submarino_com[0] == NULL){
          pos_submarino_com[0] = &matriz[linha][coluna];
        }
        else if (alocado_submarino == 2 && pos_submarino_com[1] == NULL){
          pos_submarino_com[1] = &matriz[linha][coluna];
        }
      }
    }

    // para a jangada, repetimos o processo uma vez
    while (alocado_jangada == 0){
      for(a=0; a<=6;a++){
        num = rand();
        aleatorio[a] = num;
      }

      orientacao = aleatorio[1*jogador] % 2;
      linha = aleatorio[2*jogador] % 12;
      coluna = aleatorio[3*jogador] % 12;

      // alocamos a embarcação na casa sorteada
      if (linha>0 && linha<11 && coluna>0 && coluna<11){
        alocado_jangada = criar_embarcacao(jangada, jogador, &matriz[linha][coluna], orientacao);
      }
    }

    // zeramos as variaveis para que o processo possa ser repetido para o outro jogador 
    alocado_porta_avioes = 0; 
    alocado_destroyer = 0;
    alocado_fragata = 0;
    alocado_submarino = 0; 
    alocado_jangada = 0;
  }
}


// função de tratamento das entradas 
int tratar_entrada (char entrada_s[7], int entrada_i[]) {
  int linha, coluna, valido_coluna=0, valido_linha=0, numero, prim_numero, seg_numero, i, quant_numeros=0;

  // a variável que recebemos como entrada pode ter até sete caracteres (limite arbitrário)
  // usamos a tabela ascii para transformar os caracteres inseridos na entrada para valores inteiros
  for (i=0; i<7; i++){
    // se a entrada estiver entre os valores ascii referentes às letras minusculas de A até L
    if (entrada_s[i] >= 'a' && entrada_s[i] <= 'l'){
      // se valido_coluna, inicializada como 0, continuar assim (ou seja, sem que nenhuma coluna válida já tenha sido introduzida)
      if (valido_coluna) return 0;
      // o valor a ser atribuído até coluna é o valor da entrada - 97 (considerando que o A minúsculo é referente ao 97 na tabela; equivalento assim á coluna A a 0 e assim em diante)
      coluna = ((int) entrada_s[i])-97;
      // validamos a coluna inserida
      valido_coluna = 1;
    }

    //repetimos o processo para as letras maiusculas, considerando que o A maiusculo, na tabela, é codificado como o int 65
    else if (entrada_s[i] >= 'A' && entrada_s[i] <= 'L'){
      if (valido_coluna) return 0;
      coluna = ((int) entrada_s[i])-65;
      valido_coluna = 1;
    }

    // armazenamos o número referente à linha na variável, já transformando da tabela asc novamente para inteiro (0 = 48 na tabela)
    numero = (int) entrada_s[i] - 48;

    // pega todos os numeros inseridos pelo usuario e armazena até os dois primeiros, contabilizando simultaneamente a quantidade de algarismos que foi inserida (se for maior que dois, já sabe que a entrada foi inválida)
    if (numero<=9 && numero>=0) { 
      quant_numeros += 1;
      if (quant_numeros == 1){
        prim_numero = numero;
      }
      else if (quant_numeros == 2){
        seg_numero = numero;
      }
    }
  }

  if (quant_numeros>0 && quant_numeros<3){
    if (quant_numeros == 1){
      // se apenas um numero for inserido, a linha vai ser equivalente ao seu valor - 1
      if (prim_numero>0){
        linha = prim_numero - 1;
        valido_linha = 1;
      }
      else{
        return 0;
      }
    }
    else{
      // se dois algarismos tiverem sido inseridos e atenderem as condições, calcula-se o valor da linha
      if (prim_numero == 1 && seg_numero<3){
        linha = prim_numero*10 + seg_numero - 1;
        valido_linha = 1;
      }
      else{
        return 0;
      }
    }
  }

  // se linha e coluna forem válidas, as armazenamos no vetor e retornamos 1
  if (valido_linha && valido_coluna){
    entrada_i[0] = linha;
    entrada_i[1] = coluna;
    return 1;
  } //caso contrário, retornamos 0
  return 0;
}




// função de ataque 
int ataque(casa *matriz, int jogador, casa **pos_submarino_hum, casa **pos_submarino_com, casa **jogadas_hum, casa **jogadas_com){
  embarcacao *emb;
  int tipo_emb, i=0;

  if (jogador == humano){
    // se o jogador for humano, a embarcação a ser atacada, apontada por emb_com, será agora apontada por emb
    emb = matriz->emb_com;

    //se a embarcação atacada for nula, o retorno visual deve ser '0' (acertou água)
    if (emb == NULL){
      matriz->info_com = 'O';
      return 0;
    }
    // se a quantidade de vidas da embarcação atacada for 1, temos a possibilidade de que seja submarino ou jangada ou uma parcela dos demais tipo de embarcações (quando ja foram atacados anteriormente)
    else if(emb->vida == 1){ 
      // se for submarino, deve ser exibido no tabuleiro o @
      if (emb->tipo == submarino){
        matriz->info_com = '@';

        // após acertar o submarino, temos que apagá-lo do vetor que armazena suas posições; então comparamos a matriz com os elementos de pos_submarino_com e, quando o identificamos, esvaziamos o espaço 

        if (pos_submarino_com[0] == matriz){
          pos_submarino_com[0] = NULL;
        }
        else{
          pos_submarino_com[1] = NULL;
        }
        free(matriz->emb_com);
        matriz->emb_com = NULL;

        return 4; // retorna o def referente ao submarino, para mostrar qual tipo de embarcação foi atacada
      } 
      // se o tipo for jangada, info_com deve armazenar '&' para mostrar no tabuleiro
      else if (emb->tipo == jangada){
        matriz->info_com = '&';

        // se a jangada do oponente for atacada, o jogador deve perder um de seus submarinos. portanto, se ele ainda tiver submarinos armazenados, o tiramos do vetor de embarcações e mostramos * no tabuleiro 
        i = 0;
        while (jogadas_com[i] != NULL){
          i+=1;
        }

        if (pos_submarino_hum[0] != NULL){
          jogadas_com[i] = pos_submarino_hum[0];
          pos_submarino_hum[0]->info_hum = '*';
          free(pos_submarino_hum[0]->emb_hum);
        }
        else if (pos_submarino_hum[1] != NULL){
          jogadas_com[i] = pos_submarino_hum[1];
          pos_submarino_hum[1]->info_hum = '*';
          free(pos_submarino_hum[1]->emb_hum);
        }
        free(matriz->emb_com);
        matriz->emb_com = NULL;

        return 5; // retornamos o valor correspondente à jangada
      }
      // se não for nenhum dos casos, sabemos que é uma das casas de uma embarcação recém afundada.
      // procuramos, então, as demais casas ocupadas pela mesma embarcação para que toda ela receba o mesmo símbolo (#, no caso) 
      // buscamos as casas não nulas e iguais a emb até com 4 casas de distancia para cada lado, configurando o pior caso (onde atacou a extremidade de um porta-avião) e preenchemos seu info_com com #
      else{
        // casos para a esquerda
        if (matriz->esq != NULL && matriz->esq->emb_com == emb){
          matriz->esq->info_com = '#';

          if (matriz->esq->esq != NULL && matriz->esq->esq->emb_com == emb){
            matriz->esq->esq->info_com = '#';

            if (matriz->esq->esq->esq != NULL && matriz->esq->esq->esq->emb_com == emb){
              matriz->esq->esq->esq->info_com = '#';

              if (matriz->esq->esq->esq->esq != NULL && matriz->esq->esq->esq->esq->emb_com == emb){
                matriz->esq->esq->esq->esq->info_com = '#';
              }
            }
          }
        } 
        // casos para a direita 
        if (matriz->dir != NULL && matriz->dir->emb_com == emb){
          matriz->dir->info_com = '#';

          if (matriz->dir->dir != NULL && matriz->dir->dir->emb_com == emb){
            matriz->dir->dir->info_com = '#';

            if (matriz->dir->dir->dir != NULL && matriz->dir->dir->dir->emb_com == emb){
              matriz->dir->dir->dir->info_com = '#';

              if (matriz->dir->dir->dir->dir != NULL && matriz->dir->dir->dir->dir->emb_com == emb){
                matriz->dir->dir->dir->dir->info_com = '#';
              }
            }
          }
        }
        // casos para cima
        if (matriz->cima != NULL && matriz->cima->emb_com == emb){
          matriz->cima->info_com = '#';

          if (matriz->cima->cima != NULL && matriz->cima->cima->emb_com == emb){
            matriz->cima->cima->info_com = '#';

            if (matriz->cima->cima->cima != NULL && matriz->cima->cima->cima->emb_com == emb){
              matriz->cima->cima->cima->info_com = '#';

              if (matriz->cima->cima->cima->cima != NULL && matriz->cima->cima->cima->cima->emb_com == emb){
                matriz->cima->cima->cima->cima->info_com = '#';
              }
            }
          }
        }
        // casos para baixo
        if (matriz->baixo != NULL && matriz->baixo->emb_com == emb){
          matriz->baixo->info_com = '#';

          if (matriz->baixo->baixo != NULL && matriz->baixo->baixo->emb_com == emb){
            matriz->baixo->baixo->info_com = '#';

            if (matriz->baixo->baixo->baixo != NULL && matriz->baixo->baixo->baixo->emb_com == emb){
              matriz->baixo->baixo->baixo->info_com = '#';

              if (matriz->baixo->baixo->baixo->baixo != NULL && matriz->baixo->baixo->baixo->baixo->emb_com == emb){
                matriz->baixo->baixo->baixo->baixo->info_com = '#';
              }
            }
          }
        }
        matriz->info_com = '#'; //referente à casa atacada
      } 

      //armazena o tipo da embarcação atacada e o retorna, além de liberar o espaço de memória
      tipo_emb = emb->tipo;
      free(emb);
      matriz->emb_com = NULL;
      return tipo_emb;
    }

    // se tiver mais de 1 vida
    else{
      // agora que temos o tipo da embarcação atacada, diminuiremos uma das suas vidas e encontraremos suas extremidades para que possamos mostrar, como informação do tabuleiro, as setas que as representam
      if (emb->tipo == fragata){
        emb->vida = emb->vida-1;
        // checa se há uma embarcação na casa ao lado (repete o processo para esquerda, direita, cima e baixo)
        if (matriz->esq != NULL){
          // compara embarcação atacada com a que está na esquerda, para ter certeza de que é a mesma
          // no caso da fragata, a outra casa da embarcação precisa estar imediatamente em um dos seus lados
          //dependendo da posição encontrada, atualizamos info_com para indicar, ao usuario, a direção na qual esta o resto da embarcação 
          if (matriz->esq->emb_com == emb){
            matriz->info_com = '>';      
          }
        }
        if (matriz->dir != NULL){
          if (matriz->dir->emb_com == emb){
            matriz->info_com = '<';
          }
        }
        if (matriz->cima != NULL){
          if (matriz->cima->emb_com == emb){
            matriz->info_com = 'v';
          }
        }
        if (matriz->baixo != NULL){
          if (matriz->baixo->emb_com == emb){
            matriz->info_com = '^';
          }
        }
      }

      //processo repetido para o destroyer, considerando que ocupa 3 casas 
      else if(emb->tipo == destroyer){
        emb->vida = emb->vida-1;
        // checa se a casa atingida é a que está no meio do destroyer, e, em caso positivo, colocamos * no info_com
        if ((matriz->esq != NULL && matriz->esq->emb_com == emb && matriz->dir != NULL && matriz->dir->emb_com == emb) || (matriz->cima != NULL && matriz->cima->emb_com == emb && matriz->baixo != NULL && matriz->baixo->emb_com == emb)) {
          matriz->info_com = '*';
        }

        else{
          // repete o processo de descobrir para qual direção está o restante da embarcação, armazenando no info_com a seta que representa sua extremidade
          if (matriz->esq != NULL){
            if (matriz->esq->emb_com == emb){
              matriz->info_com = '>';
            }
          }
          if (matriz->dir != NULL){
            if (matriz->dir->emb_com == emb){
              matriz->info_com = '<';
            }
          }
          if (matriz->cima != NULL){
            if (matriz->cima->emb_com == emb){
              matriz->info_com = 'v';
            }
          }
          if (matriz->baixo != NULL){
            if (matriz->baixo->emb_com == emb){
              matriz->info_com = '^';
            }
          }
        }

        
      }
      //processo repetido para o porta avioes, considerando que ocupa 5 casas
      else if(emb->tipo == porta_avioes){
        emb->vida = emb->vida-1;
        // casas adjacentes à do meio
        if ((matriz->esq != NULL && matriz->esq->emb_com == emb && matriz->dir != NULL && matriz->dir->emb_com == emb) || (matriz->cima != NULL && matriz->cima->emb_com == emb && matriz->baixo != NULL && matriz->baixo->emb_com == emb)) {
          matriz->info_com = '*';
        }
        // casa do meio
        else if ((matriz->esq != NULL && matriz->esq->esq != NULL && matriz->esq->esq->emb_com == emb && matriz->dir != NULL &&matriz->dir->dir != NULL && matriz->dir->dir->emb_com == emb) || (matriz->cima != NULL && matriz->cima->cima != NULL && matriz->cima->cima->emb_com == emb && matriz->baixo != NULL && matriz->baixo->baixo != NULL && matriz->baixo->baixo->emb_com == emb)) {
          matriz->info_com = '*';
        }
        // extremidades
        else{
          if (matriz->esq != NULL){
            if (matriz->esq->emb_com == emb){
              matriz->info_com = '>';
            }
          }
          if (matriz->dir != NULL){
            if (matriz->dir->emb_com == emb){
              matriz->info_com = '<';
            }
          }
          if (matriz->cima != NULL){
            if (matriz->cima->emb_com == emb){
              matriz->info_com = 'v';
            }
          }
          if (matriz->baixo != NULL){
            if (matriz->baixo->emb_com == emb){
              matriz->info_com = '^';
            }
          }
        }

      }

      return -1; //atacou apenas uma parte da embarcação, não a afundou
    }

  }
  
  // jogador sendo computador - semelhante ao processo para o jogador humano mudando, somente, os simbolos
  else{
    emb = matriz->emb_hum;

    if (emb == NULL){
      matriz->info_hum = 'O';
      return 0;
    }
    else if(emb->vida == 1){
      if (emb->tipo == submarino){
        matriz->info_hum = '*';
        // procura o submarino atacado no pos_submarino_hum para apagá-lo
        if (pos_submarino_hum[0] == matriz){
          pos_submarino_hum[0] = NULL;
        }
        else{ 
          pos_submarino_hum[1] = NULL;
        }

        free(matriz->emb_hum);
        matriz->emb_hum = NULL;
        return 4;
      } 
      else if (emb->tipo == jangada){
        matriz->info_hum = '*';
        
        i = 0;
        while (jogadas_hum[i] != NULL){
          i+=1;
        }

        //se ainda tiver submarinos, vai perder um deles apos atacar a jangada
        if (pos_submarino_com[0] != NULL){
          jogadas_hum[i] = pos_submarino_com[0];
          pos_submarino_com[0]->info_com = '@';
          free(pos_submarino_com[0]->emb_com);
        }
        else if (pos_submarino_com[1] != NULL){
          jogadas_hum[i] = pos_submarino_com[1];
          pos_submarino_com[1]->info_com = '@';
          free(pos_submarino_com[1]->emb_com);
        }

        free(matriz->emb_hum);
        matriz->emb_hum = NULL;
        return 5;
      }
      else{

        if (matriz->esq != NULL && matriz->esq->emb_hum == emb){
          matriz->esq->info_hum = '*';

          if (matriz->esq->esq != NULL && matriz->esq->esq->emb_hum == emb){
            matriz->esq->esq->info_hum = '*';

            if (matriz->esq->esq->esq != NULL && matriz->esq->esq->esq->emb_hum == emb){
              matriz->esq->esq->esq->info_hum = '*';

              if (matriz->esq->esq->esq->esq != NULL && matriz->esq->esq->esq->esq->emb_hum == emb){
                matriz->esq->esq->esq->esq->info_hum = '*';
              }
            }
          }
        }
        if (matriz->dir != NULL && matriz->dir->emb_hum == emb){
          matriz->dir->info_hum = '*';

          if (matriz->dir->dir != NULL && matriz->dir->dir->emb_hum == emb){
            matriz->dir->dir->info_hum = '*';

            if (matriz->dir->dir->dir != NULL && matriz->dir->dir->dir->emb_hum == emb){
              matriz->dir->dir->dir->info_hum = '*';

              if (matriz->dir->dir->dir->dir != NULL && matriz->dir->dir->dir->dir->emb_hum == emb){
                matriz->dir->dir->dir->dir->info_hum = '*';
              }
            }
          }
        }
        if (matriz->cima != NULL && matriz->cima->emb_hum == emb){
          matriz->cima->info_hum = '*';

          if (matriz->cima->cima != NULL && matriz->cima->cima->emb_hum == emb){
            matriz->cima->cima->info_hum = '*';

            if (matriz->cima->cima->cima != NULL && matriz->cima->cima->cima->emb_hum == emb){
              matriz->cima->cima->cima->info_hum = '*';

              if (matriz->cima->cima->cima->cima != NULL && matriz->cima->cima->cima->cima->emb_hum == emb){
                matriz->cima->cima->cima->cima->info_hum = '*';
              }
            }
          }
        
        }
        if (matriz->baixo != NULL && matriz->baixo->emb_hum == emb){
          matriz->baixo->info_hum = '*';

          if (matriz->baixo->baixo != NULL && matriz->baixo->baixo->emb_hum == emb){
            matriz->baixo->baixo->info_hum = '*';

            if (matriz->baixo->baixo->baixo != NULL && matriz->baixo->baixo->baixo->emb_hum == emb){
              matriz->baixo->baixo->baixo->info_hum = '*';

              if (matriz->baixo->baixo->baixo->baixo != NULL && matriz->baixo->baixo->baixo->baixo->emb_hum == emb){
                matriz->baixo->baixo->baixo->baixo->info_hum = '*';
              }
            }
          }
          
        }

        matriz->info_hum = '*';
      }

      matriz->info_hum = '*';
      tipo_emb = emb->tipo;
      free(emb);
      matriz->emb_hum = NULL;
      return tipo_emb;
    }

    else{
      emb->vida = emb->vida-1;
      matriz->info_hum = '*';
    }

    matriz->info_hum = '*';

    return -1;
  }

}



// Função de jogada do computador
int jogada_computador(casa **matriz, casa **jogadas_com, casa **jogadas_hum, casa **pos_submarino_hum, casa **pos_submarino_com, int *retorno, char jogada[4]){
  int continuar = 0, i=0, j=0, x=0;
  int aleatorio[2], num, a, linha, coluna, quant_m_um = 0, pos_prim_m_um = 0, aux_ret = 0;


  printf("\n---------------------- Vez do Computador ----------------------\n\n");

  // Printa o tabuleiro e os últimos ataques de cada jogador 
  printar_tabuleiro(matriz, jogadas_hum, jogadas_com, jogada);

  // Percorremos o vetor de retorno das jogadas do computador, contamos a quantidade de -1's e anotamos a posição do primeiro '-1'. (Cada -1 representa uma embarcação que foi atacada mas não afundada).
  // Convencionamos as seguintes posições para o vetor de retorno:
  // Caso 1 ataque bem sucedido: [baixo, cima, esq, dir, posição_atual]
  // Caso 2 ataques bem sucedidos (Já sabemos a orientação da embarcação): Horizontal: [aux1, aux2, esq, dir, posição atual], Vertical: [baixo, cima, aux1, aux2, posição_atual]

  while (j<5){
    if (retorno[j] == -1){
      quant_m_um += 1;

      if (quant_m_um == 1){
        pos_prim_m_um = j;
      }
    }
    j += 1;
  }
  
  // se a quantidade de -1's for igual a 0, continuar recebe 1, pois significa que não estamos na iminência de afundar uma embarcação
  if (quant_m_um == 0){
    continuar = 1;
  }

  // Enquanto continuar for igual a 1, tentaremos efetuar uma jogada aleatória
  while (continuar){
    // Sorteamos dois números aleatórios e colocamos no vetor 'aleatorio'
    for(a=0; a<2;a++){
      num = rand();
      aleatorio[a] = num;
    }

    // Utilizando os números aleatórios do vetor, vamos pegar seus restos da divisão por 12 e atribuí-los a 'linha' e 'coluna'
    linha = aleatorio[0] % 12;
    coluna = aleatorio[1] % 12;

    // Percorrendo o vetor de jogadas do computador até que encontremos uma posição NULL no vetor ou até que encontremos a posição de memória correspondente à posição que iríamos atacar 
    i = 0;
    while (jogadas_com[i] != &matriz[linha][coluna] && jogadas_com[i] != NULL){
      i += 1;
    }

    // Se saímos do while por ter encontrado uma posição nula no vetor, prosseguimos com o ataque, anotando a posição de memória no vetor jogadas do computador
    if (jogadas_com[i] == NULL){
      jogadas_com[i] = &matriz[linha][coluna];
      continuar = 0;
      return ataque(&matriz[linha][coluna], computador, pos_submarino_hum, pos_submarino_com, jogadas_hum, jogadas_com);
    }

  }

  // Se temos um caso de ataque bem sucedido, atacaremos as casas adjacentes para descobrir a orientação da embarcação
  if (quant_m_um == 1  && ((pos_prim_m_um == 4)
   || ((pos_prim_m_um == 2) || (pos_prim_m_um == 3)) && (retorno[0] == 0) 
   || ((pos_prim_m_um == 0) || (pos_prim_m_um == 1)) && retorno[2] == 0)){


    // Buscando uma posição vazia no vetor de jogadas realizadas pelo computador    
    i = 0;
    while (jogadas_com[i] != NULL){
      i += 1;
    }

    // Se encontramos uma posição vazia no vetor de ataque e estamos diante do primeiro ataque realizado (que por sinal obteve sucesso), testaremos a casa à sua direita
    if (jogadas_com[i] == NULL && pos_prim_m_um == 4){
      if ((jogadas_com[i-1])->dir != NULL){
        // Verificamos se a casa que desejamos já foi atacada
        x=0;
        while (jogadas_com[x] != (jogadas_com[i-1])->dir && jogadas_com[x] != NULL){
          x += 1;
        }
        // Se não tiver sido atacada, atacaremos e anotaremos no vetor de jogadas a sua posição
        if (jogadas_com[x] == NULL){
          jogadas_com[i] = (jogadas_com[i-1])->dir;
          retorno[2] = -1;

          return ataque((jogadas_com[i-1])->dir, computador, pos_submarino_hum, pos_submarino_com, jogadas_hum, jogadas_com);
        }
        // Se já tiver sido atacada, faremos com que ele ataque na casa da esquerda (ajustando as condições necessárias para ele entrar no próximo if)
        else{
          jogadas_com[i] = jogadas_com[i-1];
          i += 1;
          pos_prim_m_um = 2;
          retorno[4] = 0;
          retorno[2] = -1;
        }
      }
      // Se não houver a casa, faremos com que ele ataque na casa da esquerda (ajustando as condições necessárias para ele entrar no próximo if)
      else{
        jogadas_com[i] = jogadas_com[i-1];
        i += 1;
        pos_prim_m_um = 2;
        retorno[4] = 0;
        retorno[2] = -1;
      }
    }

    // Agora testamos a casa para a esquerda
    if (jogadas_com[i] == NULL && pos_prim_m_um == 2){
      if ((jogadas_com[i-2])->esq != NULL){
        x = 0;
        while(jogadas_com[x] != (jogadas_com[i-2])->esq && jogadas_com[x] != NULL){
          x += 1;
        }
        if (jogadas_com[x] == NULL){
          jogadas_com[i] = (jogadas_com[i-2])->esq;
          retorno[2] = 0;
          retorno[3] = -1;
  
          return ataque((jogadas_com[i-2])->esq, computador, pos_submarino_hum, pos_submarino_com, jogadas_hum, jogadas_com);
        }
        // Se já tiver sido atacada, faremos com que ele ataque na casa de cima (ajustando as condições necessárias para ele entrar no próximo if)
        else{
          jogadas_com[i] = jogadas_com[i-1];
          i += 1;
          pos_prim_m_um = 3;
          retorno[4] = 0;
          retorno[3] = -1;
        }
      }
      // Se não houver a casa, faremos com que ele ataque na casa de cima (ajustando as condições necessárias para ele entrar no próximo if)
      else{
        jogadas_com[i] = jogadas_com[i-1];
        i += 1;
        pos_prim_m_um = 3;
        retorno[4] = 0;
        retorno[3] = -1;
      }
      
      
    }


    // Agora testamos a casa para cima
    if (jogadas_com[i] == NULL && pos_prim_m_um == 3){
      if ((jogadas_com[i-3])->cima != NULL){
        x = 0;
        while(jogadas_com[x] != (jogadas_com[i-3])->cima && jogadas_com[x] != NULL){
          x += 1;
        }
        if (jogadas_com[x] == NULL){
          jogadas_com[i] = (jogadas_com[i-3])->cima;
          retorno[3] = 0;
          retorno[0] = -1;
          return ataque((jogadas_com[i-3])->cima, computador, pos_submarino_hum, pos_submarino_com, jogadas_hum, jogadas_com);
        }
        // Se já tiver sido atacada, faremos com que ele ataque na casa de baixobaixo (ajustando as condições necessárias para ele entrar no próximo if)
        else{
          jogadas_com[i] = jogadas_com[i-1];
          i += 1;
          pos_prim_m_um = 0;
          retorno[4] = 0;
          retorno[0] = -1;
        }
      }
      // Se não houver a casa, faremos com que ele ataque na casa de baixo (ajustando as condições necessárias para ele entrar no próximo if)
      else{
        jogadas_com[i] = jogadas_com[i-1];
        i += 1;
        pos_prim_m_um = 0;
        retorno[4] = 0;
        retorno[0] = -1;
      }
    }

    // Por último, se necessário, atacamos a casa de baixo
    if (jogadas_com[i] == NULL && pos_prim_m_um == 0){
      jogadas_com[i] = (jogadas_com[i-4])->baixo;
      retorno[0] = 0;
      retorno[1] = -1;
      return ataque((jogadas_com[i-4])->baixo, computador, pos_submarino_hum, pos_submarino_com, jogadas_hum, jogadas_com);
    }

  }

  // Se já encontramos a orientação da embarcação (a partir de um ataque bem sucedido em uma das casas adjacentes), continuamos a atacar as casas em tal direção
  else{
    // Auxiliar 1 (aux1) agora representará a distância da casa em que estamos até a primeira casa atacada com sucesso
    //  Para embarcação na horizontal:
    if (pos_prim_m_um == 2 || pos_prim_m_um == 3){
      // Incrementando 'aux1'
      retorno[0] += 1;
    }
    // Para embarcação na vertical:
    if (pos_prim_m_um == 0 || pos_prim_m_um == 1){
      // Iniciando o 'aux1'
      if (retorno[2] == 0){
        retorno[2] = 2;
      }
      // Incrementando 'aux1'
      retorno[2] += 1;
    }

    // Percorrendo o vetor de jogadas até encontrar a primeira posição vazia
    i = 0;
    while (jogadas_com[i] != NULL){
      i += 1;
    }

    // Seguindo o ataque para a casa da direita
    if (jogadas_com[i] == NULL && pos_prim_m_um == 2 && retorno[4] == -1){
      if ((jogadas_com[i-1])->dir != NULL){
        x=0;
        // Verifica intenção de ataque repetido
        while (jogadas_com[x] != (jogadas_com[i-1])->dir && jogadas_com[x] != NULL){
          x += 1;
        }
        if (jogadas_com[x] == NULL){
          jogadas_com[i] = (jogadas_com[i-1])->dir;
          return ataque((jogadas_com[i-1])->dir, computador, pos_submarino_hum, pos_submarino_com, jogadas_hum, jogadas_com);
        }
        // Se a direita já tiver sido atacada, orienta a jogada para a esquerda
        else{
          i += 1;
          retorno[4] = 0;
        }
      }
      // Se a direita for nula, orienta a jogada para a esquerda
      else{
        i += 1;
        retorno[4] = 0;
      }
      
    }
    
    // Se o ataque para a direita for mal sucedido, ataca-se a esquerda
    if (jogadas_com[i] == NULL && pos_prim_m_um == 2 && retorno[4] == 0){
      jogadas_com[i] = (jogadas_com[i-retorno[0]-1])->esq;
      retorno[2] = 0;
      retorno[3] = -1;
      aux_ret = retorno[0]+1;
      retorno[0] = 0;

      return ataque((jogadas_com[i-aux_ret])->esq, computador, pos_submarino_hum, pos_submarino_com, jogadas_hum, jogadas_com);
    }
    
    // Seguindo o ataque para a casa da esquerda, já sabendo que atingiu todas as casas à direita pertencentes a embarcação
    if (jogadas_com[i] == NULL && pos_prim_m_um == 3 && retorno[4] == -1){
      jogadas_com[i] = (jogadas_com[i-1])->esq;

      return ataque((jogadas_com[i-1])->esq, computador, pos_submarino_hum, pos_submarino_com, jogadas_hum, jogadas_com);
    }


    // Seguindo o ataque para cima
    if (jogadas_com[i] == NULL && pos_prim_m_um == 0 && retorno[4] == -1){
      if ((jogadas_com[i-1])->cima != NULL){
        x=0;
        // Verifica intenção de ataque repetido
        while (jogadas_com[x] != (jogadas_com[i-1])->cima && jogadas_com[x] != NULL){
          x += 1;
        }
        if (jogadas_com[x] == NULL){
          jogadas_com[i] = (jogadas_com[i-1])->cima;
  
          return ataque((jogadas_com[i-1])->cima, computador, pos_submarino_hum, pos_submarino_com, jogadas_hum, jogadas_com);
        }
        // Se a casa de cima já tiver sido atacada, orienta a jogada para a casa de baixo
        else{
          i += 1;
          retorno[4] = 0;
        }
      }
      // Se casa de cima for nula, orienta a jogada para a casa de baixo
      else{
        i += 1;
        retorno[4] = 0;
      }
      
    }
    // Se o ataque para cima for mal sucedido, ataca-se a casa de baixo
    if (jogadas_com[i] == NULL && pos_prim_m_um == 0 && retorno[4] == 0){
      jogadas_com[i] = (jogadas_com[i-retorno[2]-1])->baixo;
      retorno[0] = 0;
      retorno[1] = -1;
      aux_ret = retorno[2]+1;
      retorno[2] = 0;

      return ataque((jogadas_com[i-aux_ret])->baixo, computador, pos_submarino_hum, pos_submarino_com, jogadas_hum, jogadas_com);
    }

    // Seguindo o ataque para a casa de baixo, já sabendo que atingiu todas as casas na direção de cima pertencentes a embarcação
    if (jogadas_com[i] == NULL && pos_prim_m_um == 1 && retorno[4] == -1){
      jogadas_com[i] = (jogadas_com[i-1])->baixo;

      return ataque((jogadas_com[i-1])->baixo, computador, pos_submarino_hum, pos_submarino_com, jogadas_hum, jogadas_com);
    }
  }

}




// Função de jogada do humano
int jogada_humano(casa **matriz, casa **jogadas_hum, casa **jogadas_com, casa **pos_submarino_hum, casa **pos_submarino_com, char jogada[4], int ult_retorno){
  char entrada_s[7];
  int jogada_valida = 0, entrada_i[] = {0,0}, linha, coluna, i;

  printf("\n-------------------------- Sua Vez --------------------------\n\n");

  // Impressão do tabuleiro com os últimos ataques de cada jogador
  printar_tabuleiro(matriz, jogadas_hum, jogadas_com, jogada);

  // Se o retorno da função de ataque anterior indicar abatimento, iremos printar a mensagem correspondente
  if (ult_retorno == porta_avioes){
    printf("O Humano acabou de afundar um Porta Aviões do Computador.\n");
  }
  else if (ult_retorno == destroyer){
    printf("O Humano acabou de afundar um Destroyer do Computador.\n");
  }
  else if (ult_retorno == fragata){
    printf("O Humano acabou de afundar uma Fragata do Computador.\n");
  }
  else if (ult_retorno == submarino){
    printf("O Humano acabou de afundar um Submarino do Computador.\n");
  }

  // Enquanto a jogada inserida pelo usuário não for validada, ele continua pedindo para digitar novamente
  while(jogada_valida == 0){
    for (int i=0; i<7; i++){
      entrada_s[i] = ' ';
    }
    printf("Entre a sua jogada: ");
    scanf(" %[^\n]", entrada_s);

    // Valida a entrada em formato de string e obtém os inteiros correspondentes à linha e à coluna
    jogada_valida = tratar_entrada(entrada_s, entrada_i);

    if (jogada_valida){
      i = 0;
      // Atribui os inteiros obtidos pela função de validação à linha e coluna
      linha = entrada_i[0];
      coluna = entrada_i[1];
      // Verifica intenção de repetição de ataque e, caso negativo, encontra uma posição disponível no vetor de jogadas do humano
      while (jogadas_hum[i] != &matriz[linha][coluna] && jogadas_hum[i] != NULL){
        i += 1;
      }
      // Se não houver intenção de repetição de ataque, realiza a jogada na posição pretendida
      if (jogadas_hum[i] == NULL){
        jogadas_hum[i] = &matriz[linha][coluna];
        return ataque(&matriz[linha][coluna], humano, pos_submarino_hum, pos_submarino_com, jogadas_hum, jogadas_com);
      }
      // Se a posição escolhida pelo usuário já tiver sido atacada, informamos que a entrada é inválida e a pedimos novamente
      else{
        for (int i=0; i<7; i++){
          entrada_s[i] = ' ';
        }
        printf("Opção inválida! Tente Novamente.\n");
        jogada_valida = 0;
      }    
    }
    // Se a entrada que o usuário digitou foi inválida, informamos isso e a pedimos novamente
    else{
      for (int i=0; i<7; i++){
        entrada_s[i] = ' ';
      }
      printf("Opção inválida! Tente Novamente.\n");
    }
  }

}


// Função que imprime as últimas coordenadas atacadas por cada jogador
void ultima_jogada(casa **matriz, casa **jogadas, char jogada[4]){
  int i=0, cont=1, linha=0, coluna=0;

  // Verifica se já fora realizado algum ataque por parte do jogador
  if (jogadas[0] != NULL){

    // Encontra a primeira posição disponível no vetor para pegarmos a última posição atacada
    while (jogadas[i] != NULL){
      i += 1;
    }
    
    // Busca no tabuleiro a posição em inteiro de linha e coluna a partir do ponteiro para a casa
    while (cont){
      while(coluna<12 && &matriz[linha][coluna] != jogadas[i-1]){
        coluna +=1;
      }

      if (&matriz[linha][coluna] == jogadas[i-1]){
        linha = linha+1;
        // Atribui o número de dois dígitos à string 'jogada'
        jogada[1] = linha/10 + '0';
        jogada[2] = linha%10 + '0';
        // Atribui a letra à string 'jogada'
        jogada[0] = (char) (coluna + 65);
        
        cont = 0;
      }
      else{
        linha += 1;
        coluna = 0;
      }
    }
  }
}
