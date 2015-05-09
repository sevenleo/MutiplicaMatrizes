#include <stdio.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/stat.h>
 
#define M 2
 
void printm(int size, double *m, char *s)
{
    printf("%s \n\n", s);
    int i,j;
    for(i=0; i<size; i++)
    {
        for(j=0; j<size; j++)
        {
            printf("%g\t" ,*(m+M*i+j));
        }
        printf("\n\n");
    }
}
 
 
void populate (int size, double *a, double *b)
{
    int i,j;
    for(i=0; i<size; i++)
    {
        for(j=0; j<size; j++)
        {
            *(a+i*M+j) = 1+rand()%100;
            *(b+i*M+j) = 1+rand()%100;
        }
    }
}
 
void computeRow(int size,int r,double *a,double *b,double *c)
{
 
    int j,k;
 
    for(j=0; j<size; j++)
    {
        *(c+r*M+j)=0;
 
        for(k=0; k<size; k++)
        {
            *(c+r*M+j)=*(c+r*M+j)+*(a+r*M+k)* *(b+k*M+j);
        }
    }
}
 
void printTime(struct timeval start, struct timeval end)
{
    int h,m;
    double s,t;
 
    if (start.tv_usec>end.tv_usec)
    {
        t=1000000+end.tv_usec-start.tv_usec;
        t=end.tv_sec - start.tv_sec + t/1000000 - 1;
 
 
    }
    else
    {
        t=end.tv_usec-start.tv_usec;
        t=end.tv_sec - start.tv_sec + t/1000000;
    }
 
    h=t/3600;
    t=t-h*3600;
    m=t/60;
    s=t-m*60;
 
    if(h>0)
        printf("Time: %d:%d:%f hours \n" ,h,m,s);
    else if (m>0)
        printf("Time: %d:%f minutes \n" ,m,s);
    else
        printf("Time: %g secondes \n" ,s);
 
}
 
int main()
{
    struct timeval tvalBefore, tvalAfter;
    char buff[256];
    pid_t childPID;
    double t;
    int i,j;
    int shm_fd;
    double* shared_memory;
    /* the size (in bytes) of the shared memory segment */
    const int msize = 3*M*M*sizeof(double);
    const char *name = "MATRIX_MULT";
 
    shm_fd = shm_open (name, O_CREAT | O_EXCL | O_RDWR, S_IRWXU | S_IRWXG);
    if (shm_fd < 0)
    {
        perror("In shm_open()");
        exit(1);
    }
    fprintf(stderr, "Created shared memory object %s\n", name);
    /* attach the shared memory segment */
    ftruncate(shm_fd, msize);
    printf("shmat returned\n");
    shared_memory = (double *) mmap(NULL, msize, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (shared_memory == NULL)
    {
        perror("In mmap()");
        exit(1);
    }
    fprintf(stderr, "Shared memory segment allocated correctly (%d bytes).\n", msize);
 
    double *a = shared_memory;
    double *b = shared_memory + M*M;
    double *c = b + M*M;
 
    gettimeofday (&tvalBefore, NULL);
    populate(M,a,b);
    printf("Done populating\n");
    for(i=0; i<M; i++)
    {
        childPID=fork();
        if(childPID >= 0)
        {
            if(childPID==0)
            {
 
                gettimeofday (&tvalBefore, NULL);
                computeRow(M, i, a, b, c);
                gettimeofday (&tvalAfter, NULL);
 
                printf("Elapse time for parallel matrix multiplication row number %d \n", i);
                printTime(tvalBefore, tvalAfter);
 
                exit(0);
 
            }
        }
    }
    int x = 0;
    int stat;
    do
    {
        x = waitpid(-1,&stat,WNOHANG);
    }
    while (x==0);
 
    printm(M,a,"Matrix a");
    printm(M,b,"Matrix b");
    printm(M,c,"Matrix a*b");
    /* now detach the shared memory segment */
    shm_unlink(name);
    gettimeofday (&tvalAfter, NULL);
    printf("Elapse time for PARENT of parallel matrix multiplication \n");
    printTime(tvalBefore, tvalAfter);
 
    return 0;
}
