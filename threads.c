#include	<stdio.h>
#include	<wait.h>
#include 	<stdlib.h>
#include 	<time.h>
#include	<sys/types.h>
#include	<sys/timeb.h>
//#include </usr/include/bits/signum.h >
#include <signal.h>
#define BASE 100

   	typedef struct {
	   int **mat1;
	   int **mat2;
	   int **matrizResultado;
	   int coluna;
	   int linha;
	   int thread;
	} Calculos;
	
	//TAMANHO PRE-DEFINIDO
	int dimensao;
	int pai;
	int temp=0;
	
	//cria matriz de saida
	int **matrizResultado;
	
	void calcula(Calculos c){
			int acumula,k;
	
			acumula=0; 
			/*
			for (k=0;k<dimensao;k++){
				acumula=acumula+c.mat1[k][c.coluna]*c.mat2[c.linha][k]; 	 
			} 
			c.matrizResultado[c.linha][c.coluna]=acumula;*/
			printf("\nSou a t= %i e achei o valor %i",c.thread,acumula);
			 
			
	}
		
	
	void preenche(int **matriz,int semente){
	   int i,j,temp;
	    srand(semente);
	    
		for (j=0;j<dimensao;j++){
			for (i=0;i<dimensao;i++) {
				matriz[i][j]=rand()%BASE; 
									
				temp=rand()%2;
				if (temp ==0) matriz[i][j] *= (-1);
			}
		}
   }


	void imprime (int **matriz){
		int i,j;
		printf ("\n");
		for (j=0;j<dimensao;j++) {
			for (i=0;i<dimensao;i++) {
			  printf ("%8i ",matriz[i][j]);
			}
			printf ("\n");
		}
		
		
	}
	
	
	void multiplica(int **mat1,int **mat2){
		//var
		int acumula=0;
		int linha,coluna,k,i,j;
		int rc,t;
		Calculos args;
		
		
		//var tempo
		struct timeb start, stop;
		double elapsed;

		
		
		
			
		 //calcula
		pthread_t threads[dimensao*dimensao]; 
		args.mat1=mat1;
		args.mat2=mat2;
		args.matrizResultado=matrizResultado;
		t=0;
		ftime(&start); //inicia contagem do tempo
		for (linha=0;linha<dimensao;linha++){
			args.linha=linha;
			for (coluna=0;coluna<dimensao;coluna++){
					args.coluna=coluna;
					args.thread=t;
					rc = pthread_create(&threads[t], NULL, calcula,args);
					if (rc) { printf("ERROR code is %d\n", rc); exit(-1); }
					t++;
						
			}
			
		}
		
		//recebe as threads
		t=0;
		for (linha=0;linha<dimensao;linha++){
			for (coluna=0;coluna<dimensao;coluna++){
				pthread_join(threads[t], NULL);
				t++;
			}
		}
		

		
		ftime(&stop); //finaliza contagem do tempo
		

		printf ("\nResultado:\n\n");
		imprime(matrizResultado);
		
		elapsed=((double) stop.time + ((double) stop.millitm * 0.001)) - ((double) start.time + ((double) start.millitm * 0.001));
		printf("\n -> O tempo de execucao e de %.3lf \n", elapsed);
		
	}
	
	
	void gerasite(int **matrizA,int **matrizB){
			int i,j;
			
			printf ("\n  google-chrome ' http://www.wolframalpha.com/input/?i=");
			printf ("{");
			for (j=0;j<dimensao;j++) {
				printf ("{");
				for (i=0;i<dimensao;i++) {
					if (i==0) printf ("%i",matrizA[i][j]);
					else printf (",%i",matrizA[i][j]);
					
				}
				if (j+1==dimensao) printf ("}");
				else printf ("},");
			}
			printf ("}");
			printf (" * ");
			printf ("{");
			for (j=0;j<dimensao;j++) {
				printf ("{");
				for (i=0;i<dimensao;i++) {
					if (i==0) printf ("%i",matrizB[i][j]);
					else printf (",%i",matrizB[i][j]);
					
				}
				if (j+1==dimensao) printf ("}");
				else printf ("},");
			}
			printf ("}'\n");
		}
   
   
   
int main(void){
	
		int **matrizA;
		int **matrizB;
		int i,j;
		

		FILE *arq;

		
		printf("\nQual a dimensao N (NxN) de suas matrizes?\n");
		scanf("%i",&dimensao);
		printf("\nOK, dimensao = %i\n",dimensao);




		//Cria matrizes 
		
		if (dimensao==0 || dimensao==0 ) return -1;
		matrizA = (int **)malloc(dimensao*sizeof(int *));
		matrizB = (int **)malloc(dimensao*sizeof(int *));
		matrizResultado = (int **)malloc(dimensao*sizeof(int *));
		for (i=0;i<dimensao;i++) matrizA[i] = (int *)malloc(dimensao*sizeof(int));
		for (i=0;i<dimensao;i++) matrizB[i] = (int *)malloc(dimensao*sizeof(int));
		for (i=0;i<dimensao;i++) matrizResultado[i] = (int *)malloc(dimensao*sizeof(int));


		//PREENCHE MATRIZ 
		preenche(matrizA,time(NULL));
		preenche(matrizB,time(NULL)+1);
		
		/*
		printf ("\nMatriz A:");
		imprime(matrizA);
		printf ("\nMatriz B:");
		imprime(matrizB);
		*/
		
		
		//SUBPROCESSOS CALCULAM A MULTIPLICACAO
		multiplica(matrizA,matrizB);
		
		//gera link
		//gerasite(matrizA,matrizB);
			
		
		printf("\n =============================================== \n");

		
    //exit(0);
	return 0;
}


