%%cu
#include <stdio.h>
#include <stdlib.h>
#define BLOCKSIZE 16
#define SIZE 128

__global__ void vectvectshared (int *A, int *B, int *r)
{    
  __shared__ int temp[SIZE];
	
  int i = threadIdx.x; 
  int j = threadIdx.y;

  int ind  = j + (blockDim.x * i);

  if (ind < SIZE)
     temp[ind] = A[ind] * B[ind];
    
   __syncthreads();

   if(ind == 0){
     int sum = 0;
     for(int i = 0; i < SIZE; i++)
        sum += temp[i];
     *r = sum;
   }
}

void fill_dp_vector (int* vec,int size)
{
   int ind;
   for(ind = 0; ind < size; ind++)
        vec[ind] = 3*ind;
}

int main ()
{
   int *hostA, *hostB, *res;
   int *devA, *devB, *devres;

   int vlen;

   vlen=SIZE;
	
   dim3 threadspblock(BLOCKSIZE,BLOCKSIZE);

   hostA = (int *) malloc (vlen * sizeof(int));
   hostB = (int *) malloc (vlen * sizeof(int));
   res = (int *) malloc (sizeof(int));
   
   fill_dp_vector (hostA, vlen);   
   fill_dp_vector (hostB, vlen);

   cudaMalloc((void **) &devA, vlen * sizeof(int));
   cudaMalloc((void **) &devB, vlen * sizeof(int));
   cudaMalloc((void **) &devres, sizeof(int));

   cudaMemcpy(devA, hostA, vlen * sizeof(int), cudaMemcpyHostToDevice);
   cudaMemcpy(devB, hostB, vlen * sizeof(int), cudaMemcpyHostToDevice);
		
   vectvectshared<<<1, threadspblock>>>(devA, devB, devres);

   cudaMemcpy (res, devres, sizeof(int), cudaMemcpyDeviceToHost);
 
   cudaFree (devA);
   cudaFree (devB);
   cudaFree (devres);

   printf("%d\n", *res);

   free (hostA);
   free (hostB);
   free (res);

   return 0;}