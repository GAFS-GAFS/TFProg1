#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <stdbool.h>
#include "lef.h"
#include "lista.h"
#include "set.h"
#include "eventos.h"

int aleat(long int min, long int max)
{
    return (min + (rand() % (max - min + 1)));
} // Função geradora de Aleatorios normal.

// Nova função de imprime lista pois a antiga imprimia coisas a mais.
void imprime_lista(base_t *base)
{
    lnode_t *atual;

    printf("[");

    atual = base->espera->head; // O atual recebe o inicio da lista.
    while (atual != NULL)
    {
        printf(" %d", atual->val);
        atual = atual->next;
    } // Enquanto não chega no final da lista, é impresso todos os elementos.

    printf(" ] \n");
}

struct set_t *cria_cjt_hab(int n)
{
    int hab;
    struct set_t *hab_criada; // Cria um conjunto que vai ser preenchido com as habilidades aleatórias.

    hab_criada = set_create(N_HABILIDADES);

    while (set_card(hab_criada) < n)
    {
        hab = aleat(1, 10);
        if (set_add(hab_criada, hab))
        {
            hab_criada->size++;
        }
    } // Enquanto o tamnho for menor do que o passado do tamanho do conjunto, adiciona um núemro aleatório e incrementa o tamanho do conjunto.

    return (hab_criada);
}

long distancia(int x1, int y1, int x2, int y2)
{
    long dist;

    dist = sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));

    return dist;
} // Calcula a distancia cartesiana do ponto 1 e 2.

void chega(int tempo, heroi_t *heroi, base_t *base, struct lef_t *lef_mundo)
{
    struct evento_t *evento;
    bool espera;

    heroi->base = base->id; // Atualiza a base do heroi oom a nova base em que ele chegou.

    if (set_empty(base->presentes) && lista_vazia(base->espera))
    {
        espera = true; // se tiver espaço e a lista de espera estiver vazia ele espera para entrar.
    }
    else
    {
        espera = (heroi->paciencia > 10 * lista_tamanho(base->espera));
        // Caso não cumpra os requisitos, o heroi ve se ele espera ou não.
    }

    if (espera)
    {
        evento = cria_evento(tempo, ESPERA, heroi->id, base->id);
        insere_lef(lef_mundo, evento);

        printf("%6d: CHEGA  HEROI %2d BASE %d (%2d/%2d) ESPERA\n", tempo, heroi->id, base->id, base->presentes->num, base->lotacao);
    } // Caso decida esperar, o evento é inserido na LEF e é informado em qual base ele vai esperar.
    else
    {
        evento = cria_evento(tempo, DESISTE, heroi->id, base->id);
        insere_lef(lef_mundo, evento);

        printf("%6d: CHEGA  HEROI %2d BASE %d (%2d/%2d) DESISTE\n", tempo, heroi->id, base->id, base->presentes->num, base->lotacao);
    } // Caso ele não tenha paciencia para esperar, ele desiste de entrar na base em que ele chegou.
}

void espera(int tempo, heroi_t *heroi, base_t *base, struct lef_t *lef_mundo)
{
    struct evento_t *evento;

    lista_insere(base->espera, heroi->id, lista_tamanho(base->espera));
    // Insere o herói na lista de espera.

    evento = cria_evento(tempo, AVISA, base->id, 0);
    insere_lef(lef_mundo, evento);

    printf("%6d: ESPERA HEROI %2d BASE %d (%2d)\n", tempo, heroi->id, base->id, base->espera->size);
    // Cria o evento avisa, e o heroi espera na fila para entrar na base.
}

void desiste(int tempo, heroi_t *heroi, base_t *base, struct lef_t *lef_mundo)
{
    int dest;
    struct evento_t *evento;

    dest = aleat(0, N_BASES - 1);

    evento = cria_evento(tempo, VIAJA, heroi->id, dest);
    insere_lef(lef_mundo, evento);

    printf("%6d: DESIST HEROI %2d BASE %d\n", tempo, heroi->id, base->id);
    // O heroi vai pra uma base aleátoria, podendo voltar para a mesma que ele saiu.
}

void avisa(int tempo, base_t *base, struct lef_t *lef_mundo)
{
    struct evento_t *evento;

    printf("%6d: AVISA  PORTEIRO BASE %d (%2d/%2d) FILA ", tempo, base->id, base->presentes->num, base->lotacao);

    imprime_lista(base);

    while (base->presentes->num < base->lotacao && !lista_vazia(base->espera)) // Enquanto os presentes na base não atingirem a lotação, ou a lista não estiver fazia, o laço acontece.
    {
        int elem_rem;

        lista_retira(base->espera, &elem_rem, 0);
        set_add(base->presentes, elem_rem);
        // Retira o heroi da fila de espera e deixa ele entrar na base.

        evento = cria_evento(tempo, ENTRA, elem_rem, base->id);
        insere_lef(lef_mundo, evento);
        // Cria e insere o evento na LEF no mesmo momento.
    }
}

