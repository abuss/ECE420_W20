/* File:       
 *    mpi_ping_pong.c
 *
 * Purpose:    
 *    A program that ping -pong messages using MPI
 *
 * Compile:    
 *    mpicc -g -Wall -o mpi_ping_pong mpi_ping_pong.c
 * Usage:        
 *    mpiexec -n<number of processes> ./mpi_ping_pong
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

   /* Start up MPI */
   MPI_Init(NULL, NULL); 

   /* Get the number of processes */
   MPI_Comm_size(MPI_COMM_WORLD, &comm_sz); 

   /* Get my rank among all the processes */
   MPI_Comm_rank(MPI_COMM_WORLD, &my_rank); 

   int ball = 0;

   if (comm_sz > 1) {

      if (my_rank == 0) { 
         /* Create message */
         next = (my_rank + 1) % comm_sz;
         ++ball;
         printf("Ping (%d) from process %d --> %d!\n", ball, my_rank, next);
         /* Send message to next process */
         MPI_Send(&ball, 1, MPI_INT, next, 0, MPI_COMM_WORLD); 
      }

      /* Receive message from other process */
      prev = (my_rank + comm_sz - 1) % comm_sz;
      MPI_Recv(&ball, 1, MPI_INT, prev, 0, MPI_COMM_WORLD, &status);
      int source = status.MPI_SOURCE;

      printf("Ball received from %d",source);
      ++ball;
      next = (my_rank + 1) % comm_sz;
      if (ball % 2 == 1)
         printf(" : Ping ");
      else
         printf(" : Pong ");
      printf(" (%d) from process %d --> %d!\n", ball, my_rank, next);
      /* Send message to next process */
      MPI_Send(&ball, 1, MPI_INT, next, 0, MPI_COMM_WORLD); 

   } else {
      printf("Can't play ping-pong alone!\n");
   }
   /* Shut down MPI */
   MPI_Finalize(); 

   return 0;
}  /* main */
