#include <stdio.h>
#include <stdlib.h>
#include "lista.h"

lista_t *lista_cria()
{
    lista_t *nova_lista = (lista_t *)malloc(sizeof(lista_t));

    if (nova_lista != NULL)
    {
        nova_lista->head = NULL;
        nova_lista->tail = NULL;
        nova_lista->size = 0;
    } // Cria uma lista com informacoes nulas, ou seja, zeradas.

    return (nova_lista);
}

lista_t *lista_destroi(lista_t *lst)
{
    if (lst != NULL) // caso a lista esteja com alguma informacao, entra na condicao
    {
        lnode_t *atual = lst->head; // O nodo atual recebe o mesmo valor,

        while (atual != NULL)
        {
            lnode_t *proximo = atual->next;
            free(atual);
            atual = proximo;
        } // percorre a lista liberando a memória ocupada por cada nó

        free(lst); // por fim libera a lista completa
    }

    return (NULL);
}

int lista_insere(lista_t *lst, int elem, int pos)
{
    if (lst == NULL)
    {
        return (-1);
    } // confirma se a lista está válida

    lnode_t *novo = (lnode_t *)malloc(sizeof(lnode_t)); // cria um nodo auxiliar

    if (novo == NULL)
    {
        return -1;
    } // verifica se a alocação foi bem sucedida

    novo->val = elem; // recebe no nodo o valor que deseja manipular

    if (pos < 0 || pos >= lst->size)
    {
        novo->next = NULL;
        novo->prev = lst->tail;

        if (lst->size == 0)
        {
            lst->head = novo;
        }
        else
        {
            lst->tail->next = novo;
        }

        lst->tail = novo;
    } // verifica se os ponteiros estão corretamente agrupados, caso não esteja, é atualizado o tamanho da lista
    else
    {
        lnode_t *atual = lst->head;

        for (int i = 0; i < pos; i++) // percorre a lista ate a posicao desejada
        {
            atual = atual->next;
        }

        novo->next = atual;
        novo->prev = atual->prev;
        // para manter a ordem, o nodo novo aponta para o atual, mantendo a conformação da lista

        if (atual->prev != NULL)
        {
            atual->prev->next = novo;
        } // caso o anterior nao seja NULL, quer dizer que nao é o primeio, assim configurado para ter a conexao para apontar para o no anteriror, o
          // novo nó, e o proximo nó da lista
        else
        {
            lst->head = novo;
        } // atualiza o valor do inicio da lista

        atual->prev = novo;
    }

    lst->size++;
    // aumenta uma posição da lista

    return (lst->size);
}

int lista_retira(lista_t *lst, int *elem, int pos)
{
    if (lst == NULL || lst->size == 0)
    {
        return (-1);
    } // verifica se a lista esta vazia

    if (pos < 0)
    {
        pos = lst->size - 1;
    } // se for negatio, eh tomado como o último elemento

    if (pos >= lst->size)
    {
        return (-1);
    } // verifica se a posicao eh valida

    lnode_t *atual = lst->head;

    for (int i = 0; i < pos; i++)
    {
        atual = atual->next;
    } // percorre a lista ate a posicao desejada

    // atualizacao dos ponteiros para manter a consistencia da lista
    if (atual->prev != NULL)
    {
        atual->prev->next = atual->next;
    }
    else
    {
        lst->head = atual->next;
    }

    if (atual->next != NULL)
    {
        atual->next->prev = atual->prev;
    }
    else
    {
        lst->tail = atual->prev;
    }

    *elem = atual->val;
    // retorna a posicao do elemento removido em formato de ponteiro

    free(atual);

    lst->size--;
    // atualiza o tamanho da lista

    return (lst->size);
}

int lista_consulta(lista_t *lst, int *elem, int pos)
{
    if (lst == NULL || lst->size == 0)
    {
        return (-1);
    } // verifica se a lista esta vazia

    if (pos < 0)
    {
        pos = lst->size - 1;
    } // se for negatio, eh tomado como o último elemento

    if (pos >= lst->size)
    {
        return -1;
    } // verifica se a posicao eh valida

    lnode_t *atual = lst->head;

    for (int i = 0; i < pos; i++)
    {
        atual = atual->next;
    } // percorre a lista ate a posicao desejada

    *elem = atual->val; // retorna o valor do elemento na posicao desejada

    return (lst->size); // retorna o tamanho pois não foi alterado
}

int lista_procura(lista_t *lst, int elem)
{
    if (lst == NULL || lst->size == 0)
    {
        return (-1);
    }

    lnode_t *atual = lst->head; // atualiza o ponteiro atual com o inicio da lista

    for (int pos = 0; pos < lst->size; pos++)
    {
        if (atual->val == elem) // compara o valor do nó atual com o elemento da lista
        {
            return (pos);
        }

        atual = atual->next; // move para o proximo elemento da lista
    }

    return (-1);
}

int lista_vazia(lista_t *lst)
{
    return (lst == NULL || lst->size == 0); // verifica se é vazia ou possui 0
}

int lista_tamanho(lista_t *lst)
{
    if (lst == NULL) // verifica se é valido
    {
        return (-1);
    }

    return (lst->size); // retorna o tamanho da lista
}

void lista_imprime(char *nome, lista_t *lst)
{
    printf("%s: [", nome);

    lnode_t *atual = lst->head; // o atual recebe o inicio da lista

    while (atual != NULL)
    {
        printf(" %d", atual->val);
        atual = atual->next;
    } // enquanto não chega no final da lista, é impresso todos os elementos

    printf(" ] (%d elementos)\n", lst->size); // imprime a quantida de elementos na lista
}
