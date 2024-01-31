%% cu
#include <stdio.h>
#include <stdlib.h>
#define N 1000
#define THREAD_PER_BLOCK 512

__global__ void add(int *a, int *b, int *c)
{
    int indice = threadIdx.x + blockIdx.x * blockDim.x;
    if (indice < N)
        c[indice] = a[indice] + b[indice];
}

int main()
{
    int *a, *b, *c;
    int *gpu_a, *gpu_b, *gpu_c;
    int size = N * sizeof(int);

    cudaMalloc((void **)&gpu_a, size);
    cudaMalloc((void **)&gpu_b, size);
    cudaMalloc((void **)&gpu_c, size);

    a = (int *)malloc(size);
    b = (int *)malloc(size);
    c = (int *)malloc(size);

    for (int i = 0; i < N; i++)
    {
        a[i] = i;
    }

    for (int i = 0; i < N; i++)
    {
        b[i] = 2 * i;
    }

    cudaMemcpy(gpu_a, a, size, cudaMemcpyHostToDevice);
    cudaMemcpy(gpu_b, b, size, cudaMemcpyHostToDevice);

    add<<<(N + THREAD_PER_BLOCK) / THREAD_PER_BLOCK, THREAD_PER_BLOCK>>>(gpu_a, gpu_b, gpu_c);

    cudaMemcpy(c, gpu_c, size, cudaMemcpyDeviceToHost);

    cudaFree(gpu_a);
    cudaFree(gpu_b);
    cudaFree(gpu_c);

    for (int i = 0; i < N; i++)
    {
        printf("%d\n", c[i]);
    }

    free(a);
    free(b);
    free(c);

    return 0;
}