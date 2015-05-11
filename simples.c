//Leonardo Neves da Silva DRE110155777 
//Luan Cerqueira Martins  DRE111211704
//T1 SO 2015.1 ProfValeria

#include	<stdio.h>
#include 	<stdlib.h>
#include 	<time.h>
#include	<sys/timeb.h>

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
void gerasite(int **matrizA,int **matrizB);
void preenche(int **matriz,int semente);
void imprime (int **matriz);
void calcula(int t);
void multiplica(int **mat1,int **mat2);


//////////////////////////////////////////////////////
	
int main( int argc, char *argv[ ] ){
		
		//variaveis de tempo
		struct timeb start, stop;
		double elapsed;


		int i;
		
		if (argc > 1 ) {
			dimensao = atoi(argv[1]);
		}
		else {
			printf("\nQual a dimensao N (NxN) de suas matrizes?\n");
			scanf("%i",&dimensao);
		}

		//Cria matrizes 
		if (dimensao<=0 ) return -1;
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
		
		printf("\n[ProcessoUnico] Matrizes = %i x %i -> O tempo de execucao e de %.3lf\n", dimensao, dimensao,elapsed);			

		//exit(0);
		return 0;
}

//////////////////////////////////////////////////////


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

	
void calcula(int t){
		
		int linha,coluna,acumula,k;
		linha=t;
		for (coluna=0;coluna<dimensao;coluna++){
			acumula=0;
			for (k=0;k<dimensao;k++){
						acumula=acumula+matrizA[k][coluna]*matrizB[linha][k]; 
			} 
			matrizResultado[linha][coluna]=acumula;
		}
}

void multiplica(int **mat1,int **mat2){
	
		int i,rc,acumula;
		pthread_t th[dimensao];
		
		for (i=0;i<dimensao;i++){
			calcula(i);
		}
}
