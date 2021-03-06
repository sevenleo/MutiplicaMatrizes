//Leonardo Neves da Silva DRE110155777 
//Luan Cerqueira Martins  DRE111211704
//T1 SO 2015.1 ProfValeria

#include	<stdio.h>
#include 	<stdlib.h>
#include 	<time.h>
#include	<sys/timeb.h>
#include 	<sys/types.h>
#include <signal.h>
#define BASE 100
#define BUFFER 256
typedef struct {
      int linha;
      int coluna;
	  int valor;
	  int fd[2];
} Elemento;

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
void multiplica(int **mat1,int **mat2);

int fd[2];

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

        // Inicializa o Pipe
        if (pipe(fd)){                      
        	fprintf(stderr, "pipe error\n");
        	exit(-1);
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
		//ftime(&start);					//inicia timer
		multiplica(matrizA,matrizB);
		//ftime(&stop);					//para timer
		elapsed=((double) stop.time + ((double) stop.millitm * 0.001)) - ((double) start.time + ((double) start.millitm * 0.001));
	
		/*
		//IMPRIME
		printf ("\nMatriz A:");
		imprime(matrizA);
		printf ("\nMatriz B:");
		imprime(matrizB);*/
		printf ("\n-Resultado:");
		imprime(matrizResultado);
		
		//gera link
		gerasite(matrizA,matrizB);
		
		printf("\n[Subprocessos=%i] Matrizes = %i x %i -> O tempo de execucao e de %.3lf\n",dimensao, dimensao, dimensao,elapsed);

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
	


void multiplica(int **mat1,int **mat2){
	
		int linha,coluna,acumula,i,k;
		int dimensao2=dimensao*dimensao;
		int id=1;

		for (linha=0;linha<dimensao;linha++){
			if (id!=0){
				id=fork();
			}
			if (id==0){
					//fecha escrita para ler//causando starvation
					//close(fd[1]);                   
					//read(fd[0], &matrizResultado, sizeof (int **));
					
					for (coluna=0;coluna<dimensao;coluna++){
							acumula=0;
							for (k=0;k<dimensao;k++){
											acumula=acumula+mat1[k][coluna]*mat2[linha][k]; 
							} 
							matrizResultado[linha][coluna]=acumula;
							printf("\n ## PID=%i Encontrou=%i para a posicao [%i,%i]",(int)getpid(),acumula,linha,coluna);
							
					}
							
					//fecha leitura para escrever
					printf("\n ###### PID=%i :",(int)getpid()); //ptintar matriz obtida para avaliacao
					imprime(matrizResultado);
					close(fd[0]);
					write(fd[1], &matrizResultado, sizeof (int **) * dimensao * dimensao);				
					
					exit(0);

			}
			
		}
		
		if( id != 0 ) for (linha=0;linha<dimensao2;linha++) {
			wait();
		}
		else kill(getpid(), SIGKILL);
		
		//soh o pai acessa essa area de Codigo, os filhos ja retornaram ou morreram
		close(fd[1]);
		
		read(fd[0], &matrizResultado, sizeof (int **) * dimensao * dimensao);
				
}