void entra(int tempo, heroi_t *heroi, base_t *base, struct lef_t *lef_mundo)
{
    int temp_per_bas, saida_base;

    struct evento_t *evento;

    temp_per_bas = 15 + heroi->paciencia * (aleat(1, 20));
    // Calcula o tempo de permanencia na Base.

    saida_base = tempo + temp_per_bas;
    // Calcula o tempo de saida da soma do tempo que ele entrou mais os minutos que ele irá ficar.

    evento = cria_evento(saida_base, SAI, heroi->id, base->id);
    insere_lef(lef_mundo, evento);

    printf("%6d: ENTRA  HEROI %2d BASE %d (%2d/%2d) SAI %d \n", tempo, heroi->id, base->id, base->presentes->num, base->lotacao, saida_base);
    // Cria e insere na LEF o Evento de saída do heroi da base em que ele está.
}

void sai(int tempo, heroi_t *heroi, base_t *base, struct lef_t *lef_mundo)
{
    int dest;
    struct evento_t *evento;

    set_del(base->presentes, heroi->id);
    // Remove da base.

    dest = aleat(0, N_BASES - 1);
    // Base destino aleatória.

    evento = cria_evento(tempo, AVISA, base->id, 0);
    insere_lef(lef_mundo, evento);
    // Avisa o porteiro que um saiu da base e pode trazer mais um pra dentro.

    evento = cria_evento(tempo, VIAJA, heroi->id, dest);
    insere_lef(lef_mundo, evento);

    printf("%6d: SAI    HEROI %2d BASE %d (%2d/%2d)\n", tempo, heroi->id, base->id, base->presentes->num, base->lotacao);
    // O heroi sai e viaja para a base destino que foi definida anteriormente.
}

void viaja(int tempo, heroi_t *heroi, base_t *base, struct lef_t *lef_mundo, mundo_t *mundo)
{
    long dist = 0;
    int dur = 0;
    int temp_de_v;

    struct evento_t *evento;

    dist = distancia(base->lx, base->ly, mundo->bases[heroi->base].lx, mundo->bases[heroi->base].ly);
    // Calcula a distancia cartesiana da base atual para a base destino.

    dur = dist / heroi->velocidade;
    // Duração da viagem.

    temp_de_v = tempo + dur;
    // Tempo de viagem.

    evento = cria_evento(temp_de_v, CHEGA, heroi->id, base->id);
    insere_lef(lef_mundo, evento);

    printf("%6d: VIAJA  HEROI %2d DA BASE %d PARA BASE %d DIST %ld VEL %d CHEGA %d\n", tempo, heroi->id, heroi->base, base->id, dist, heroi->velocidade, temp_de_v);
    // O heroi viaja da base x para a Y e chega no tempo que durou a viagem.
}

