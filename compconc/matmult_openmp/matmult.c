#include <stdio.h>
#include <omp.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include "./timer.h"

void matPrint(int* m, int size);
void matMult(int* m1, int* m2, int* result, int size);
void matRead(int* m, int size);
void ompMatMult(int* m1, int* m2, int* result, int size, int nthreads);

int main(int argc, char *argv[])
{
    int nthreads, matrixSize;
    double start, finish;

    if (argc > 1) 
    {
	nthreads = atoi(argv[1]);
    }
    else 
    {
	puts("Número de argumentos de linha de comando insuficientes.");
    }
    
    scanf("%d", &matrixSize);
    
    // Alloc
    int *A = (int *) malloc(sizeof(int) * matrixSize * matrixSize);
    int *B = (int *) malloc(sizeof(int) * matrixSize * matrixSize);
    int *C = (int *) malloc(sizeof(int) * matrixSize * matrixSize);
    
    // Read
    matRead(A, matrixSize);
    matRead(B, matrixSize);
    
    // Print
    //matPrint(A, matrixSize);
    //matPrint(B, matrixSize);
    
    // Compute (single-threaded)
    GET_TIME(start);
    matMult(A, B, C, matrixSize);
    GET_TIME(finish);
    printf("time (single-threaded): %f\n", finish - start);
    
    // Print
    matPrint(C, matrixSize);
    
    // Compute (multi-threaded)
    memset(C, 0, sizeof(int) * matrixSize * matrixSize);
    GET_TIME(start);
    ompMatMult(A, B, C, matrixSize, nthreads);
    GET_TIME(finish);
    printf("time (multi-threaded): %f\n", finish - start);
    
    // Print
    matPrint(C, matrixSize);
    return 0;
}

void matMult(int* m1, int* m2, int* result, int size)
{
    int i, j, x;
    
    for (i = 0; i < size; i++)
    {
	for (j = 0; j < size; j++)
	{
	    int accumulator = 0;
	    for (x = 0; x < size; x++)
	    {
		accumulator += m1[i * size + x] * m2[x * size + j];
	    }
	    result[i * size + j] = accumulator;
	}
    }
}

// OpenMP
void ompMatMult(int* m1, int* m2, int* result, int size, int nthreads)
{
    int i, j, x;
    
    #pragma omp parallel for num_threads(nthreads) private (i, j, x)
    for (i = 0; i < size; i++)
    {
	for (j = 0; j < size; j++)
	{
	    int accumulator = 0;
	    for (x = 0; x < size; x++)
	    {
		accumulator += m1[i * size + x] * m2[x * size + j];
	    }
	    result[i * size + j] = accumulator;
	}
    }   
}

void matPrint(int* m, int size)
{
#ifdef PRINT
    int i, j;
    
    for (i = 0; i < size; i++)
    {
	for (j = 0; j < size; j++)
	{
	    printf("%3d ", m[i * size + j]);
	}
	puts("");
    }
    puts("");
#endif
}

void matRead(int* m, int size)
{
    int i, j;
    
    for (i = 0; i < size; i++)
    {
	for (j = 0; j < size; j++)
	{
	    scanf("%d", &m[i * size + j]);
	}
	//getchar();
    }
}
