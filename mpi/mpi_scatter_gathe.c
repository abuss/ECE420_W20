/* File:       
 *    mpi_scatter_gather.c
 *
 * Purpose:    
 *    A program that uses MPI to do scatter and gather
 *
 * Compile:    
 *    mpicc -g -Wall -o mpi_scatter_gather mpi_scatter_gather.c
 * Usage:        
 *    mpiexec -n<number of processes> ./mpi_scatter_gather
 *
 * Input:      
 *    None
 * Output:     
 *    A greeting from each process
 *
 * Algorithm:  
 *    Each process receive a message from process 0, and prints 
 *    the message received.
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <mpi.h>     /* For MPI functions, etc */ 

const int MAX_STRING = 100;

int main(void) {
   int        comm_sz;               /* Number of processes    */
   int        my_rank;               /* My process rank        */
   
   double*       data; 
   double*       local_data; 
   
   /* Start up MPI */
   MPI_Init(NULL, NULL); 

   /* Get the number of processes */
   MPI_Comm_size(MPI_COMM_WORLD, &comm_sz); 

   /* Get my rank among all the processes */
   MPI_Comm_rank(MPI_COMM_WORLD, &my_rank); 

   if (my_rank == 0) { 
      data = (double*)malloc(comm_sz*sizeof(double));

      printf("Please enter %d numbers:\n",comm_sz);
      for(int i=0; i < comm_sz; ++i)
         scanf("%le",data+i);
   }  
   MPI_Barrier(MPI_COMM_WORLD);
   
   local_data = (double*)malloc(comm_sz*sizeof(double));

   /* Scatter a message to all processes */
   MPI_Scatter(data, 1, MPI_DOUBLE, local_data, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD); 

   /* Print my message */
   printf("Data received %f on process %d!\n", local_data[0], my_rank);


   /* Update local values */
   double mult = pow(10,my_rank);
   local_data[0] *= mult;

   /* Gather data from all processes */
   MPI_Gather(local_data, 1, MPI_DOUBLE, data, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD); 

   if (my_rank == 0) { 
      printf("Received values on process 0:\n");
      for(int i=0; i < comm_sz; ++i)
         printf("%f ",data[i]);
      free(data);
   }  

   free(local_data);

   /* Shut down MPI */
   MPI_Finalize(); 

   return 0;
}  /* main */
