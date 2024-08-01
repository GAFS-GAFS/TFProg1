#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct set_t
{
    int size;
    int num;
    bool *flag;
};

struct set_t *set_create(int max)
{
    struct set_t *s = (struct set_t *)malloc(sizeof(struct set_t)); // Aloca memmoria o suficiente para o conjunto

    if (s)
    {
        s->size = max;
        s->num = 0;
        s->flag = (bool *)malloc(max * sizeof(bool)); // Aloca memoria para o array flag

        if (!s->flag)
        {
            free(s);

            return (NULL);
        } // Verifica se a alocação foi bem sucedida

        for (int i = 0; i < max; i++)
        {
            s->flag[i] = false;
        } // Deixa todos os valores identificados como false, ou seja, nao contendo no conjunto
    }
    return (s);
}

struct set_t *set_destroy(struct set_t *s)
{
    if (s)
    {
        free(s->flag);
        free(s);
    } // Libera a memória alocada no array e no conjunto

    return (NULL);
}

int set_add(struct set_t *s, int item)
{
    // Verifica se o conjunto e o item são válidos
    if (s == NULL || s->flag == NULL || item < 0 || item >= s->size)
    {
        return 0; // Falha se o conjunto for inválido ou o item estiver fora do intervalo permitido
    }

    // Verifica se o item já está no conjunto
    if (s->flag[item])
    {
        return 0; // Falha se o item já estiver presente no conjunto
    }

    // Adiciona o item ao conjunto
    s->flag[item] = true;
    s->num++; // Incrementa o número de elementos no conjunto

    return 1; // Sucesso
}

int set_del(struct set_t *s, int item)
{
    if (s && item >= 0 && item < s->size && s->flag[item]) // Verifica se é válido para a remocao
    {
        s->flag[item] = false; // Marca o item ausente no conjunto
        s->num--;

        return (1);
    }
    return (0);
}

int set_in(struct set_t *s, int item)
{
    if (s && item >= 0 && item < s->size) // Verificação sobre validade dos items

    {
        return (s->flag[item]);
    }

    return (0);
}

int set_empty(struct set_t *s)
{
    return (s && s->num == 0); // Deixa o item nulo
}

int set_card(struct set_t *s)
{
    if (s)
    {
        return (s->num); // Retorna o numero de elementos no conjunto
    }

    return (-1);
}

int set_contains(struct set_t *s1, struct set_t *s2)
{
    if (s1 && s2) // Verifica se são validos
    {
        for (int i = 0; i < s2->size; i++)
        {
            if (s2->flag[i] && !s1->flag[i]) // Se o elemento estiver presente em s2 mas nao s1, s1 não contem s2
            {
                return (0);
            }
        }
        return (1);
    }
    return (0);
}

int set_equal(struct set_t *s1, struct set_t *s2)
{
    if (s1 && s2)
    {
        return (s1->num == s2->num && set_contains(s1, s2)); // Retorna verdadeiro se os conjuntos têm o mesmo número de elementos e se s1 contém s2
    }

    return (0);
}

int set_union(struct set_t *s1, struct set_t *s2, struct set_t *s3)
{
    int i, x;

    if ((!s1) || (!s2) || (!s3))
        return 0;

    x = 0;

    for (i = 0; i < s3->size; i++)
    {
        if ((s1->flag[i] == true) || (s2->flag[i] == true))
        {
            s3->flag[i] = 1;
            x++;
        }
    }

    s3->num = x;

    return 1;
}

int set_intersect(struct set_t *s1, struct set_t *s2, struct set_t *s3)
{
    if (s1 && s2 && s3 && s1->size == s2->size && s1->size == s3->size)
    {
        int count = 0; // Variável para contar os elementos na interseção

        for (int i = 0; i < s1->size; i++)
        {
            s3->flag[i] = s1->flag[i] && s2->flag[i]; // Faz a interseção dos conjuntos s1 e s2 em s3

            if (s3->flag[i])
            {
                count++;
            }
        }
        s3->num = count; // Atualiza o número de elementos em s3

        return (count); // Retorna a contagem de elementos na interseção
    }
    return (0);
}

int set_diff(struct set_t *s1, struct set_t *s2, struct set_t *s3)
{
    if (s1 && s2 && s3 && s1->size == s2->size && s1->size == s3->size)
    {
        int count = 0;

        for (int i = 0; i < s1->size; i++)
        {
            s3->flag[i] = s1->flag[i] && !s2->flag[i]; // Faz a diferença dos conjuntos s1 e s2 em s3

            if (s3->flag[i]) // Incrementa o contador se o elemento está presente na diferença
            {
                count++;
            }
        }
        s3->num = count; // Atualiza o número de elementos em s3

        return (count); // Retorna a contagem de elementos na diferença
    }
    return (0);
}

int set_copy(struct set_t *s1, struct set_t *s2)
{
    if (s1 && s2 && s1->size == s2->size)
    {
        s2->num = s1->num; // Copia o número de elementos de s1 para s2

        for (int i = 0; i < s1->size; i++)
        {
            s2->flag[i] = s1->flag[i]; // Copia os elementos de s1 para s2
        }
        return (1); // Retorna verdadeiro caso seja bem sucedido
    }
    return (0);
}

void set_print(struct set_t *s)
{
    if (s)
    {
        printf("[ ");

        for (int i = 0; i < s->size; i++)
        {
            if (s->flag[i])
            {
                printf("%d ", i); // Se o elemento está presente, imprime o indice
            }
        }
        printf("]\n");
    }
}