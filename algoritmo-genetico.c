#include<math.h>
#include<stdlib.h>
#include"algoritmo-genetico.h"
#include"variaveis-aleatorias.h"

void computa_fitnesses(Populacao pop) {
    for (int i = 0; i < pop.tam_populacao; i++) {
        Individuo *it = &pop.individuos[i];
        it->fitness = rosenbrock(*it);
    }
}

double rosenbrock(Individuo individuo)
{
    int dimensao = individuo.dimensao;
    double *x = individuo.genes;
    double soma = 0;

    for (int i = 0; i < dimensao - 1; i++) {
        soma += 100 * pow(x[i+1] - pow(x[i], 2), 2) + pow(x[i] - 1, 2); // 100(x_{i+1} - x_i^2)^2 + (x_i - 1)^2
    }

    return soma;
}

Individuo selecao_torneio(Populacao pop, int k)
{
    double melhor_fitness = INFINITY;
    Individuo melhor;

    if (k < 1) {
        fprintf(stderr, "ERRO: Torneio chamado com k = %d - Parâmetro inválido\n");
        exit(2);
    }

    for (int i = 0; i < k; i++) {
        int r = rand() % pop.tam_populacao;

        if (pop.individuos[r].fitness == INFINITY) {
            fprintf(stderr, "ERRO: Torneio chamado em população que não tem todas as fitnesses inicializadas.\n");
            exit(3);
        }

        if (pop.individuos[r].fitness < melhor_fitness) {
            melhor = pop.individuos[r];
            melhor_fitness = melhor.fitness;
        }
    }

    return copia_individuo(melhor);
}

Individuo recombinacao_blx_alpha(Individuo pai1, Individuo pai2, double alpha)
{
    Individuo filho = inicializa_individuo(pai1.dimensao);
    double beta = uniforme(-alpha, 1 + alpha);
    
    for (int i = 0; i < filho.dimensao; i++) { // filho = pai1 + beta * (pai2 - pai1)
        filho.genes[i] = pai1.genes[i] + beta * (pai2.genes[i] - pai1.genes[i]);
    }

    return filho;
}

void mutacao_gaussiana(Individuo individuo)
{
    int idx_gene_mutado = rand() % individuo.dimensao;

    individuo.genes[idx_gene_mutado] = normal(individuo.genes[idx_gene_mutado], 0.2);

    return;
}