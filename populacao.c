#include<stdlib.h>
#include<math.h>
#include<stdio.h>
#include<string.h>

#include"populacao.h"
#include"variaveis-aleatorias.h"

Individuo inicializa_individuo(int dimensao) 
{
    Individuo individuo;

    individuo.dimensao = dimensao;
    individuo.fitness = INFINITY; // Evitando valores lixo

    individuo.genes = malloc(sizeof(*individuo.genes) * dimensao);
    MEMCHECK(individuo.genes);

    return individuo;
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

    printf("\nFitness: %lf\n", individuo.fitness);
    
    return;
}

Individuo copia_individuo(Individuo individuo)
{
    Individuo novo;

    novo.dimensao = individuo.dimensao;
    novo.fitness = individuo.fitness;
    novo.genes = malloc(sizeof(*novo.genes) * novo.dimensao);
    MEMCHECK(novo.genes);

    for (int i = 0; i < novo.dimensao; i++) {
        novo.genes[i] = individuo.genes[i];
    }

    return novo;
}

void free_individuo(Individuo individuo)
{
    free(individuo.genes);
}

Populacao populacao_vazia(int tam_populacao, double min, double max)
{
    Populacao pop;

    pop.tam_populacao = tam_populacao;
    pop.limite_inferior = min;
    pop.limite_superior = max;

    pop.individuos = malloc(pop.tam_populacao * sizeof(Individuo));
    MEMCHECK(pop.individuos);

    return pop;
}

Populacao inicializa_populacao(int tam_populacao, int dimensao, double min, double max)
{
    Populacao pop;

    pop = populacao_vazia(tam_populacao, min, max);

    for (int i = 0; i < pop.tam_populacao; i++) {
        pop.individuos[i] = individuo_aleatorio(dimensao, pop.limite_inferior, pop.limite_superior);
    }

    ordena_populacao_por_fitness(pop);

    return pop;
}

void integra_filhos(Populacao *pop_principal, Populacao pop_absorvida)
{
    Individuo *pop_final = malloc(sizeof(*pop_principal->individuos) * pop_principal->tam_populacao);
    MEMCHECK(pop_final);

    int it_principal = 0;
    int it_absorvida = 0;
    int num_selecionados = 0;

    // Presume-se que pop_principal já esteja ordenada.
    ordena_populacao_por_fitness(pop_absorvida);

    while (num_selecionados < pop_principal->tam_populacao && it_absorvida < pop_absorvida.tam_populacao) {
        double fitness_principal = pop_principal->individuos[it_principal].fitness;
        double fitness_absorvida = pop_absorvida.individuos[it_absorvida].fitness;

        if (fitness_principal < fitness_absorvida) {
            pop_final[num_selecionados] = copia_individuo(pop_principal->individuos[it_principal]);
            it_principal++;
        }
        else {
            pop_final[num_selecionados] = copia_individuo(pop_absorvida.individuos[it_absorvida]);
            it_absorvida++;
        }

        num_selecionados++;
    }

    if (num_selecionados < pop_principal->tam_populacao) {
        for (int i = num_selecionados; i < pop_principal->tam_populacao; i++) {
            pop_final[i] = copia_individuo(pop_principal->individuos[i]);
        }
    }

    free_populacao(*pop_principal);
    pop_principal->individuos = pop_final;
    
    return;
}

void ordena_populacao_por_fitness(Populacao pop)
{
    quick_sort_pop(pop.individuos, 0, pop.tam_populacao - 1);
}

void quick_sort_pop(Individuo *vetor, int lo, int hi)
{    
    int i = lo;
    int j = hi;

    if (lo >= hi) { // Caso base, vetor com 1 elemento ou menos
        return;
    }

    int pivo = lo + rand() % (hi - lo + 1); // indice aleatório entre lo e hi
    double valor_pivo = vetor[pivo].fitness;

    while (1) {
        while (vetor[i].fitness < valor_pivo)
            i++;
         
        while (vetor[j].fitness > valor_pivo)
            j--;
        
        /* indices antes de j (incluso) têm fitness <= valor_pivo,
        * e indices depois de i (incluso) tem fitness >= valor pivo
        * i.e., j é o último indice da partição esquerda e i o primeiro 
        * da partição direita */
        if (i >= j)
            break;
            
        troca(vetor, i, j);
        i++;
        j--;
    }

    quick_sort_pop(vetor, lo, j);
    quick_sort_pop(vetor, j + 1, hi);
}

Individuo* amostra_uniforme(Populacao pop, int N) {
    Individuo *copia = malloc(sizeof(Individuo) * pop.tam_populacao);
    MEMCHECK(copia);

    memcpy(copia, pop.individuos, sizeof(Individuo) * pop.tam_populacao);

    fisher_yates_shuffle(copia, pop.tam_populacao);

    Individuo *sample = malloc(sizeof(Individuo) * N);
    MEMCHECK(sample);

    for (int i = 0; i < N; i++) {
        sample[i] = copia[i];  // ou copie profundamente se necessário
    }

    free(copia);
    return sample;
}

void fisher_yates_shuffle(Individuo *vetor, int tam)
{
    for (int i = tam - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        troca(vetor, i, j);
    }
}

void troca(Individuo *vetor, int a, int b)
{
    Individuo tmp = vetor[a];
    vetor[a] = vetor[b];
    vetor[b] = tmp;
}

void free_populacao(Populacao pop)
{
    for (int i = 0; i < pop.tam_populacao; i++) {
        free_individuo(pop.individuos[i]);
    }

    free(pop.individuos);
}
