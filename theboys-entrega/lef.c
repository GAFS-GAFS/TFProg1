#include <stdlib.h>
#include <stdio.h>
#include "lef.h"

struct evento_t *cria_evento(int tempo, int tipo, int dado1, int dado2)
{
    struct evento_t *evento = (struct evento_t *)malloc(sizeof(struct evento_t));

    if (evento != NULL)
    {
        evento->tempo = tempo;
        evento->tipo = tipo;
        evento->dado1 = dado1;
        evento->dado2 = dado2;
    }

    return (evento);
}

struct evento_t *destroi_evento(struct evento_t *e)
{
    free(e);
    return (NULL);
}

struct lef_t *cria_lef()
{
    struct lef_t *lef = (struct lef_t *)malloc(sizeof(struct lef_t));

    if (lef != NULL)
    {
        lef->primeiro = NULL;
    }

    return (lef);
}

struct lef_t *destroi_lef(struct lef_t *l)
{
    struct nodo_lef_t *atual = l->primeiro;

    while (atual != NULL)
    {
        struct nodo_lef_t *proximo = atual->prox;

        destroi_evento(atual->evento); // Free the event first
        free(atual);                   // Then free the node

        atual = proximo;
    }

    free(l);

    return NULL;
}

int insere_lef(struct lef_t *l, struct evento_t *e)
{
    struct nodo_lef_t *novo_nodo = (struct nodo_lef_t *)malloc(sizeof(struct nodo_lef_t));

    if (novo_nodo == NULL)
    {
        return 0;
    }

    novo_nodo->evento = e;
    novo_nodo->prox = NULL;

    if (l->primeiro == NULL || l->primeiro->evento->tempo > e->tempo)
    {

        novo_nodo->prox = l->primeiro;
        l->primeiro = novo_nodo;

        return (1);
    }

    struct nodo_lef_t *anterior = l->primeiro;

    while (anterior->prox != NULL && (anterior->prox->evento->tempo <= e->tempo || (anterior->prox->evento->tipo < e->tipo && anterior->prox->evento->tempo < e->tempo)))
    {
        anterior = anterior->prox;
    }

    novo_nodo->prox = anterior->prox;
    anterior->prox = novo_nodo;

    return (1);
}

struct evento_t *retira_lef(struct lef_t *l)
{
    if (l->primeiro == NULL)
    {
        return NULL;
    }

    struct nodo_lef_t *primeiro_nodo = l->primeiro;

    l->primeiro = l->primeiro->prox;

    struct evento_t *evento = primeiro_nodo->evento;

    free(primeiro_nodo);

    return evento;
}

int vazia_lef(struct lef_t *l)
{
    if (l->primeiro == NULL)
    {
        return 1; // LEF está vazia
    }
    return 0; // LEF não está vazia
}

void imprime_lef(struct lef_t *l)
{
    struct nodo_lef_t *atual = l->primeiro;
    while (atual != NULL)
    {
        printf("tempo %d tipo %d  %d  %d\n", atual->evento->tempo, atual->evento->tipo, atual->evento->dado1, atual->evento->dado2);
        atual = atual->prox;
    }
}
