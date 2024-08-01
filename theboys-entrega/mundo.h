/*
    Implementa o Laço principal e as Funções do Mundo Utilizados no Trabalho The Boys
    Escrito na Linguagem C
    Autor : Gabriel Augusto Fabri Soltovski

*/

#include "lef.h"
#include "lista.h"
#include "set.h"

// Defines de váriaveis Globais
#define T_INICIO (0)
#define T_FIM_DO_MUNDO (525600)
#define N_TAMANHO_DO_MUNDO (20000)
#define N_HABILIDADES (10)
#define N_HEROIS (N_HABILIDADES * 5)
#define N_BASES (N_HEROIS / 6)
#define N_MISSOES T_FIM_DO_MUNDO / 100

// Defines para a analise dos Eventos
#define CHEGA (0)
#define ESPERA (1)
#define DESISTE (2)
#define AVISA (3)
#define ENTRA (4)
#define SAI (5)
#define VIAJA (6)
#define MISSAO (7)
#define FIM (8)

typedef struct heroi
{
    int id;
    int experiencia;
    int paciencia;
    int velocidade;
    int base;
    struct set_t *cjt_hab; // conjunto de habilidades do heroi.
} heroi_t;

typedef struct base
{
    int id;
    int lotacao;
    int lx, ly;
    struct set_t *presentes; // conjunto de herois presentes na base.
    lista_t *espera;         // fila de espera para entrar na base.
} base_t;

typedef struct missao
{
    int id;
    int tent;
    int lx, ly;           // localizaoes x e y no plano cartesiano.
    struct set_t *cjt_hn; // conjunto de habilidades necessarias para concluir a missao.

} missao_t;

typedef struct mundo
{
    int n_habilidades;
    int lx, ly;
    int relogio, m_cumpridas, m_adiadas; // conta as missoes cumpridas ou adiadas.

    int n_heroi;
    heroi_t *herois; // todos os herois do mundo.

    int n_bases;
    base_t *bases; // todas as bases do mundo.

    int n_missoes;
    missao_t *missoes; // todas as missoes do mundo.
} mundo_t;

struct base_missao
{
    int base_ident;
    int dist_final;
    struct set_t *hab_base; // habilidades da Base, que são todas as habilidades dos herois presentes na base.
};                          // única struct criada fora das que foram pedidas, ela reúne informações que são tratadas no evento missao.

/*
    Inicia o heroi, o fazendo chegar em alguma báse entre 0 e N_BASES, e entre 0 e 4320 (3 dias).
*/
void inicia_heroi(heroi_t *heroi, struct lef_t *lef_mundo);

/*
    Inicia a Missao, escolhe um tempo entre 0 e FIM_DO_MUNDO e insere a missão na lef para ela ser realizada ou não.
*/
void inicia_missao(missao_t *missao, struct lef_t *lef_mundo);

/*
    Destroi todas as informações alocadas dinamicamente no mundo.
*/
void destroi_mundo(mundo_t *m);

/*
    Laço que trata os eventos presentes na LEF, retirando eles da lista e analisando qual é.
*/
void laco_lef(mundo_t *mundo, struct lef_t *lef);

/*
    Inicia as informações básicas do heroi, como conjunto de hab, paciencia, velocidade e o id que é sequencial.
*/
heroi_t cria_heroi(int id);

/*
    Inicia informações básicas da Base, como o Local que ela se hospeda, lotação que é um núemro inteiro
    e cria conjuntos e filas que vierem a ser pertinentes.
*/
base_t cria_base(int id);

/*
    Inicias as informações como o local em que a missão vai ocorrer, as habilidades necessarias para cumpri-la.
*/
missao_t cria_missao(int id);

/*
    Reúne todas as informações do Mundo, e realiza os laços para as iterações em missão, base e herois, e insere
    o evento fim no final da lef.
*/
mundo_t *cria_mundo(struct lef_t *lef_mundo);
