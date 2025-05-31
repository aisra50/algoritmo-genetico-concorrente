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
        int num_selecionados = (int)(pop.tam_populacao * taxa_reproducao);
        Populacao pop_reprodutora = populacao_vazia(num_selecionados, dimensao, LIM_MIN, LIM_MAX);
        for (int i = 0; i < num_selecionados; i++) {
            pop_reprodutora.individuos[i] = selecao_torneio(pop, tam_torneio);
        }

        // Recombinação
        Populacao filhos = populacao_vazia(num_selecionados / 2, dimensao, LIM_MIN, LIM_MAX);
        
        for(int filhos_gerados = 0; filhos_gerados < filhos.tam_populacao; filhos_gerados++){
            Individuo pai1, pai2;
            pai1 = pop_reprodutora.individuos[filhos_gerados * 2];
            pai2 = pop_reprodutora.individuos[filhos_gerados * 2 + 1];

            filhos.individuos[filhos_gerados] = recombinacao_blx_alpha(pai1, pai2, ALPHA);
        }
        free_populacao(pop_reprodutora);

        // Mutação
        int num_mutados = (int)(filhos.tam_populacao * taxa_mutacao);
        // Cópia "rasa" dos individuos da população "filhos". Alterá-los altera "filhos".
        Individuo *mutantes = amostra_uniforme(filhos, num_mutados);
        for (int i = 0; i < num_mutados; i++) {
            mutacao_gaussiana(mutantes[i]); // Funciona por ser cópia rasa.
        }
        free(mutantes);

        // Mantém os melhores indivíduos, mantendo o tamanho original da população
        computa_fitnesses(filhos);
        num_avaliacoes_funcao += filhos.tam_populacao;

        integra_filhos(&pop, filhos);
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