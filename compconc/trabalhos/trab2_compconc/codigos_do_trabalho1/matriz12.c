#include	<stdio.h>
#include	<wait.h>
#include 	<stdlib.h>
#include 	<time.h>
#include	<sys/timeb.h>
#include	<sys/types.h>
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
	
	
	void preenche(int **matriz,int linhas,int colunas){
	   int i,j,temp;
	   srand(time(NULL));
		for (j=0;j<linhas;j++){
			for (i=0;i<colunas;i++) {
				matriz[i][j]=rand()%BASE;
				temp=rand()%2;
				if (temp ==0) matriz[i][j] *= (-1);
			}
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
					  if(id2==0) {
					      x=0;
					      for (i=0;i<colunas;i++) {
						x=x+(matriz[i][j]*matriz[j][i]);
						
					      }
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
		
		int **matriz;
		int i=0,j=0,temp,x;
		int colunas,linhas;
		Elemento max,min,atual;
		
		int id, status;
		FILE *arq;
		int prodint;
		
		struct timeb start, stop;
		double elapsed;
		
		
		START:
		
		
		max.valor=(-1)*(BASE*2); //max que sempre sera substituido, no exemplo a base eh 50 e o max inicial é -100
		min.valor=BASE*2; //min q sempre sera substituido


		//colunas
		printf("Digite o número de colunas: ");
		scanf("%d",&colunas);
		if (colunas==0) return -1;
		matriz = (int **)malloc(colunas*sizeof(int *));

		
		//linhas
		printf("Digite o número de linhas: ");
		scanf("%d",&linhas);
		if (linhas==0) return -2;
		for (i=0;i<colunas;i++) matriz[i] = (int *)malloc(linhas*sizeof(int));
		
		ftime(&start);

		//PREENCHE MATRIZ 
		preenche(matriz,linhas,colunas);
		
		
		
		//SUBPROCESSOS pesquisam produto interno
		prodint=PI(matriz,linhas,colunas);


		//PAI imprimindo a matriz
		if(id!=0){
			for (j=0;j<linhas;j++) for (i=0;i<colunas;i++) max=setmax(max,matriz[i][j],j,i);
			for (j=0;j<linhas;j++) for (i=0;i<colunas;i++) min=setmin(min,matriz[i][j],j,i); //poderia ter juntadao os dois
		  
			printf ("\nA matriz:\n\n");
			for (j=0;j<linhas;j++) {
				for (i=0;i<colunas;i++) {
				  printf ("%4i ",matriz[i][j]);
				}
				printf ("\n");
			}
			
			
			
			arq=fopen("dados.txt","r");
			if (arq == NULL) {
				printf ("Houve um erro ao abrir o arquivo.\n");
				return 1;
			}
			prodint=0;
			for (j=0;j<linhas;j++) {
			  fscanf(arq,"%i", &x);
			  prodint=prodint+x;
			}
			fclose (arq);
			
			puts("Dados recebidos.aNALISANDO . . .");
			
			
			//produto interno
	
		
		
			//print max min tempo
			printf("\nMaximo =%i \t %iªlinha  %iªcoluna\n", max.valor, max.linha, max.coluna);
			printf("Minimo =%i \t %iªlinha  %iªcoluna\n", min.valor, min.linha, min.coluna);
			printf("\nO produto interno é : %i\n",prodint);
			ftime(&stop);
			
			elapsed=((double) stop.time + ((double) stop.millitm * 0.001)) - ((double) start.time + ((double) start.millitm * 0.001));

			printf("  O tempo de execucao e de %7.3f \n", elapsed);
			
			//goto START;
		}
		
		
    //exit(0);
	return 0;
}
