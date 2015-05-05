#include	<stdio.h>
#include	<wait.h>
#include 	<stdlib.h>
#include 	<time.h>
#include	<sys/types.h>
#include	<sys/timeb.h>
//#include </usr/include/bits/signum.h >
#include <signal.h>
#define BASE 50
#define subp 2
#define ESPERA 5


	
	typedef struct {
	    int linha;
	    int coluna;
	    int valor;
	} Elemento;
	
	typedef struct {
	    int valor;
		Elemento a;
		Elemento b;
	} ProdutoInt;

	typedef struct {
	   int **conteudo;
	   int colunas;
	   int linhas;
	   int prodint;
	} Matriz;

  
	Elemento setmax(Elemento max, int valor,int linha,int coluna){
		if(valor > max.valor) {
			max.valor=valor;
			max.linha=linha+1;
			max.coluna=coluna+1;
		}
		return max;
	}
	
	Elemento setmin(Elemento min, int valor,int linha,int coluna){
		if(valor < min.valor) {
			min.valor=valor;
			min.linha=linha+1;
			min.coluna=coluna+1;
		}
		return min;
	}
	
	int x,j=0,t;
	
	Matriz matriz;
	
	void preenche(Matriz matriz){
	  
	   int i,j,temp;
	   srand(time(NULL));
		for (j=0;j<matriz.linhas;j++){
			for (i=0;i<matriz.colunas;i++) {
				matriz.conteudo[i][j]=rand()%BASE;
				temp=rand()%2;
				if (temp ==0) matriz.conteudo[i][j] *= (-1);
			}
		}
	  }

      void *PI( void *temp){
	  int t = (int)temp;
	  
	  int i;
	  for (i=0;i<matriz.colunas;i++) {
		  x=x+((matriz.conteudo[i][t] )* (matriz.conteudo[t][i]) );
		  matriz.prodint=x;

		  
	  }
	  

	}

   
   
   
int main(void){
		struct timeb start, stop;
		double elapsed;
		int i=0,j=0,temp;
		Elemento max,min,atual;
		//Matriz matriz;
		int id, status;
		int prodint;
		
		int rc;
		
		START:
		
		matriz.prodint=0;
		x=0;
		
		max.valor=(-1)*(BASE*2); //max que sempre sera substituido, no exemplo a base eh 50 e o max inicial é -100
		min.valor=BASE*2; //min q sempre sera substituido
		

		//matriz.colunas
		printf("Digite o número de matriz.colunas: ");
		scanf("%d",&matriz.colunas);
		if (matriz.colunas==0) return -1;
		matriz.conteudo = (int **)malloc(matriz.colunas*sizeof(int *));

		
		//linhas
		printf("Digite o número de linhas: ");
		scanf("%d",&matriz.linhas);
		if (matriz.linhas==0) return -2;
		for (i=0;i<matriz.colunas;i++) matriz.conteudo[i] = (int *)malloc(matriz.linhas*sizeof(int));
		
		ftime(&start);
		
		//PREENCHE MATRIZ 
		preenche(matriz);
		
		
		pthread_t threads[matriz.linhas]; 
		//SUBPROCESSOS pesquisam produto interno
		
		for(t=0; t<matriz.linhas; t++){ 
			
			rc = pthread_create(&threads[t], NULL, PI,(void *)t);
			printf("In main: criando thread numero %d\n", t); 
			if (rc) { 
			  printf("ERROR code is %d\n", rc); 
			  exit(-1); 
			} 
		} 
		
	
		
		for (t=0; t<matriz.linhas; t++) {
		  if (pthread_join(threads[t], NULL)) {
		      printf("---------------ERRO: pthread_join() ERRO NA THREAD=%i\n",t); exit(-1); 
		  } 
		} 

		

		//imprimindo a matriz
		
			for (j=0;j<matriz.linhas;j++) for (i=0;i<matriz.colunas;i++) max=setmax(max,matriz.conteudo[i][j],j,i);
			for (j=0;j<matriz.linhas;j++) for (i=0;i<matriz.colunas;i++) min=setmin(min,matriz.conteudo[i][j],j,i); //poderia ter juntadao os dois
		  
			printf ("\nA matriz:\n\n");
			for (j=0;j<matriz.linhas;j++) {
				for (i=0;i<matriz.colunas;i++) {
				  printf ("%4i ",matriz.conteudo[i][j]);
				}
				printf ("\n");
			}
			

			
		
			//print max min tempo
			printf("\nMaximo =%i \t %iªlinha  %iªcoluna\n", max.valor, max.linha, max.coluna);
			printf("Minimo =%i \t %iªlinha  %iªcoluna\n", min.valor, min.linha, min.coluna);
			printf("\nO produto interno é : %i\n",matriz.prodint);
				      
			
			ftime(&stop);
			elapsed=((double) stop.time + ((double) stop.millitm * 0.001)) -((double) start.time + ((double) start.millitm * 0.001));
			printf(" O tempo total foi de  %7.3f \n", elapsed);
			
			goto START;
			

		
		
	    pthread_exit(NULL);
}