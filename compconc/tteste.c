#include <stdio.h>
#include <pthread.h> 
#include <unistd.h>
#include <stdlib.h>
#define max 10

int r1, r2, c1, c2, res_mat[max][max], mat1[max][max], mat2[max][max];

void * mult_thread(void * t) {
    int i, r, c, tot = 0, * rc;

    rc = (int * ) t;
    r = rc[0];
    c = rc[1];

    for (i = 0; i < c1; i++) {
        tot += mat1[r][i] * mat2[i][c];
    }

    res_mat[r][c] = tot;
    return (NULL);
}

void accept(int mat[max][max], int r, int c) {
    int i, j;

    for (i = 0; i < r; i++)
    for (j = 0; j < c; j++) {
        mat[i][j]=1;
    }
    printf("\n");
}

void display() {
    int i, j;

    printf("\n");
    for (i = 0; i < r1 || i < r2; i++) {
        for (j = 0; j < c1; j++) {
            if (i < r1) printf(" %d ", mat1[i][j]);
        }

        if (i == r1 / 2) printf("\t*\t");
        else printf("\t\t");
        for (j = 0; j < c2; j++) {
            if (i < r2) printf(" %d ", mat2[i][j]);
        }

        if (i == r1 / 2) printf("\t=\t");
        else printf("\t\t");

        for (j = 0; j < c2; j++) {
            if (i < r1) printf(" %d ", res_mat[i][j]);
        }

        printf("\n");
    }
    printf("\n");
}

int main(void) {
    int i, j, rc[2];
    pthread_t p[max][max];

    printf("\nEnter : \n");
    scanf("%d", & r1);
    printf("\n");

	c1=r1;
	r2=r1;
	c2=r2;
	
    accept(mat1, r1, c1);
	accept(mat2, r2, c2);

    if (c1 == r2) {
        for (i = 0; i < r1; i++)
        for (j = 0; j < c2; j++) {
            rc[0] = i;
            rc[1] = j;
            pthread_create( & p[i][j], NULL, mult_thread, rc);
        }
    }

    display();

    return 1;
}
