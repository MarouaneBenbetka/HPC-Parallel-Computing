#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"
#define N 4

int main()
{
    int rank, i, element = 0;
    int *X = NULL;
    int *Y = NULL;
    int *A = NULL;
    int *A_ligne = NULL;

    X = (int *)malloc(N * sizeof(int));
    A_ligne = (int *)malloc(N * sizeof(int));

    for (i = 0; i < N; i++)
        X[i] = i + 1;

    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0)
    {
        Y = (int *)malloc(N * sizeof(int));
        A = (int *)malloc(N * N * sizeof(int));

        for (i = 0; i < N; i++)
            Y[i] = 0;
        for (i = 0; i < N * N; i++)
            A[i] = i * 2;
    }

    MPI_Scatter(A, N, MPI_INT, A_ligne, N, MPI_INT, 0, MPI_COMM_WORLD);
    for (i = 0; i < N; i++)
        element = element + (A_ligne[i] * X[i]);
    MPI_Gather(&element, 1, MPI_INT, Y, 1, MPI_INT, 0, MPI_COMM_WORLD);
    if (rank == 0)
    {
        for (i = 0; i < N; i++)
            printf("%d\n", Y[i]);
    }

    MPI_Finalize();

    return 0;
}