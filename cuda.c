#include <stdio.h>

// Définir la taille de la matrice et du vecteur
#define N 256

// Fonction pour initialiser la matrice et le vecteur
void initialize(int *matrix, int *vector)
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
            matrix[i * N + j] = 1;
    }

    for (int i = 0; i < N; ++i)
    {
        vector[i] = 1;
    }
}

// Fonctions pour afficher les résultas
void displayMatrix(int *matrix)
{
    for (int i = 0; i < N; ++i)
    {
        for (int j = 0; j < N; ++j)
        {
            printf("%d ", matrix[i * N + j]);
        }
        printf("\n");
    }
}

void displayVector(int *matrix)
{

    for (int i = 0; i < N; i++)
    {
        printf("%d ", matrix[i]);
    }
}

// Fonction du kernel CUDA pour le produit matrice-vecteur
__global__ void matrixVectorProduct(int *A, int *B, int *C)
{
    int row = blockIdx.y;
    int col = blockIdx.x;
    int indice_i = (threadIdx.x * 4 + threadIdx.y);
    int indice_b = row * 16 + col;
    int nb = N / 16;
    int __shared__ sum_tmp[N / 16];

    for (int i = (indice_i)*nb; i < (indice_i + 1) * nb; i++)
    {

        sum_tmp[indice_i] += A[(indice_b)*N + i] * B[i];
    }

    __syncthreads();

    if (indice_i == 0)
    {
        int sum = 0;
        for (int i = 0; i < nb; i++)
            sum += sum_tmp[i];
        C[indice_b] = sum;
    }
}

int main()
{
    // Allouer de la mémoire sur le CPU
    int *h_A, *h_B, *h_C;
    float elapsedTime;
    cudaEvent_t start, stop;

    h_A = (int *)malloc(N * N * sizeof(int));
    h_B = (int *)malloc(N * sizeof(int));
    h_C = (int *)malloc(N * sizeof(int));

    // Initialiser la matrice et le vecteur
    initialize(h_A, h_B);

    // Allouer de la mémoire sur le GPU
    int *d_A, *d_B, *d_C;

    cudaMalloc((void **)&d_A, N * N * sizeof(int));
    cudaMalloc((void **)&d_B, N * sizeof(int));
    cudaMalloc((void **)&d_C, N * sizeof(int));

    // Copier les données du CPU vers le GPU
    cudaMemcpy(d_A, h_A, N * N * sizeof(int), cudaMemcpyHostToDevice);
    cudaMemcpy(d_B, h_B, N * sizeof(int), cudaMemcpyHostToDevice);

    // Définir la configuration de la grille et des blocs
    dim3 blockSize(4, 4);
    dim3 gridSize(16, 16);

    // Appeler le noyau CUDA

    cudaEventCreate(&start);
    cudaEventCreate(&stop);
    cudaEventRecord(start, 0);
    matrixVectorProduct<<<gridSize, blockSize, 0>>>(d_A, d_B, d_C);
    cudaEventRecord(stop, 0);
    cudaEventSynchronize(stop);
    cudaEventElapsedTime(&elapsedTime, start, stop);
    printf("\n Elapsed time : %f ms\n", elapsedTime);

    // Copier les résultats du GPU vers le CPU
    cudaMemcpy(h_C, d_C, N * sizeof(int), cudaMemcpyDeviceToHost);

    // displayMatrix(h_A);

    // displayVector(h_B);

    // Afficher la matrice résultante
    displayVector(h_C);

    // Libérer la mémoire
    free(h_A);
    free(h_B);
    free(h_C);

    cudaFree(d_A);
    cudaFree(d_B);
    cudaFree(d_C);

    return 0;
}
