/*
    Implementa os Eventos e Auxiliares Utilizados no Trabalho The Boys
    Escrito na Linguagem C
    Autor : Gabriel Augusto Fabri Soltovski

*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <stdbool.h>
#include "mundo.h"

/*
    Gerador de Números Aleatórios
*/
int aleat(long int min, long int max);

/*
    Imprime a Lista completa da base especificada
*/
void imprime_lista(base_t *base);

/*
    Cria um conjunto de Habilidades que armazena valores de 0 até n

    Ou seja, caso n seja 3, o conjunto aceita somente os números [0, 1 , 2 , 3 ]
*/
struct set_t *cria_cjt_hab(int n);
/*
    Calcula a Distancia Cartesiana do Ponto A ao Ponto B
*/
long distancia(int x1, int y1, int x2, int y2);

/*
    Trata o Evento CHEGA
    atualiza base de H

se há vagas em B e a fila de espera em B está vazia:
        espera = true
    senão:
        espera = (paciência de H) > (10 * tamanho da fila em B)
    se espera:
        cria e insere na LEF o evento ESPERA (agora, H, B)
    senão:
        cria e insere na LEF o evento DESISTE (agora, H, B)

    Insere na LEF ou o evento ESPERA ou o DESISTE
*/
void chega(int tempo, heroi_t *heroi, base_t *base, struct lef_t *lef_mundo);

/*
    Trata o Evento ESPERA
        adiciona H ao fim da fila de espera de B
        cria e insere na LEF o evento AVISA (agora, B

    Insere na LEF o evento AVISA
*/
void espera(int tempo, heroi_t *heroi, base_t *base, struct lef_t *lef_mundo);
/*
    Trata o Evento DESISTE
        escolhe uma base destino D aleatória
        cria e insere na LEF o evento VIAJA (agora, H, D)

    Insere na LEF o evento VIAJA
*/
void desiste(int tempo, heroi_t *heroi, base_t *base, struct lef_t *lef_mundo);
/*
    Trata o Evento AVISA
    enquanto houver vaga em B e houver heróis esperando na fila:
        retira primeiro herói (H') da fila de B
        adiciona H' ao conjunto de heróis presentes em B
        cria e insere na LEF o evento ENTRA (agora, H', B)

    Insere na LEF o evento ENTRA
*/
void avisa(int tempo, base_t *base, struct lef_t *lef_mundo);
/*
    Trata o Evento ENTRA
    calcula TPB = tempo de permanência na base:
    TPB = 15 + paciência de H * aleatório [1...20]
    cria e insere na LEF o evento SAI (agora + TPB, H, B)

    Insere na LEF o evento SAI
*/
void entra(int tempo, heroi_t *heroi, base_t *base, struct lef_t *lef_mundo);
/*
    Trata o Evento SAI

    retira H do conjunto de heróis presentes em B
    escolhe uma base destino D aleatória
    cria e insere na LEF o evento VIAJA (agora, H, D)
    cria e insere na LEF o evento AVISA (agora, B)

    Insere na LEF o evento VIAJA
*/
void sai(int tempo, heroi_t *heroi, base_t *base, struct lef_t *lef_mundo);
/*
    Trata o Evento VIAJA

    calcula duração da viagem:
    distância = distância cartesiana entre a base atual de H e a base D
    duração = distância / velocidade de H
    cria e insere na LEF o evento CHEGA (agora + duração, H, D)

    Insere LEF o evento CHEGA
*/
void viaja(int tempo, heroi_t *heroi, base_t *base, struct lef_t *lef_mundo, mundo_t *mundo);
/*
    Trata o Evento MISSAO

    calcula a distância de cada base ao local da missão M
    encontra BMP = base mais próxima da missão cujos heróis possam cumpri-la
    se houver uma BMP:
        incrementa a experiência dos heróis presentes na base BMP
    senão:
        cria e insere na LEF o evento MISSAO (T + 24*60, M) para o dia seguinte
*/
void missao(mundo_t *mundo, missao_t *missao, struct lef_t *lef_mundo);
/*
    Trata o Evento FIM

    apresenta as experiências dos heróis
    apresenta as estatísticas das missões
    encerra a simulação

    Termina a Simulação
*/
void fim(int tempo, mundo_t *mundo);