
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "mpi.h"
#define PI 3.1415926535L

double f(double a) { return (double)4.0 / ((double)1.0 + (a * a)); }

int main()
{
    int n, myid, numprocs, i;
    double mypi, pi, h, sum, x;

    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);
    n = 0;
    for (;;)
    {
        if (myid == 0)
        {
            printf(" Enter the number of intervals: (0 quits )");
            scanf("%d", &n);
        }
        MPI_Bcast(&n, 1, MPI_INTEGER, 0, MPI_COMM_WORLD);
        if (n == 0)
            break;
        h = ((double)1.0) / (double)n;
        sum = 0.0;
        for (i = myid + 1; i <= n; i += numprocs)
        {
            x = h * ((double)i - (double)0.5);
            sum += f(x);
        }
        mypi = h * sum;
        MPI_Reduce(&mypi, &pi, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
        if (myid == 0)
            printf("PI is approximately %.16f, Error is %.16f\n", pi, fabs(pi - PI));
    }
    MPI_Finalize();

    return 0;
}