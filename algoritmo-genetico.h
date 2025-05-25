#include "populacao.h"

double rosenbrock(Individuo v); // Retorna a fitness do indivíduo
Individuo selecao_torneio(Populacao pop, int k); // Retorna um indivíduo da população usando o método de seleção de torneio, com torneios de tamanho k
Individuo recombinacao_blx_alpha(Individuo pai1, Individuo pai2, double alpha); // Gera um novo indivíduo 
void mutacao_gaussiana(Individuo individuo); 
