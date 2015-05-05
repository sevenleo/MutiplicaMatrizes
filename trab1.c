#include	<stdio.h>
#include	<wait.h>
#include 	<stdlib.h>
#include 	<time.h>
#include	<sys/timeb.h>
#include	<sys/types.h>
#include 	<signal.h>
#define BASE 50
#define subp 2
#define ESPERA 5
#define TESTE 2
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

  
	
	void preenche(int **matriz,int linhas,int colunas,int semente){
	   int i,j,temp;
	    srand(semente);
	    
		for (j=0;j<linhas;j++){
			for (i=0;i<colunas;i++) {
				matriz[i][j]=rand()%BASE;
				temp=rand()%2;
				if (temp ==0) matriz[i][j] *= (-1);
			}
		}
   }



	void versus(int **mat1,int **mat2,int NN){
		//var
		int acumula=0;
		int linha,coluna,k,i,j;
		
		//cria matriz de saida
		int colunas=TESTE,linhas=TESTE;
		int **mat3;
		mat3 = (int **)malloc(NN*sizeof(int *));
		for (i=0;i<NN;i++) mat3[i] = (int *)malloc(NN*sizeof(int));
		
		
		//calcula
		for (linha=0;linha<NN;linha++){
				for (coluna=0;coluna<NN;coluna++){
						acumula=0; 
						for (k=0;k<NN;k++){
								acumula=acumula+mat1[linha][k]*mat2[k][coluna]; 
						} 
						mat3[linha][coluna]=acumula; 
				} 
		}
		
		
		printf ("\nResultado:\n\n");
		for (j=0;j<linhas;j++) {
			for (i=0;i<colunas;i++) {
			  printf ("%4i ",mat3[i][j]);
			}
			printf ("\n");
		} 
		 
		
	}
	

     int PI(int **matriz,int linhas,int colunas){
		int x=0;
		int i,j,status2;
		int id2;
		int id3=getpid();
		FILE *arq2;
		
		int k=2;
		arq2 = fopen("dados.txt", "w+");
		fclose (arq2);
		for (j=0;j<linhas;j++){
				if(id3!=0)
				  id2=fork();
				  //printf("\n *esta parte esta sendo feita pelo processo: %i",getpid());
				  printf("\n *esta parte esta sendo feita pelo processo: %i",id2);
				  if(id2==0) {
					  x=0;
					  for (i=0;i<colunas;i++) x=x+(matriz[i][j]*matriz[j][i]);
					  id3 = 0;
					  arq2 = fopen("dados.txt", "a");
					  fprintf(arq2,"%i\n", x);
					  fclose (arq2);
				  }
				
				if (id3==0) break;
		}
				
		for (j=0;j<linhas;j++) wait(&status2);
		if( getpid() != id3 ) kill(getpid(), SIGKILL);
		
		
		
		return x;
      }

   
   
   
int main(void){
		
		int **matrizA;
		int **matrizB;
		int i=0,j=0,temp,x;
		int colunas,linhas;

		FILE *arq;
		
		struct timeb start, stop;
		double elapsed;
		
	
		printf ("----\nO processo pai se chama: %i\n----\n",getpid());

		START:
		
		//printf("Digite o número de colunas e linhas (N): ");
		
		//colunas
		colunas=TESTE;
		if (colunas==0) return -1;
		matrizA = (int **)malloc(colunas*sizeof(int *));
		matrizB = (int **)malloc(colunas*sizeof(int *));
		
		//linhas
		linhas=TESTE;
		for (i=0;i<colunas;i++) matrizA[i] = (int *)malloc(linhas*sizeof(int));
		for (i=0;i<colunas;i++) matrizB[i] = (int *)malloc(linhas*sizeof(int));
		
		
		

		//PREENCHE MATRIZ 
		preenche(matrizA,linhas,colunas,time(NULL));
		preenche(matrizB,linhas,colunas,time(NULL)+1);
		
		//PAI imprimindo a matriz
	
		printf ("\nA matriz:\n\n");
		for (j=0;j<linhas;j++) {
			for (i=0;i<colunas;i++) {
			  printf ("%4i ",matrizA[i][j]);
			}
			printf ("\n");
		}
		
		printf ("\nA matriz:\n\n");
		for (j=0;j<linhas;j++) {
			for (i=0;i<colunas;i++) {
			  printf ("%4i ",matrizB[i][j]);
			}
			printf ("\n");
		}
		
		//testarnosite
		printf ("\n   http://www.wolframalpha.com/input/?i=");
		printf ("{");
		for (j=0;j<linhas;j++) {
			printf ("{");
			for (i=0;i<colunas;i++) {
				if (i==0) printf ("%i",matrizA[i][j]);
				else printf (",%i",matrizA[i][j]);
				
			}
			if (j+1==linhas) printf ("}");
			else printf ("},");
		}
		printf ("}");
		printf (" * ");
		printf ("{");
		for (j=0;j<linhas;j++) {
			printf ("{");
			for (i=0;i<colunas;i++) {
				if (i==0) printf ("%i",matrizB[i][j]);
				else printf (",%i",matrizB[i][j]);
				
			}
			if (j+1==linhas) printf ("}");
			else printf ("},");
		}
		
		printf ("}\n");
		
		//SUBPROCESSOS CALCULAM A MULTIPLICACAO
		ftime(&start);
		versus(matrizA,matrizB,colunas);
		ftime(&stop);
		
		elapsed=((double) stop.time + ((double) stop.millitm * 0.001)) - ((double) start.time + ((double) start.millitm * 0.001));
	
		printf("\n -> O tempo de execucao e de %.3lf \n", elapsed);
		printf("\n =============================================== \n");
		system("cat dados.txt");
		
		//goto START;
		
		
		
    //exit(0);
	return 0;
}


