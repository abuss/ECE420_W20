/* File:     omp_schedule.c
 *
 * Purpose:  A parallel hello, world program that uses OpenMP
 *
 * Compile:  gcc -g -Wall -fopenmp -o omp_schedule omp_schedule.c
 * Run:      ./omp_schedule <number of threads>
 * 
 * Input:    none
 * Output:   A message from each thread
 *
 * IPP:      Section 5.1 (pp. 211 and ff.)
 */
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>   

/*--------------------------------------------------------------------*/
int main(int argc, char* argv[]) {
   int thread_count = strtol(argv[1], NULL, 10); 

#  pragma omp parallel num_threads(thread_count) 
   {
      int my_rank = omp_get_thread_num();

      printf("Hello from thread %d of %d\n", my_rank, thread_count);
   }

   return 0; 
}  /* main */

