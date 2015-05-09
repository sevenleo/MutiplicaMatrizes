all: 
	@echo ____________________________________________________________________Cria
	clear all
	@echo ____________________________________________________________________compila
	@gcc simples.c -o simples 
	@gcc subprocessos.c -o subprocessos 
	@gcc threads.c -o threads -lpthread
	@clear
	@echo ____________________________________________________________________Executa
	@echo Execute ./subprocessos ou./threads ou ./simples
	

clean: 
	rm a.out 


	
