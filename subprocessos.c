#include	<stdio.h>
#include	<wait.h>
#include 	<stdlib.h>
#include 	<time.h>
#include	<sys/timeb.h>
#include	<sys/types.h>
#include 	<signal.h>
#define BASE 100
	
	typedef struct {
      int linha;
      int coluna;
	  int valor;
   } Elemento;
   
	//TAMANHO PRE-DEFINIDO
	int dimensao;
	int pai;
	int temp=0;
	
	//cria matriz de saida
	int **matrizResultado;
		
	
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
		int procs;
		
		
		//var tempo
		struct timeb start, stop;
		double elapsed;

		//pnt
		int *point;
		point=&temp;
		
		
		//contagem do tempo
		ftime(&start);
			
		
		//calcula
		for (linha=0;linha<dimensao;linha++){
			
			for (coluna=0;coluna<dimensao;coluna++){
				
				if (procs!=0){ //inicia filhos
					procs=fork();
				}
				if (procs==0){ //filhos continuam
				
						acumula=0; 
						
						for (k=0;k<dimensao;k++){
								acumula=acumula+mat1[k][coluna]*mat2[linha][k]; 
						} 
						matrizResultado[linha][coluna]=acumula;
						*point=acumula; 
						temp=acumula;
						
				}
			}
			
		}
		
		//se for pai para processos
		for (linha=0;linha<dimensao;linha++){
			for (coluna=0;coluna<dimensao;coluna++){
				if( getpid() == pai ) {
					wait(NULL);
					printf ("\n 'Ola' (%i) -> %i , %i\n",getpid(),*point,temp);
				}
				else {
					printf ("\n 'Oi pai' (%i)-> %i , %i\n",getpid(),*point,temp);
					kill(getpid(), SIGKILL);
				}
			}
		}
		ftime(&stop);
		

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
		pai=getpid();
		printf ("\nO processo pai se chama: %i\n",pai);


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
		gerasite(matrizA,matrizB);
			
		
		printf("\n =============================================== \n");

		
    //exit(0);
	return 0;
}


