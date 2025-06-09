#include<math.h>
#include<stdlib.h>
#include"algoritmo-genetico.h"
#include"variaveis-aleatorias.h"

#ifdef CONCORRENTE

#include<pthread.h>

typedef struct {
    Individuo *individuos;
    int inicio;
    int fim;
} Argumentos;

extern int num_threads;

void *computa_fitnesses_task(void *args) {
    Argumentos *argumentos = (Argumentos *)args;
    
    int inicio = argumentos->inicio;
    int fim = argumentos->fim;
    Individuo *individuos = argumentos->individuos;

    for (int i = inicio; i < fim; i++) {
        Individuo *it = &individuos[i];
        it->fitness = rosenbrock(individuos[i]);
    }
}

void computa_fitnesses(Populacao pop) {
    int fatia = pop.tam_populacao / num_threads;
    Argumentos args[num_threads];
    pthread_t tids[num_threads];

    for (int i = 0; i < num_threads; i++) {
        args[i].inicio = i * fatia;
        args[i].individuos = pop.individuos;

        if (i == num_threads - 1) { // A última thread fica com o resto se não for divisível
            args[i].fim = pop.tam_populacao;
        }
        else {
            args[i].fim = args[i].inicio + fatia;
        }

        pthread_create(&tids[i], NULL, computa_fitnesses_task, &args[i]);
    }

    for (int i = 0; i < num_threads; i++) {
        pthread_join(tids[i], NULL);
    }

    return;
}

#else

void computa_fitnesses(Populacao pop) {
    for (int i = 0; i < pop.tam_populacao; i++) {
        Individuo *it = &pop.individuos[i];
        it->fitness = rosenbrock(*it);
    }
    return;
}

#endif

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

Populacao selecao(Populacao pop, double taxa_reproducao, int tam_torneio)
{
    int num_selecionados = (int)(pop.tam_populacao * taxa_reproducao);
    int dimensao = pop.individuos[0].dimensao;
    double min = pop.limite_inferior;
    double max = pop.limite_superior;

    Populacao pop_reprodutora = populacao_vazia(num_selecionados, dimensao, min, max);
    for (int i = 0; i < num_selecionados; i++) {
        pop_reprodutora.individuos[i] = selecao_torneio(pop, tam_torneio);
    }

    return pop_reprodutora;
}

Individuo recombinacao_blx_alpha(Individuo pai1, Individuo pai2, double alpha)
{
    Individuo filho = inicializa_individuo(pai1.dimensao);
    double beta = uniforme(-alpha, 1 + alpha);
    
    for (int i = 0; i < filho.dimensao; i++) { // filho = pai1 + beta * (pai2 - pai1)
        filho.genes[i] = pai1.genes[i] + beta * (pai2.genes[i] - pai1.genes[i]);
        if (filho.genes[i] > 5.12) {
            filho.genes[i] = 5.12;
        }
        if (filho.genes[i] < -5.12) {
            filho.genes[i] = -5.12;
        }
    }

    return filho;
}

Populacao recombinacao(Populacao pop_reprodutora, double alpha)
{
    int num_filhos = pop_reprodutora.tam_populacao / 2;
    int dimensao = pop_reprodutora.individuos[0].dimensao;
    double min = pop_reprodutora.limite_inferior;
    double max = pop_reprodutora.limite_superior;
    
    Populacao filhos = populacao_vazia(num_filhos, dimensao, min, max);

    for(int filhos_gerados = 0; filhos_gerados < filhos.tam_populacao; filhos_gerados++){
        Individuo pai1, pai2;
        pai1 = pop_reprodutora.individuos[filhos_gerados * 2];
        pai2 = pop_reprodutora.individuos[filhos_gerados * 2 + 1];

        filhos.individuos[filhos_gerados] = recombinacao_blx_alpha(pai1, pai2, alpha);
    }

    return filhos;
}

void mutacao_gaussiana(Individuo individuo)
{
    int idx_gene_mutado = rand() % individuo.dimensao;

    individuo.genes[idx_gene_mutado] = normal(individuo.genes[idx_gene_mutado], 0.3);
    if (individuo.genes[idx_gene_mutado] > 5.12) {
        individuo.genes[idx_gene_mutado] = 5.12;
    }
    if (individuo.genes[idx_gene_mutado] < -5.12) {
        individuo.genes[idx_gene_mutado] = -5.12;
    }

    return;
}

void mutacao(Populacao pop, double taxa_mutacao)
{
    int num_mutados = (int)(pop.tam_populacao * taxa_mutacao);

    // Cópia "rasa" dos individuos da população "pop". Alterá-los altera "pop".
    Individuo *mutantes = amostra_uniforme(pop, num_mutados);
    for (int i = 0; i < num_mutados; i++) {
        mutacao_gaussiana(mutantes[i]); // Funciona por ser cópia rasa.
    }
    
    free(mutantes);
}