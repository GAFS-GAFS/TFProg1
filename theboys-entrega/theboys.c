#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <stdbool.h>
#include "mundo.h"
#include "lef.h"
#include "lista.h"
#include "set.h"

int main()
{

  mundo_t *m;
  struct lef_t *lef_mundo = cria_lef();

  // Cria o mundo e a LEF que são globais.

  m = cria_mundo(lef_mundo);

  laco_lef(m, lef_mundo);

  destroi_mundo(m);

  destroi_lef(lef_mundo);
  // destroi a LEF que foi alocada.

  return (0);
}