#include <stdio.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
 
 
#define DIM 4
#define NUM_OF_FORK_CALLS 2
#define NUM_OF_PROC 4
 
int main()
{
    pid_t pid;
    int segment_id;
    int *partition;
    *partition = 0;
    int i;
    int j;
    int matrixA[DIM][DIM];
    int matrixB[DIM][DIM];
    int matrixC[DIM][DIM];
 
    //allocating a shared memory segment
    segment_id = shmget(IPC_PRIVATE, sizeof(int)*(DIM*DIM)*3, IPC_CREAT|0666);
 
    //attach the shared memory segment to the partition variable
    partition = (int *) shmat(segment_id, NULL,0);
 
    //fill the matrices
    for(i = 0; i < DIM; i++)
    {
    for(j = 0; j < DIM; j++)
    {
        matrixA[i][j] = i + j;
        matrixB[i][j] = i + 3;
    }
    }
 
    for(i = 0; i < NUM_OF_FORK_CALLS; i++)
    {
    pid = fork();
    }
 
 
    //error occurred
    if(pid < 0)
    {
        fprintf(stderr, "Fork Failed");
        return 1;
    }
 
    //child process
    else if(pid==0)
    {
 
        int start = (*partition * DIM)/NUM_OF_PROC;
        /*The start position will tell which row to start calculating */
 
        int end = ((*partition+1) * DIM)/NUM_OF_PROC;
        /*The end position wil tell which row to stop calculating*/
 
        int i;
        int j;
        int k;
 
         //These for loops will calculate a partition of C
        for(i = start; i<end; i++)
        {
            for(j = 0; j < DIM; j++)
            {
                matrixC[i][j] = 0;
 
                for(k = 0; k < DIM; k++)
                {
                     matrixC[i][j] += matrixA[i][k] * matrixB[k][j];
                }
             }
        }
 
    }
 
    //parent process
    else
    {
 
        wait(NULL);
 
        int i;
        int j;
 
        //print matrix A
        for(i = 0; i < DIM; i++)
        {
            for(j = 0; j<DIM; j++) 
            {
                printf("%d ",matrixA[i][j]);  
            }
 
            printf("\n");  
        }
 
        printf("-----------\n\n");
 
        //print matrix B
        for(i = 0; i < DIM; i++)
        {
            for(j = 0; j<DIM; j++)  
            {
                printf("%d ",matrixB[i][j]);  
            }
 
            printf("\n");  
        }
 
         printf("-----------\n\n");
 
        //print matrix C
        for(i = 0; i < DIM; i++)
        {
            for(j = 0; j<DIM; j++) 
            {
                printf("%d ",matrixC[i][j]);  
            }
 
            printf("\n"); 
        }
         printf("-----------\n\n");
 
    }
 
    return 0;
}
