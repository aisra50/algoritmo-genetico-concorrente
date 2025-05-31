#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include"algoritmo-genetico.h"

#define NUM_MAX_AVALIACOES 1000
#define LIM_MIN -5.12
#define LIM_MAX 5.12

#define ALPHA 0.5

int main (int argc, char **argv)
{
    int tam_pop = 100;
    int dimensao = 2;
    int tam_torneio = 3;
    double taxa_reproducao = 0.8;
    double taxa_mutacao = 0.05;
    int num_avaliacoes_funcao = 0;

    srand(time(NULL)); rand(); // O primeiro número aleatório gerado parece ter uma forte relação linear com o horario, enviesando a amostragem.

    Populacao pop = inicializa_populacao(tam_pop, dimensao, LIM_MIN, LIM_MAX);
    computa_fitnesses(pop);
    num_avaliacoes_funcao += pop.tam_populacao;

    while (num_avaliacoes_funcao < NUM_MAX_AVALIACOES) {
        // Seleção
        Populacao pop_reprodutora = selecao(pop, taxa_reproducao, tam_torneio);

        // Recombinação
        Populacao filhos = recombinacao(pop_reprodutora, ALPHA);
        free_populacao(pop_reprodutora);

        // Mutação
        mutacao(filhos, taxa_mutacao);
        
        // Elitismo
        /* Mantém os melhores indivíduos, mantendo o tamanho original da população */
        computa_fitnesses(filhos);
        num_avaliacoes_funcao += filhos.tam_populacao;
        integra_filhos(&pop, filhos); // Faz cópias dos indivíduos em "filhos". Dealoca membros de pop que forem descartados
        free_populacao(filhos);
    }

    for (int i = 0; i < pop.tam_populacao; i++) 
    {
        print_individuo(pop.individuos[i]);
    }

    printf("Best:\n");
    print_individuo(pop.individuos[0]);

    return 0;
}