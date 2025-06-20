# Algoritmo genetico concorrente

## Estrutura
O arquivo `main.c` contém tanto a implementação sequencial quanto a concorrente do algoritmo, além da versão que gera um histórico das melhores fitness por geração. Uma macro de pré-processamento define qual versão será utilizada (observe os trechos `#ifdef CONCORRENTE` e `#ifdef HISTORICO` nos arquivos `main.c` e `algoritmo-genetico.c`).

O arquivo `tempos.c` implementa tomadas de tempo a cada etapa do algoritmo, de modo a identificar os trechos que tomam mais tempo da execução, e que portanto indicam possibilitar um maior ganho com paralelismo.

Os diretórios `hist_sequencial` e `hist_concorrente` contém arquivos csv referentes à execuções do algoritmo sequencial e concorrente, respectivamente. O nome do arquivo identifica os parâmetros, e cada linha corresponde a uma execução.

Cada um dos arquivos csv tem 30 linhas. Uma linha é composta por uma lista de números, que correspondem ao melhor fitness de cada geração, seguido pelo termo `tempo_execucao: <x>`, que mostra o tempo que essa execução levou. Com esses dados, podemos obter as curvas de convergência média e o tempo médio de execução para cada combinação de parâmetros.

## Uso
Para simplificar a compilação, fizemos um makefile que diferencia as versões sequencial, concorrente e de tomadas de tempo. Para executar a versão sequencial do algoritmo, faça:
```shell
make sequencial
./sequencial <tam_pop> <dimensao> <num_max_avaliacoes>
```

E para a versão concorrente:
```shell
make concorrente
./concorrente <tam_pop> <dimensao> <num_max_avaliacoes> <num_threads>
```

Similarmente para o programa que faz as tomadas de tempo:
```shell
make tempos
./tempos <tam_pop> <dimensao> <num_max_avaliacoes>
```

E para as versões que geram o histórico de melhores fitness por geração:
```shell
make historico_seq <tam_pop> <dimensao> <num_max_avaliacoes>
./historico_seq
```

```shell
make historico_conc
./historico_conc <tam_pop> <dimensao> <num_max_avaliacoes> <num_threads>
```