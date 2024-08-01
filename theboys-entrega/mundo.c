#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <stdbool.h>

#include "eventos.h"

heroi_t cria_heroi(int id)
{
    heroi_t heroi;

    heroi.id = id;
    heroi.experiencia = 0;
    heroi.paciencia = aleat(0, 100);
    heroi.velocidade = aleat(50, 5000);
    heroi.cjt_hab = cria_cjt_hab(aleat(1, 3)); // chama a função auxiliar para criar um conjunto de tamanho aleatório entre 1 e 3.

    return (heroi);
}

void inicia_heroi(heroi_t *heroi, struct lef_t *lef_mundo)
{
    int tempo;

    struct evento_t *novo_evento;

    tempo = aleat(0, 4320);
    heroi->base = aleat(0, N_BASES - 1);

    novo_evento = cria_evento(tempo, CHEGA, heroi->id, heroi->base);
    insere_lef(lef_mundo, novo_evento);
    // Insere na LEF o instante em que o Heroi hega e qual a base dele.
}

base_t cria_base(int id)
{
    base_t base;

    base.id = id;
    base.lotacao = aleat(3, 10); // Número máximo de pessoas que podem estar dentro da base.
    base.lx = aleat(0, N_TAMANHO_DO_MUNDO - 1);
    base.ly = aleat(0, N_TAMANHO_DO_MUNDO - 1);
    base.espera = lista_cria();
    base.presentes = set_create(50); // Cria um comnjunto que recebe os Herois entre 0 e 49.

    return (base);
}

missao_t cria_missao(int id)
{

    missao_t missao;

    missao.id = id;
    missao.lx = aleat(0, N_TAMANHO_DO_MUNDO - 1);
    missao.ly = aleat(0, N_TAMANHO_DO_MUNDO - 1);
    missao.tent = 0;
    missao.cjt_hn = cria_cjt_hab(aleat(6, 10)); // Cria um conjunto de habilidades de tamanho entre 6 e 10.

    return (missao);
}

void inicia_missao(missao_t *missao, struct lef_t *lef_mundo)
{
    int tempo;
    struct evento_t *novo_evento;

    tempo = aleat(0, T_FIM_DO_MUNDO);

    novo_evento = cria_evento(tempo, MISSAO, missao->id, 0);
    insere_lef(lef_mundo, novo_evento);
    // Insere na LEF a missão que vai aparecer no momento x para as bases tentar realiza-las.
}

mundo_t *cria_mundo(struct lef_t *lef_mundo)
{
    int i;

    mundo_t *m;
    struct evento_t *evento;

    m = malloc(sizeof(mundo_t));
    m->herois = malloc(N_HEROIS * sizeof(heroi_t));
    m->bases = malloc(N_BASES * sizeof(base_t));
    m->missoes = malloc(N_MISSOES * sizeof(missao_t));
    // Realiza o Malloc para todas as entidades do mundo.

    m->n_heroi = N_HEROIS;
    m->n_bases = N_BASES;
    m->lx = N_TAMANHO_DO_MUNDO;
    m->ly = N_TAMANHO_DO_MUNDO;
    m->n_habilidades = N_HABILIDADES;
    m->n_missoes = N_MISSOES;
    m->relogio = T_INICIO;
    m->m_adiadas = 0;
    m->m_cumpridas = 0;
    // Realiza as atribuições das informações básicas do Mundo.

    for (i = 0; i < N_HEROIS; i++)
    {
        m->herois[i] = cria_heroi(i);
        inicia_heroi(&m->herois[i], lef_mundo);
    } // Laço responsável por realizar as ações para todos os Herois.

    for (i = 0; i < N_BASES; i++)
    {
        m->bases[i] = cria_base(i);
    } // Laço responsável por realizar as ações para todas as Bases.

    for (i = 0; i < N_MISSOES; i++)
    {
        m->missoes[i] = cria_missao(i);
        inicia_missao(&m->missoes[i], lef_mundo);
    } // Laço responsável por realizar as ações para todas as Missoes.

    evento = cria_evento(T_FIM_DO_MUNDO, FIM, N_HEROIS, N_MISSOES);
    insere_lef(lef_mundo, evento);
    // Insere na LEF o evento que para as iterações com a LEF.

    return m;
}

void destroi_mundo(mundo_t *m)
{
    int i;

    for (i = 0; i < m->n_heroi; i++)
    {
        set_destroy(m->herois[i].cjt_hab);
    } // Laço para destruir os mallocs realizados na entidade herois.

    for (i = 0; i < m->n_bases; i++)
    {
        lista_destroi(m->bases[i].espera);
        set_destroy(m->bases[i].presentes);
    } // Laço para destruir os mallocs realizados na entidade base.

    for (i = 0; i < m->n_missoes; i++)
    {
        set_destroy(m->missoes[i].cjt_hn);
    } // Laço para destruir os mallocs realizados na entidade missao.

    free(m->herois);
    free(m->bases);
    free(m->missoes);

    free(m);
    // Realiza os frees dos mallocs que foram feitos durante o inicia mundo.
}

void laco_lef(mundo_t *m, struct lef_t *lef_mundo)
{
    struct evento_t *evento_atual;

    while (lef_mundo && m->relogio < T_FIM_DO_MUNDO) // Enquanto tiver eventos na LEF ou o Relógio for menor que o fim do Mundo.
    {
        evento_atual = retira_lef(lef_mundo);

        m->relogio = evento_atual->tempo;

        switch (evento_atual->tipo)
        {
        case CHEGA:
            chega(m->relogio, &m->herois[evento_atual->dado1], &m->bases[evento_atual->dado2], lef_mundo);
            break;
        case ESPERA:
            espera(m->relogio, &m->herois[evento_atual->dado1], &m->bases[evento_atual->dado2], lef_mundo);
            break;
        case DESISTE:
            desiste(m->relogio, &m->herois[evento_atual->dado1], &m->bases[evento_atual->dado2], lef_mundo);
            break;
        case AVISA:
            avisa(m->relogio, &m->bases[evento_atual->dado1], lef_mundo);
            break;
        case ENTRA:
            entra(m->relogio, &m->herois[evento_atual->dado1], &m->bases[evento_atual->dado2], lef_mundo);
            break;
        case SAI:
            sai(m->relogio, &m->herois[evento_atual->dado1], &m->bases[evento_atual->dado2], lef_mundo);
            break;
        case VIAJA:
            viaja(m->relogio, &m->herois[evento_atual->dado1], &m->bases[evento_atual->dado2], lef_mundo, m);
            break;
        case MISSAO:
            missao(m, &m->missoes[evento_atual->dado1], lef_mundo);
            break;
        case FIM:
            fim(m->relogio, m);
            break;
        }
        // Laço que trata cada evento presente na LEF e atribiu os dados quase que uniformemente em cada função.
        destroi_evento(evento_atual);
    }
}