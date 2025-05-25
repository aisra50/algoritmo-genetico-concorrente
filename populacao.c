#include<stdlib.h>
#include<math.h>
#include"populacao.h"

Populacao inicializa_populacao(int tam_populacao, int dimensao, double min, double max)
{
    Populacao pop;

    pop.tam_populacao = tam_populacao;
    pop.limite_inferior = min;
    pop.limite_superior = max;

    pop.individuos = malloc(pop.tam_populacao * sizeof(Individuo));
    MEMCHECK(pop.individuos);

    for (int i = 0; i < pop.tam_populacao; i++) {
        pop.individuos[i] = individuo_aleatorio(dimensao, pop.limite_inferior, pop.limite_superior);
    }

    return pop;
}

Individuo inicializa_individuo(int dimensao) {
    Individuo individuo;

    individuo.dimensao = dimensao;
    individuo.fitness = INFINITY; // Evitando valores lixo

    individuo.genes = malloc(sizeof(*individuo.genes) * dimensao);
    MEMCHECK(individuo.genes);

    return individuo;
}

double uniforme(double a, double b)
{
    double uniforme_padrao = (double)rand() / (double)RAND_MAX;

    return a + uniforme_padrao * (b - a);
}

Individuo individuo_aleatorio(int dimensao, double min, double max)
{
    Individuo individuo = inicializa_individuo(dimensao);

    for (int i = 0; i < dimensao; i++) {
        individuo.genes[i] = uniforme(min, max);
    }

    return individuo;
}

void print_individuo(Individuo individuo)
{
    printf("[");
    for (int i = 0; i < individuo.dimensao - 1; i++) {
        printf("%lf, ", individuo.genes[i]);
    }
    printf("%lf", individuo.genes[individuo.dimensao - 1]);
    printf("]");

    printf(" Fitness: %lf\n", individuo.fitness);
    
    return;
}