#include<stdio.h>
#define MEMCHECK(X) if ((X) == NULL) {fprintf(stderr, "ERRO: Erro ao alocar memória\n"); exit(1);}

typedef struct {
    int dimensao;
    double fitness;
    double *genes;
} Individuo;

typedef struct {
    double limite_superior;
    double limite_inferior;

    Individuo *individuos;
    int tam_populacao;
} Populacao;


// Funções auxiliares
Individuo inicializa_individuo(int dimensao); // As coordenadas serão valores lixo
Individuo individuo_aleatorio(int dimensao, double min, double max);
void print_individuo(Individuo);
Individuo copia_individuo(Individuo individuo);
void free_individuo(Individuo individuo);
Populacao populacao_vazia(int tam_populacao, int dimensao, double min, double max);
void free_populacao(Populacao pop);
void quick_sort_pop(Individuo *vetor, int lo, int hi);
void troca(Individuo *vetor, int a, int b);

// Parte da estrutura geral do algoritmo
Populacao inicializa_populacao(int tam_populacao, int dimensao, double min, double max); // Gera tam_populacao individuos com genes distribuidos uniformemente entre min e max
void integra_filhos(Populacao pop_principal, Populacao pop_absorvida);
void computa_fitnesses(Populacao pop);
void ordena_populacao_por_fitness(Populacao pop);