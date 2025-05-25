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
double uniforme(double a, double b); // Variável aleatória com distribuição uniforme entre a e b
Individuo individuo_aleatorio(int dimensao, double min, double max);
void print_individuo(Individuo);

// Parte da estrutura geral do algoritmo
Populacao inicializa_populacao(int tam_populacao, int dimensao, double min, double max); // Gera tam_populacao individuos com genes distribuidos uniformemente entre min e max
void computa_fitnesses(Populacao pop);