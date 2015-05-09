all: 
	@echo ____________________________________________________________________Cria
	clear all
	@echo ____________________________________________________________________compila
	@gcc simples.c -o simples 
	@gcc subprocessos.c -o subprocessos 
	@gcc threads.c -o threads -lpthread
	@clear
	@echo ____________________________________________________________________Executa
	@echo Execute ./subprocessos ou ./threads ou ./simples
	sh relatorio.sh > relatorio.txt
	gedit relatorio.txt

clean: 
	rm a.out 


	
