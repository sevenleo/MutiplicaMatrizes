#include	<stdio.h>
#include	<wait.h>
#include 	<stdlib.h>
#include 	<time.h>
#include	<sys/timeb.h>
#include	<sys/types.h>
#define BASE 50

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
	int j,i;
	for (j=0;j<linhas;j++){
			for (i=0;i<colunas;i++) {
			    x=x+(matriz[i][j]*matriz[j][i]);
			}
	}
	return x;
      }
   
 
   
int main(void){

		struct timeb start, stop;
		double elapsed;
		int **matriz;
		int i=0,j=0,temp;
		int bigppid=getpid();
		int colunas,linhas;
		Elemento max,min,atual;
		clock_t inicio, fim;
		int prodint;
		
		
		
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

		//time(&inicial);
		ftime(&start);
		
		//PREENCHE MATRIZ 
		preenche(matriz,linhas,colunas);
		
		
		//pesquisa max e min
		for (j=0;j<linhas;j++) for (i=0;i<colunas;i++) max=setmax(max,matriz[i][j],j,i);
		for (j=0;j<linhas;j++) for (i=0;i<colunas;i++) min=setmin(min,matriz[i][j],j,i);
		
		//produto interno
		prodint=PI(matriz,linhas,colunas);
		// imprimindo a matriz
		printf ("\nA matriz:\n\n");
		for (j=0;j<linhas;j++) {
			for (i=0;i<colunas;i++) {
			  printf ("%4i ",matriz[i][j]);
			}
			printf ("\n");
		}
		
		//print max min tempo
		printf("\nMaximo =%i \t %iªlinha  %iªcoluna\n", max.valor, max.linha, max.coluna);
		printf("Minimo =%i \t %iªlinha  %iªcoluna\n", min.valor, min.linha, min.coluna);
		printf("\nO produto interno é : %i\n",prodint);
		

	      ftime(&stop);
	      elapsed=((double) stop.time + ((double) stop.millitm * 0.001)) - ((double) start.time + ((double) start.millitm * 0.001));

	      printf("  O tempo de execucao e de %7.3f \n", elapsed);

		
		goto START;

    //exit(0);
	return 0;
}