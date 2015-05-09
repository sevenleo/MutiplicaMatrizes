
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
#include <time.h>
 
 
 
 
double *a,*b,*c;
 
int M;
 
void print_matrix(double *m)
{
    int i,j;
    for(i=0; i<M; i++)
    {
        for(j=0; j<M; j++)
        {
            printf("%g\t" ,*(m+M*i+j));
        }
        printf("\n");
    }
 
    printf("\n");
}
 
void generate_matrix(double *m)
{
    int i;
    for(i=0; i<M*M; i++)
    {
        *(m+i) = 1+rand()%100;
    }
}
 
void calc_row(int row)
{
    int j,k;
    for(j=0; j<M; j++)
    {
        *(c+row*M+j)=0;
        for(k=0; k<M; k++)
        {
            *(c+row*M+j)=*(c+row*M+j)+*(a+row*M+k)* *(b+k*M+j);
        }
    }
}
 
int main(int argc, char *argv[])
{
     srand(time(NULL));
     if(argc != 2){
 
        printf("\nERROR\n");
 
        exit(0);
 
    }
 
    if(atoi(argv[1]) < 1){
 
        printf("\nERROR\n");
 
        exit(0);
 
    }
    int i;
    M = atoi(argv[1]);
    clock_t t1,t2;
    int shm_fd;
    double* shared_memory;
 
    int msize = 3*M*M*sizeof(double);
    char *name = "MATRIX_MULT";
 
    shm_fd = shm_open (name, O_CREAT | O_EXCL | O_RDWR, S_IRWXU | S_IRWXG);
 
    if (shm_fd < 0)
    {
        perror("In shm_open()");
        exit(1);
    }
 
    ftruncate(shm_fd, msize);
 
    shared_memory = (double *) mmap(NULL, msize, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (shared_memory == NULL)
    {
        perror("In mmap()");
        exit(1);
    }
 
    a = shared_memory;
    b = shared_memory + M*M;
    c = b + M*M;
 
    generate_matrix(a);
    generate_matrix(b);
   
    t1 = clock();
    for(i=0; i<M; i++)
    {
        int childPID=fork();
        if(childPID >= 0)
        {
            if(childPID==0)
            {
                calc_row(i);
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
   
    shm_unlink(name);
    t2 = clock();
 
    print_matrix(a);
 
    print_matrix(b);
 
    print_matrix(c);
 
 
    printf("\n\ntime taken = %lf\n",(double)(t2-t1)/(CLOCKS_PER_SEC / 1000.0));
 
    return 0;
 
}

