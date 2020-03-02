/* File:       
 *    mpi_bcast.c
 *
 * Purpose:    
 *    A "hello,world" program that uses MPI
 *
 * Compile:    
 *    mpicc -g -Wall -o mpi_bcast mpi_bcast.c
 * Usage:        
 *    mpiexec -n<number of processes> ./mpi_bcast
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
#include <string.h>  /* For strlen             */
#include <mpi.h>     /* For MPI functions, etc */ 

const int MAX_STRING = 100;

int main(void) {
   char       name[MAX_STRING];  /* String storing message */
   int        comm_sz;               /* Number of processes    */
   int        my_rank;               /* My process rank        */

   /* Start up MPI */
   MPI_Init(NULL, NULL); 

   /* Get the number of processes */
   MPI_Comm_size(MPI_COMM_WORLD, &comm_sz); 

   /* Get my rank among all the processes */
   MPI_Comm_rank(MPI_COMM_WORLD, &my_rank); 

   if (my_rank == 0) { 
      printf("What's your name? \n");
      scanf("%s",name);
   }  
   MPI_Barrier(MPI_COMM_WORLD);

   /* Broadcast message to all processes */
   MPI_Bcast(name, MAX_STRING, MPI_CHAR, 0, MPI_COMM_WORLD); 

   /* Synchronize again before print */
   MPI_Barrier(MPI_COMM_WORLD);

   /* Print my message */
   printf("Greetings %s from process %d of %d!\n", name, my_rank, comm_sz);

   /* Shut down MPI */
   MPI_Finalize(); 

   return 0;
}  /* main */
