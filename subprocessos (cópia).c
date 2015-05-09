#include	<stdio.h>
#include	<wait.h>
#include 	<stdlib.h>
#include 	<time.h>
#include	<sys/timeb.h>
#include	<sys/types.h>
#include 	<signal.h>
#define BASE 100
	
	//FUNCOES
	void preenche(int **matriz,int semente);
	void imprime (int **matriz);
	void multiplica(int **mat1,int **mat2);
	void gerasite(int **matrizA,int **matrizB);
   
	//PRE-DEFINIDO
	int dimensao;
	int pai;
	
	//cria matriz de saida
	int **matrizResultado;	
   
   
int main(void){
	
		int **matrizA;
		int **matrizB;
		int i,j;
		

		printf("\nQual a dimensao N (NxN) de suas matrizes?\n");
		scanf("%i",&dimensao); 
		//dimensao=2;
		printf("\nOK, dimensao = %i\n",dimensao);
		
		//PAI
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
		
		

		
		
		//SUBPROCESSOS CALCULAM A MULTIPLICACAO
		multiplica(matrizA,matrizB);
		
		
		//imprime 
		printf ("\nMatriz A:");
		imprime(matrizA);
		printf ("\nMatriz B:");
		imprime(matrizB);
		printf ("\nMatriz A*B:");
		imprime(matrizResultado);
		
		//gera link
		gerasite(matrizA,matrizB);
			
		
		printf("=============================================== \n");

		
    exit(0);
	return 0;
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
	
	/*
	void multiplica(int **mat1,int **mat2){
		//var
		int acumula=0;
		int linha,coluna,k,i,j;
		int pid=getpid();
		int pid=1;
		int countsp=0; //conta subprocessos
		
		//var tempo
		struct timeb start, stop;
		double elapsed;

		
		//contagem do tempo
		ftime(&start);
			
		
		//calcula
		for (linha=0;linha<dimensao;linha++){
			
			for (coluna=0;coluna<dimensao;coluna++){
				
				if (pid!=0){ //cria filhos
					countsp++;
					pid=fork();
				}
				if (pid==0){ //filhos continuam
				
						matrizResultado[linha][coluna]=0;
						//acumula=0; 
						
						
							for (k=0;k<dimensao;k++){
									matrizResultado[linha][coluna]+=mat1[k][coluna]*mat2[linha][k];
									
							} 
							acumula=matrizResultado[linha][coluna];
						
							printf("\n--M=%i  SP-> %i",acumula, getpid());
							//matrizResultado[linha][coluna]=acumula;
						
				}

				

			}
			
		}
		

		if (pid!=0) for (k=1;k<countsp;k++) wait(NULL);
		else kill(getpid(), SIGKILL);
		
		
		
		ftime(&stop);
		

		printf ("\nResultado:\n\n");
		imprime(matrizResultado);
		
		elapsed=((double) stop.time + ((double) stop.millitm * 0.001)) - ((double) start.time + ((double) start.millitm * 0.001));
		printf("\n -> O tempo de execucao e de %.3lf \n", elapsed);
		
	}*/
	
	void multiplica(int **mat1,int **mat2){
			int sp,pid,p,lin,col,k,status,x=0;
			
			//criar sp
			sp=-1;
			pid=getpid();
			for(p=0;p<dimensao*dimensao;p++){
					if (pid!=0) {
							sp++;							
							pid=fork();
						}
					else break;
			}
			x++;							
			printf("\n\t\t x=%i PID=%i",x,pid);
							 

			//calcula
			if (pid==0) {
					lin=(int)(sp/dimensao);
					col=sp%dimensao;
					
					matrizResultado[lin][col]=0;
					for (k=0;k<dimensao;k++){
							matrizResultado[lin][col]+=mat1[k][col]*mat2[lin][k];
					}
					
			}
			//mata sp
			for(p=dimensao*dimensao;p>0;p--){
					if (pid!=0) {
							sp--;
							wait(&status);
						}
					else kill(getpid(), SIGKILL);
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
   
