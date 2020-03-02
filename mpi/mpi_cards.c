/* File:       
 *    mpi_cards.c
 *
 * Purpose:    
 *    A program that send multiple messages using MPI
 *
 * Compile:    
 *    mpicc -g -Wall -o mpi_cards mpi_cards.c
 * Usage:        
 *    mpiexec -n<number of processes> ./mpi_cards
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

char* hspace(int n) {
   char* space =  malloc(n+1);
   space = memset(space,' ',n);
   space[n] = '\0';
   return space;
}


int main(void) {
   int        comm_sz;               /* Number of processes    */
   int        my_rank;               /* My process rank        */
   MPI_Status status;                /* Communication status   */ 
   int        next;                  /* Next process rank to send a message */

   /* Start up MPI */
   MPI_Init(NULL, NULL); 

   /* Get the number of processes */
   MPI_Comm_size(MPI_COMM_WORLD, &comm_sz); 

   /* Get my rank among all the processes */
   MPI_Comm_rank(MPI_COMM_WORLD, &my_rank); 

   char letter = 'a';

   if (comm_sz > 1) {

      if (my_rank == 0) {  // Master process
         for(int i=0; i < 24; ++i) {
            /* Create message */
            next = (i + 1) % comm_sz;
            printf("Card [%c] to %d!\n", letter++, next);
            /* Send message to next process */
            MPI_Send(&letter, 1, MPI_CHAR, next, 0, MPI_COMM_WORLD); 
         }
         letter = '.';
         for (int i=1; i<comm_sz; ++i) {
            // MPI_Send(&letter, 1, MPI_CHAR, i, 0, MPI_COMM_WORLD);
            MPI_Ssend(&letter, 1, MPI_CHAR, i, 0, MPI_COMM_WORLD);
         }
         printf("Master done\n");
      }
      else {
         do {
            /* Receive message from other process */
            MPI_Recv(&letter, 1, MPI_CHAR, 0, 0, MPI_COMM_WORLD, &status);

            printf("%sPlayer %d received letter [%c]\n", hspace(my_rank*20), my_rank, letter);

            sleep(1);

         } while (letter!='.');
      }

   } else {
      printf("Playing solitare!\n");
   }
   /* Shut down MPI */
   MPI_Finalize(); 

   return 0;
}  /* main */
