#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include"algoritmo-genetico.h"
#include"timer.h"

#define LIM_MIN -5.12
#define LIM_MAX 5.12

#define ALPHA 0.5

int main (int argc, char **argv)
{
    double tempo_inicio, tempo_fim, delta;
    double inicio_temp, fim_temp;

    double tempo_selecao = 0, tempo_recomb = 0, tempo_mutacao = 0;
    double tempo_elitismo_fitnesses = 0, tempo_elitismo_integra = 0;
    double tempo_inicializacao = 0;

    int tam_torneio = 15;
    double taxa_reproducao = 0.8;
    double taxa_mutacao = 0.3;
    int num_avaliacoes_funcao = 0;

    int tam_pop, dimensao, num_max_avaliacoes; // argumentos de linha de comando

    if (argc != 4) {
        fprintf(stderr, "ERRO: Uso: %s <tam_pop> <dimensao> <num_max_avaliacoes>\n", argv[0]);
        exit(1);
    }

    tam_pop = atoi(argv[1]);
    dimensao = atoi(argv[2]);
    num_max_avaliacoes = atoi(argv[3]);

    srand(time(NULL)); rand(); // O primeiro número aleatório gerado parece ter uma forte relação linear com o horario, enviesando a amostragem.

    GET_TIME(tempo_inicio);

    Populacao pop = inicializa_populacao(tam_pop, dimensao, LIM_MIN, LIM_MAX);
    computa_fitnesses(pop);
    ordena_populacao_por_fitness(pop);
    num_avaliacoes_funcao += pop.tam_populacao;

    GET_TIME(fim_temp);
    tempo_inicializacao += fim_temp - tempo_inicio;

    while (num_avaliacoes_funcao < num_max_avaliacoes) {
        GET_TIME(inicio_temp);
        // Seleção
        Populacao pop_reprodutora = selecao(pop, taxa_reproducao, tam_torneio);
        
        GET_TIME(fim_temp);
        tempo_selecao += fim_temp - inicio_temp;

        GET_TIME(inicio_temp);
        // Recombinação
        Populacao filhos = recombinacao(pop_reprodutora, ALPHA);
        free_populacao(pop_reprodutora);
        
        GET_TIME(fim_temp);
        tempo_recomb += fim_temp - inicio_temp;

        GET_TIME(inicio_temp);
        // Mutação
        mutacao(filhos, taxa_mutacao);

        GET_TIME(fim_temp);
        tempo_mutacao += fim_temp - inicio_temp;
        
        GET_TIME(inicio_temp);
        // Elitismo
        /* Mantém os melhores indivíduos, mantendo o tamanho original da população */
        computa_fitnesses(filhos);
        GET_TIME(fim_temp);
        tempo_elitismo_fitnesses += fim_temp - inicio_temp;
        
        GET_TIME(inicio_temp);
        num_avaliacoes_funcao += filhos.tam_populacao;
        integra_filhos(&pop, filhos); // Faz cópias dos indivíduos em "filhos". Dealoca membros de pop que forem descartados
        free_populacao(filhos);
        GET_TIME(fim_temp);
        tempo_elitismo_integra += fim_temp - inicio_temp;
    }

    GET_TIME(tempo_fim);

    delta = tempo_fim - tempo_inicio;

    printf("Best fitness: %lf\n", pop.individuos[0].fitness);

    printf("\nTempo de inicialização: %lf segundos (%.2lf%% total)\n", tempo_inicializacao, 100 * tempo_inicializacao / delta);
    printf("Tempo de seleção: %lf segundos (%.2lf%% total)\n", tempo_selecao, 100 * tempo_selecao / delta);
    printf("Tempo de recombinação: %lf segundos (%.2lf%% total)\n", tempo_recomb, 100 * tempo_recomb / delta);
    printf("Tempo de mutação: %lf segundos (%.2lf%% total)\n", tempo_mutacao, 100 * tempo_mutacao / delta);
    printf("Tempo de elitismo (computando fitnesses): %lf segundos (%.2lf%% total)\n", tempo_elitismo_fitnesses, 100 * tempo_elitismo_fitnesses / delta);
    printf("Tempo de elitismo (integrando filhos à população): %lf segundos (%.2lf%% total)\n", tempo_elitismo_integra, 100 * tempo_elitismo_integra / delta);
    
    printf("\nTempo total de execução: %lf segundos\n", delta);
    
    double metrificado = tempo_selecao + tempo_recomb + tempo_mutacao + 
                         tempo_elitismo_fitnesses + tempo_elitismo_integra + 
                         tempo_inicializacao;
    printf("Tempo total metrificado: %lf segundos (%.2lf%% total)", metrificado, 100 * metrificado / delta);
    return 0;
}