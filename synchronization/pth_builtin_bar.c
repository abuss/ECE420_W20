/* File:  
 *    pth_builtin_bar_mod.c
 *
 * Purpose:
 *    Use built-in barriers to synchronize threads.
 *
 * Compile:
 *    gcc -g -Wall -o pth_builtin_bar pth_builtin_bar.c -lpthread
 *    timer.h must be available
 *
 * Usage:
 *    ./pth_builtin_bar <thread_count>
 *
 * Input:
 *    none
 * Output:
 *    Time for BARRIER_COUNT barriers
 *
 * Note:
 *    Verbose output can be enabled with the compile flag -DDEBUG
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#include "timer.h"

#define BARRIER_COUNT 100

int thread_count;
int barrier_thread_count = 0;
pthread_barrier_t barr;

void Usage(char* prog_name);
void *Thread_work(void* rank);

/*--------------------------------------------------------------------*/
int main(int argc, char* argv[]) {
   long       thread;
   pthread_t* thread_handles; 
   double start, finish;

   if (argc != 2)
      Usage(argv[0]);
   thread_count = strtol(argv[1], NULL, 10);

   thread_handles = malloc (thread_count*sizeof(pthread_t));
   
   /* Barrier initialization */
   if(pthread_barrier_init(&barr, NULL, thread_count)) // thread_count is the number of threads to be synchronized on barrier
   {
      printf("Could not create a barrier\n");
      return -1;
   }

   GET_TIME(start);
   for (thread = 0; thread < thread_count; thread++)
      pthread_create(&thread_handles[thread], NULL,
          Thread_work, (void*) thread);

   for (thread = 0; thread < thread_count; thread++) {
      pthread_join(thread_handles[thread], NULL);
   }
   GET_TIME(finish);
    
   printf("Elapsed time = %e seconds\n", finish - start);

   pthread_barrier_destroy(&barr);
   free(thread_handles);
   return 0;
}  /* main */


/*--------------------------------------------------------------------
 * Function:    Usage
 * Purpose:     Print command line for function and terminate
 * In arg:      prog_name
 */
void Usage(char* prog_name) {

   fprintf(stderr, "usage: %s <number of threads>\n", prog_name);
   exit(0);
}  /* Usage */


/*-------------------------------------------------------------------
 * Function:    Thread_work
 * Purpose:     Run BARRIER_COUNT barriers
 * In arg:      rank
 * Return val:  Ignored
 */
void *Thread_work(void* rank) {
   long my_rank = (long) rank;
   printf("Thread %ld started\n",my_rank);
   int delay = (my_rank+1) % 5;
   sleep(delay);
   printf("Thread %ld done\n",my_rank);

   pthread_barrier_wait(&barr);
   
   printf("Thread %ld continue\n",my_rank);
   return NULL;
}  /* Thread_work */
