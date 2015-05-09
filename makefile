all: 
	@echo ____________________________________________________________________Cria
	clear all
	@echo ____________________________________________________________________compila
	@gcc simples.c -o simples 
	@gcc subprocessos.c -o subprocessos 
	@gcc threads.c -o threads -lpthread
	@echo ____________________________________________________________________Executa
	@./threads 

clean: 
	rm a.out 


	
