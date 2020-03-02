/* File:       
 *    mpi_circular.c
 *
 * Purpose:    
 *    A program with circular messages using MPI
 *
 * Compile:    
 *    mpicc -g -Wall -o mpi_circular mpi_circular.c
 * Usage:        
 *    mpiexec -n<number of processes> ./mpi_circular
 *
 * Input:      
 *    None
 * Output:     
 *    A message from each process
 *
 * Algorithm:  
 *    Each process sends a message to process 0, which prints 
 *    the messages it has received, as well as its own message.
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>  /* For strlen             */
#include <mpi.h>     /* For MPI functions, etc */ 

const int MAX_STRING = 100;

int main(void) {
   int        comm_sz;               /* Number of processes    */
   int        my_rank;               /* My process rank        */
   MPI_Status status;                /* Communication status   */ 
   int        next;                  /* Next process rank to send a message */
   int        prev;                  /* Previous process rank to receive a message */

   int a[10], b[10];

   /* Start up MPI */
   MPI_Init(NULL, NULL); 

   /* Get the number of processes */
   MPI_Comm_size(MPI_COMM_WORLD, &comm_sz); 

   /* Get my rank among all the processes */
   MPI_Comm_rank(MPI_COMM_WORLD, &my_rank); 

   for (int i=0; i<10; ++i)
      a[i] = my_rank*10;

   next = (my_rank+1) % comm_sz;
   prev = (my_rank-1+comm_sz) % comm_sz;

   // MPI_Bsend(a, 10, MPI_INT, next, 1, MPI_COMM_WORLD);
   // MPI_Recv(b, 10, MPI_INT, prev, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

/*
   MPI_Request request;
   MPI_Isend(a, 10, MPI_INT, next, 1, MPI_COMM_WORLD, &request);
   MPI_Recv(b, 10, MPI_INT, prev, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);         
*/
/**/
   if (my_rank % 2 == 1) {
      MPI_Ssend(a, 10, MPI_INT, next, 1, MPI_COMM_WORLD);
      MPI_Recv(b, 10, MPI_INT, prev, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
   }
   else {
      MPI_Recv(b, 10, MPI_INT, prev, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      MPI_Ssend(a, 10, MPI_INT, next, 1, MPI_COMM_WORLD);
   }
/**/

   printf("Proc: %d  b=",my_rank);
   for (int i=0; i<10; ++i)
      printf("%d,",b[i]);
   printf("\n");

   /* Shut down MPI */
   MPI_Finalize(); 

   return 0;
}  /* main */
