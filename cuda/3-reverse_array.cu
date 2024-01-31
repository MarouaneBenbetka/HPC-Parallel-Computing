%%cu
#include <stdio.h>
#include <stdlib.h>
#define N 2048
#define THREAD_PER_BLOCK 512

__global__ void reverseArray (int *d_b , int *d_a)
{
  int old_id = threadIdx.x + blockIdx.x * blockDim.x; 
  int new_id = N - 1 - old_id ; 
  d_b[old_id] = d_a[new_id];
}

int main (){
   int *h_a, *d_a, *d_b;
   int size = N * sizeof (int); 
   h_a = (int *) malloc (size);

   for (int i = 0; i < N; i++)
   {
       h_a[i] = i;
   }

   cudaMalloc ((void **) &d_a, size); 
   cudaMalloc ((void **) &d_b, size); 
   cudaMemcpy (d_a, h_a, size, cudaMemcpyHostToDevice);
 
   reverseArray <<< N/THREAD_PER_BLOCK, THREAD_PER_BLOCK >>>(d_b, d_a);

   cudaMemcpy (h_a, d_b, size, cudaMemcpyDeviceToHost);

   cudaFree(d_a); cudaFree(d_b); 

   for (int i = 0; i < N; i++)
   {
       printf("%d\n", h_a[i]);
   }


   free(h_a);

   return 0;}