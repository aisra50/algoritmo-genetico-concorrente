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
    int tam_pop = 10;
    int dimensao = 2;
    int tam_torneio = 3;
    double taxa_reproducao = 0.8;
    double taxa_mutacao = 0.05;
    int num_avaliacoes_funcao = 0;

    srand(time(NULL)); rand(); // O primeiro número aleatório gerado parece ter uma forte relação linear com o horario, enviesando a amostragem.

    Populacao pop = inicializa_populacao(tam_pop, dimensao, LIM_MIN, LIM_MAX);
    ordena_populacao_por_fitness(pop);
    computa_fitnesses(pop);
    num_avaliacoes_funcao += pop.tam_populacao;

    //while (num_avaliacoes_funcao < NUM_MAX_AVALIACOES) {
    while(0) {
        // Seleção
        int num_selecionados = (int)(pop.tam_populacao * taxa_reproducao);
        Populacao pop_reprodutora = populacao_vazia(num_selecionados, dimensao, LIM_MIN, LIM_MAX);
        for (int i = 0; i < num_selecionados; i++) {
            pop_reprodutora.individuos[i] = selecao_torneio(pop, tam_torneio);
        }

        // Recombinação
        Populacao filhos = populacao_vazia(num_selecionados / 2, dimensao, LIM_MIN, LIM_MAX);
        for(int i = 0; i < pop_reprodutora.tam_populacao - 1; i += 2){
            Individuo pai1, pai2;
            pai1 = pop_reprodutora.individuos[i];
            pai2 = pop_reprodutora.individuos[i + 1];

            filhos.individuos[i] = recombinacao_blx_alpha(pai1, pai2, ALPHA);
        }
        free_populacao(pop_reprodutora);

        // Mutação
        int num_mutados = (int)(filhos.tam_populacao * taxa_mutacao);
        for (int i = 0; i < num_mutados; i++) {
            // TODO: mutar "num_mutados" dos filhos, selecionados aleatoriamente sem repeticao
        }

        // Mantém os melhores indivíduos, mantendo o tamanho original da população
        integra_filhos(pop, filhos);
        free_populacao(filhos);
    }

        

    return 0;
}