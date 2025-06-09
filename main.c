#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include"algoritmo-genetico.h"
#include"timer.h"

#define LIM_MIN -5.12
#define LIM_MAX 5.12

#define ALPHA 0.5

int num_threads;

int main (int argc, char **argv)
{
    double tempo_inicio, tempo_fim, delta;
    int tam_torneio = 15;
    double taxa_reproducao = 0.8;
    double taxa_mutacao = 0.3;
    int num_avaliacoes_funcao = 0;
    int tam_pop, dimensao, num_max_avaliacoes; // argumentos de linha de comando

    #ifdef CONCORRENTE
    if (argc != 5) {
        fprintf(stderr, "ERRO: Uso: %s <tam_pop> <dimensao> <num_max_avaliacoes> <num_threads>\n", argv[0]);
        exit(1);
    }

    num_threads = atoi(argv[4]);
    #else
    if (argc != 4) {
        fprintf(stderr, "ERRO: Uso: %s <tam_pop> <dimensao> <num_max_avaliacoes>\n", argv[0]);
        exit(1);
    }
    #endif

    tam_pop = atoi(argv[1]);
    dimensao = atoi(argv[2]);
    num_max_avaliacoes = atoi(argv[3]);

    srand(time(NULL)); rand(); // O primeiro número aleatório gerado parece ter uma forte relação linear com o horario, enviesando a amostragem.

    GET_TIME(tempo_inicio);
    Populacao pop = inicializa_populacao(tam_pop, dimensao, LIM_MIN, LIM_MAX);
    
    computa_fitnesses(pop);
    ordena_populacao_por_fitness(pop);
    num_avaliacoes_funcao += pop.tam_populacao;

    #ifdef HISTORICO
    printf("%lf, ", pop.individuos[0].fitness);
    #endif

    while (num_avaliacoes_funcao < num_max_avaliacoes) {
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

        #ifdef HISTORICO
        printf("%lf, ", pop.individuos[0].fitness);
        #endif
    }

    GET_TIME(tempo_fim);
    delta = tempo_fim - tempo_inicio;

    #ifndef HISTORICO
    printf("Best fitness: %lf\n", pop.individuos[0].fitness);

    printf("Tempo de execução: %lf segundos\n", delta);
    #else
    printf("tempo_execucao: %lf\n", delta);
    #endif

    return 0;
}