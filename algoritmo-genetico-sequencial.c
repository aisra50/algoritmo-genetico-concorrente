#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include"algoritmo-genetico.h"

#define PI 3.14159265359

// double normal(double media, double desvio_padrao);

int main (int argc, char **argv)
{
    int tam_pop = 10;
    int dimensao = 2;

    
    srand(time(NULL)); rand(); // O primeiro número aleatório gerado parece ter uma forte relação linear com o horario, enviesando a amostragem.

    Populacao pop = inicializa_populacao(tam_pop, dimensao, -5.12, 5.12);
    computa_fitnesses(pop);

    // for (int i = 0; i < 10; i++) {
    //     printf("%lf\n", normal(0, 1));
    // }

    return 0;
}

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
        int r = rand() % (pop.tam_populacao + 1);

        if (pop.individuos[r].fitness == INFINITY) {
            fprintf(stderr, "ERRO: Torneio chamado em população que não tem todas as fitnesses inicializadas.\n");
            exit(3);
        }

        if (pop.individuos[r].fitness < melhor_fitness) {
            melhor = pop.individuos[r];
            melhor_fitness = melhor.fitness;
        }
    }

    return melhor;
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

// double normal(double media, double desvio_padrao) {
//     static int tem_guardado = 0;
//     static double guardado;
    
//     double u1, u2;
//     double r, theta;

//     if (tem_guardado) {
//         tem_guardado = 0;
//         return guardado;
//     }

//     do {
//         u1 = uniforme(0, 1);
//     } while (u1 <= 0);

//     u2 = uniforme(0, 1);

//     r = sqrt(-2 * log(u1)) * desvio_padrao;
//     theta = 2 * PI * u2;

//     tem_guardado = 1;
//     guardado = r * cos(theta);

//     return r * sin(theta);
// }

void mutacao_gaussiana(Individuo individuo)
{
    int idx_gene_mutado = rand() % individuo.dimensao;

    // individuo.genes[idx_gene_mutado] = normal(individuo.genes[idx_gene_mutado], 0.2);

    return;
}