void missao(mundo_t *mundo, missao_t *missao, struct lef_t *lef_mundo)
{
    int i, j, min_idx, temp;
    bool conf = false;

    struct base_missao bases_l[N_BASES];
    struct base_missao aux;
    struct evento_t *evento;

    for (i = 0; i < N_BASES; i++)
    {
        bases_l[i].base_ident = mundo->bases[i].id;
        bases_l[i].dist_final = distancia(mundo->bases[i].lx, mundo->bases[i].ly, missao->lx, missao->ly);
        bases_l[i].hab_base = set_create(N_HABILIDADES + 1);
    } // Inicialza o vetor de bases que vão ser manipulados dentro do evento missão.

    for (i = 0; i < N_BASES - 1; i++)
    {
        min_idx = i;

        for (j = i + 1; j < N_BASES; j++)
        {
            if (bases_l[j].dist_final < bases_l[min_idx].dist_final)
            {
                min_idx = j;
            }
        }

        aux = bases_l[i];
        bases_l[i] = bases_l[min_idx];
        bases_l[min_idx] = aux;
        // Troca simples.
    } // Realiza a ordenação pela distancia da base mais proxima para a base mais distante da missão.

    for (i = 0; i < N_BASES; i++)
    {
        for (j = 0; j < N_HEROIS; j++)
        {
            if (set_in(mundo->bases[i].presentes, j))
            {
                set_union(bases_l[i].hab_base, mundo->herois[j].cjt_hab, bases_l[i].hab_base);
            } // Caso o Heroi j esteja na base i, é adicionado suas habilidades dentro do conjunto de habilidades da base.
        }
    }

    printf("%6d: MISSAO %d TENT %d HAB REQ: ", mundo->relogio, missao->id, missao->tent);
    set_print(missao->cjt_hn);

    for (i = 0; i < N_BASES; i++)
    {
        printf("%6d: MISSAO %d BASE %d DIST %d HEROIS ", mundo->relogio, missao->id, bases_l[i].base_ident, bases_l[i].dist_final);
        set_print(mundo->bases[i].presentes);
        // Imprime as informações e os herois presentes dentro da base i.

        for (j = 0; j < N_HEROIS; j++)
        {
            if (set_in(mundo->bases[i].presentes, j))
            {
                printf("%6d: MISSAO %d HAB HEROI %2d: ", mundo->relogio, missao->id, mundo->herois[j].id);
                set_print(mundo->herois[j].cjt_hab);
            } // Se o heroí j estiver na base i, imprime o conjunto de habilidades do heroi.
        }

        printf("%6d: MISSAO %d UNIAO HAB BASE %d: ", mundo->relogio, missao->id, bases_l[i].base_ident);
        set_print(bases_l[i].hab_base);

        if (set_contains(bases_l[i].hab_base, missao->cjt_hn))
        {

            for (j = 0; j < N_HEROIS; j++)
            {
                if (set_in(mundo->bases[i].presentes, j))
                {
                    mundo->herois[j].experiencia++;
                }
            }
            printf("%6d: MISSAO %d CUMPRIDA BASE %d \n", mundo->relogio, missao->id, bases_l[i].base_ident);
            conf = true; // A missão foi cumprida, define conf como verdadeiro
            mundo->m_cumpridas++;
            mundo->missoes[missao->id].tent++;
            break;
        } // Se as habilidades da base forem compativeis com a da missão, os herois ganham experiencias
          // e é iterado o total de missoões cumpridas e é adicionado uma tentativa na missão.
    }

    if (!conf)
    {
        printf("%6d: MISSAO %d IMPOSSIVEL \n", mundo->relogio, missao->id);
        mundo->missoes[missao->id].tent++;
        mundo->m_adiadas++;
        temp = mundo->relogio + 24 * 60;
        evento = cria_evento(temp, MISSAO, missao->id, 0);
        insere_lef(lef_mundo, evento);
    } // Caso o Laço tenha acabado e a variavel conf for false, a missão não foi cumprida, é iterado o total de missoes adiadas, e o total de tentativas da missao
      // e a missão é definida para o dia seguinte no mesmo horário.

    for (i = 0; i < N_BASES; i++)
    {
        set_destroy(bases_l[i].hab_base);
    } // Destroi as bases criadas localmente
}

void fim(int tempo, mundo_t *mundo)
{
    int i;
    int total_tent = 0;
    int tent;
    int min_tent = 0;
    int max_tent = 0;
    int total_missoes = N_MISSOES;
    double media = 0.0;

    printf("%6d: FIM\n", tempo);

    for (i = 0; i < N_HEROIS; i++)
    {
        printf("HEROI %2d PAC %3d VEL %4d EXP %4d HABS", mundo->herois[i].id, mundo->herois[i].paciencia, mundo->herois[i].velocidade, mundo->herois[i].experiencia);
        set_print(mundo->herois[i].cjt_hab);
    } // Imprime todas as informações de todos os Heróis do mundo.

    for (i = 0; i < N_MISSOES; i++)
    {
        tent = mundo->missoes[i].tent;

        total_tent += tent;

        if (i == 0 || tent < min_tent)
        {
            min_tent = tent;
        }

        if (i == 0 || tent > max_tent)
        {
            max_tent = tent;
        }
    } // Calcula o total de tentivas realizadas, qual foi o máximo e qual foi o minimo de tentativas.

    if (total_missoes > 0)
    {
        media = (double)total_tent / total_missoes;
    } // Calcula a média de tentativas das missões.

    printf("MISSOES CUMPRIDAS %d/%d (%.2f%%)\n", mundo->m_cumpridas, total_missoes, ((float)mundo->m_cumpridas / total_missoes) * 100);
    printf("TENTATIVAS/MISSAO: MIN %d, MAX %d, MEDIA %.2f\n", min_tent, max_tent, media);

    // Imprime as informações das missoes;
}
