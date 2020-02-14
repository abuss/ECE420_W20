/* File:     omp_functions.c
 *
 * Purpose:  A program to query the parallel capabilities using OpenMP
 *
 * Compile:  gcc -g -Wall -fopenmp -o omp_functions omp_functions.c
 * Run:      ./omp_functions <number of threads>
 * 
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
      int thread_count = omp_get_num_threads();
      int thread_max_count = omp_get_max_threads();
      int processors_count = omp_get_num_procs();

      #pragma omp single
      {
         printf("Thread id: %d\n", my_rank);
         printf("Number of threads in team: %d\n",thread_count);
         printf("Number maximum of threads: %d\n",thread_max_count);
         printf("Number of processors: %d\n",processors_count);
      }
   }
}  /* Hello */
