%%cu
#include <stdio.h>
#include <stdlib.h>
#define N 512

__global__ void dot (int *a , int *b , int *c) 
{
   __shared__ int temp[N]; 	                                                             
   temp[threadIdx.x] = a[threadIdx.x] * b[threadIdx.x]; 
   
   __syncthreads ();

   // Le thread 0 effectue la somme 
   if (threadIdx.x == 0) {
      int sum = 0;
	 for (int i = 0; i < N; i++)
           sum += temp[i];
      *c = sum;        
   }} 

int main (){
   int *a, * b, *c;
   int *gpu_a, *gpu_b, *gpu_c;
   int size = N * sizeof (int); 
      
   cudaMalloc ((void **) &gpu_a, size); 
   cudaMalloc ((void **) &gpu_b, size);
   cudaMalloc ((void **) &gpu_c, sizeof (int)); 
   
   a = (int *) malloc (size);
   b = (int *) malloc (size);
   c = (int *) malloc (sizeof (int)); 

   for (int i = 0; i < N; i++)
      {
       a[i] = i;
      }


   for (int i = 0; i < N; i++)
      {
       b[i] = 2*i;
      }

   cudaMemcpy (gpu_a, a, size, cudaMemcpyHostToDevice); 
   cudaMemcpy (gpu_b, b, size, cudaMemcpyHostToDevice);
 
   dot <<<1, N>>> (gpu_a, gpu_b, gpu_c);

   cudaMemcpy (c, gpu_c, sizeof(int), cudaMemcpyDeviceToHost);

   cudaFree(gpu_a); cudaFree(gpu_b); cudaFree(gpu_c);

   printf("%d\n", *c);

   free(a); free(b); free(c);

   return 0;
}