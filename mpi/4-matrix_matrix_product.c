#include <stdio.h>
#include "mpi.h"
#define SIZE 8

int A[SIZE][SIZE], B[SIZE][SIZE], C[SIZE][SIZE];

void fill_matrix(int m[SIZE][SIZE])
{
    int i, j;
    for (i = 0; i < SIZE; i++)
        for (j = 0; j < SIZE; j++)
            m[i][j] = i + j;
}

int main(int argc, char *argv[])
{
    int myrank, P, from, to, i, j, k;
    int tag = 666;
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
    MPI_Comm_size(MPI_COMM_WORLD, &P);

    if (SIZE % P != 0)
    {
        if (myrank == 0)
            printf("Matrix size not divisible by number of processors\n");
        MPI_Finalize();
        exit(-1);
    }

    from = myrank * SIZE / P;
    to = (myrank + 1) * SIZE / P;

    if (myrank == 0)
    {
        fill_matrix(A);
        fill_matrix(B);
    }

    MPI_Bcast(B, SIZE * SIZE, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatter(A, SIZE * SIZE / P, MPI_INT, A[from], SIZE * SIZE / P, MPI_INT, 0, MPI_COMM_WORLD);

    printf("computing slice %d (from row %d to %d)\n", myrank, from, to - 1);
    for (i = from; i < to; i++)
        for (j = 0; j < SIZE; j++)
        {
            C[i][j] = 0;
            for (k = 0; k < SIZE; k++)
                C[i][j] += A[i][k] * B[k][j];
        }
    MPI_Gather(C[from], SIZE * SIZE / P, MPI_INT, C, SIZE * SIZE / P, MPI_INT, 0, MPI_COMM_WORLD);

    MPI_Finalize();

    return 0;
}