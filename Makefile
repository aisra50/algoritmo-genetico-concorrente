bibliotecas_proprias := algoritmo-genetico.c populacao.c variaveis-aleatorias.c
flags := -Wall -Wextra

sequencial: main.c $(bibliotecas_proprias)
	@echo compilando versão sequencial...
	@gcc -o sequencial main.c $(bibliotecas_proprias) -lm $(flags)
	@echo Feito!

concorrente: main.c $(bibliotecas_proprias)
	@echo compilando versão concorrente...
	@gcc -DCONCORRENTE -o concorrente main.c $(bibliotecas_proprias) -lm -lpthread $(flags)
	@echo Feito!

tempos: tempos.c $(bibliotecas_proprias)
	@echo compilando medidas de tempo...
	@gcc -o tempos tempos.c $(bibliotecas_proprias) -lm -lpthread $(flags)
	@echo Feito!

historico_seq: main.c $(bibliotecas_proprias)
	@echo compilando histórico sequencial...
	@gcc -DHISTORICO -o historico_seq main.c $(bibliotecas_proprias) -lm $(flags)
	@echo Feito!

historico_conc: main.c $(bibliotecas_proprias)
	@echo compilando histórico concorrente...
	@gcc -DCONCORRENTE -DHISTORICO -o historico_conc main.c $(bibliotecas_proprias) -lm -lpthread $(flags)
	@echo Feito!

.PHONY: clean
clean:
	@echo limpando...
	@rm -f sequencial tempos concorrente historico_conc historico_seq