

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>


#define ROOT 0
#define CHUNK_SIZE 10
#define MATRIX_SIZE 100
#define REQUEST_TAG 999999 // when the process send an init request to receive chunk
#define FINISH_TAG 1000000


void master_process(int chunk_size, int num_process);
void slave_process(int rank, int chunks_ize);
void init_matrix(int *matrix, int size);
void print_matrix(int *matrix, int chunk_size, int num_chunks);


int main()
{
   int rank, num_process;
   int chunksize = CHUNK_SIZE;


   MPI_Init(NULL, NULL);
   MPI_Comm_size(MPI_COMM_WORLD, &num_process);
   MPI_Comm_rank(MPI_COMM_WORLD, &rank);


   // Master process
   if (rank == ROOT)
   {
       master_process(chunksize, num_process);
   }
   // Slave processes
   else
   {
       slave_process(rank, chunksize);
   }


   MPI_Finalize();
   return 0;
}








void master_process(int chunksize, int num_process)
{
   int num_chunks = MATRIX_SIZE / chunksize;
   int data[MATRIX_SIZE];
   int chunk[chunksize];
   MPI_Status status;


   // This keeps track of how many chunks of data have been distributed.
   int chunks_sent = 0;
   // This is used to track the number of chunks that have been processed and returned by the slaves.
   int chunks_received = 0;
   // Initialize the count of active slave
   int active_slaves = num_process - 1;


   printf("Data initialisation : \n");
   init_matrix(data, MATRIX_SIZE);
   print_matrix(data, chunksize, num_chunks);


   // while we didn't receive all chunk or some process slaves are still activeSS
   while (chunks_received < num_chunks || active_slaves > 0)
   {
       MPI_Recv(chunk, chunksize, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);


       // Store processed data from slaves
       if (status.MPI_TAG != REQUEST_TAG)
       {
           for (int i = 0; i < chunksize; i++)
           {
               data[status.MPI_TAG * chunksize + i] = chunk[i];
           };
           chunks_received++;
       }


       // Check if there are more data chunks to send
       if (chunks_sent < num_chunks)
       {
           MPI_Send(&data[chunks_sent * chunksize], chunksize, MPI_INT, status.MPI_SOURCE, chunks_sent, MPI_COMM_WORLD);
           chunks_sent++;
       }
       else
       {
           MPI_Send(NULL, 0, MPI_INT, status.MPI_SOURCE, FINISH_TAG, MPI_COMM_WORLD);
           active_slaves--;
       }
   }


   printf("Data Modified : \n");
   print_matrix(data, chunksize, num_chunks);
}


void slave_process(int rank, int chunksize)
{


   int chunk[chunksize];
   MPI_Status status;


   // Initial request for data
   MPI_Send(NULL, 0, MPI_INT, ROOT, REQUEST_TAG, MPI_COMM_WORLD);


   while (1)
   {
       // Receive data or termination signal
       MPI_Recv(chunk, chunksize, MPI_INT, ROOT, MPI_ANY_TAG, MPI_COMM_WORLD, &status);


       if (status.MPI_TAG == FINISH_TAG)
       	 break;


       // Process data
       for (int i = 0; i < chunksize; ++i)
       {
           chunk[i] = chunk[i] * chunk[i];
       }
       // Send processed data back to master, which is also a new request for data
       MPI_Send(chunk, chunksize, MPI_INT, ROOT, status.MPI_TAG, MPI_COMM_WORLD);
   }
}


void init_matrix(int *matrix, int size)
{
   for (int i = 0; i < size; i++)
   {
       matrix[i] = i;
   }
}
void print_matrix(int *matrix, int chunk_size, int num_chunks)
{
   printf("----------------------------\n");
   for (int i = 0; i < num_chunks; i++)
   {
       printf("CHUNK %3d : ", i);
       for (int j = 0; j < chunk_size; j++)
       {
           printf("%5d ", matrix[i * chunk_size + j]);
       }
       printf("\n");
   }
   printf("\n----------------------------\n");
}
