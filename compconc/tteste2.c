#include <linux/unistd.h>
#include <sys/syscall.h>
#include <unistd.h>

#include	<stdio.h>
#include	<wait.h>
#include 	<stdlib.h>
#include 	<time.h>
#include	<sys/types.h>
#include	<sys/timeb.h>
//#include </usr/include/bits/signum.h >
#include <signal.h>
#define BASE 100

//parametros
typedef struct  {
   int **mat1;
   int **mat2;
   int coluna;
   int linha;
   int thread;
} Calculos ;


//PRE-DEFINIDOS
int dimensao;
int pai;
int temp=0;


//cria matrizes
int **matrizA;
int **matrizB;
int **matrizResultado;

//funcoes
pid_t gettid( void ) ;
void gerasite(int **matrizA,int **matrizB);
void preenche(int **matriz,int semente);
void imprime (int **matriz);
void* calcula(void* arg);
void multiplica(int **mat1,int **mat2);


//////////////////////////////////////////////////////
	
int main(void){
		
		//variaveis de tempo
		struct timeb start, stop;
		double elapsed;


		int i;
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
		
		//THREADS CALCULAM A MULTIPLICACAO
		ftime(&start);					//inicia timer
		multiplica(matrizA,matrizB);
		ftime(&stop);					//para timer
		elapsed=((double) stop.time + ((double) stop.millitm * 0.001)) - ((double) start.time + ((double) start.millitm * 0.001));
		
		//IMPRIME
		printf ("\nMatriz A:");
		imprime(matrizA);
		printf ("\nMatriz B:");
		imprime(matrizB);
		printf ("\n-Resultado:");
		imprime(matrizResultado);
		
		//gera link
		gerasite(matrizA,matrizB);
		
		printf("\n -> O tempo de execucao e de %.3lf \n", elapsed);

			

		printf("\n =============================================== \n");

		//exit(0);
		return 0;
}

//////////////////////////////////////////////////////


pid_t gettid( void ) { 
		return syscall( __NR_gettid );
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

	
void* calcula(void* arg){
		//Calculos* args = (Calculos*) arg;
		int t = (int)arg;
		int linha,coluna,acumula,k;
		linha=(int)(t/dimensao);
		coluna=t%dimensao;
		acumula=0;
		for (k=0;k<dimensao;k++){
								acumula=acumula+matrizA[k][coluna]*matrizB[linha][k]; 
		} 
		matrizResultado[linha][coluna]=acumula;
		
		printf("\n ## Thread=%i TID=%i Calculou=%i na posicao [%i,%i]",t,(int)gettid(),acumula,linha,coluna);

}

void multiplica(int **mat1,int **mat2){
	
		int i,rc,acumula;
		int dimensao2=dimensao*dimensao;
		int t=2;
		pthread_t th[dimensao2];
		
		for (i=0;i<dimensao2;i++){
			rc = pthread_create(&th[i], NULL, calcula,(void*)i);
			if (rc) { printf("ERROR code is %d\n", rc); exit(-1); }
		}
		
		for (i=0;i<dimensao2;i++){
			if (pthread_join(th[i], NULL)) {
				  printf("---------------ERRO: pthread_join() ERRO NA THREAD=%i\n",i); exit(-1); 
			  } 
		}
		

}
