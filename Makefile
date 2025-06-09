sequencial: main.c algoritmo-genetico.c populacao.c variaveis-aleatorias.c
	gcc -o sequencial main.c algoritmo-genetico.c  populacao.c variaveis-aleatorias.c -lm

concorrente: main.c algoritmo-genetico.c populacao.c variaveis-aleatorias.c
	gcc -DCONCORRENTE -o concorrente main.c algoritmo-genetico.c populacao.c variaveis-aleatorias.c -lm -lpthread

tempos: tempos.c algoritmo-genetico.c populacao.c variaveis-aleatorias.c
	gcc -o tempos tempos.c algoritmo-genetico.c populacao.c variaveis-aleatorias.c -lm -lpthread

.PHONY: clean
clean:
	rm sequencial tempos concorrente