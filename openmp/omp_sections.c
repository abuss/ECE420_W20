/* File:     omp_sections.c
 *
 * Purpose:  A parallel program to execute different code using OpenMP
 *
 * Compile:  gcc -g -Wall -fopenmp -o omp_section omp_section.c
 * Run:      ./omp_section <number of threads>
 * 
 * Input:    none
 * Output:   A message from each thread
 *
 * IPP:      Section 5.1 (pp. 211 and ff.)
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <omp.h>   

void Hello(void);  /* Thread function */

/*--------------------------------------------------------------------*/
int main(int argc, char* argv[]) {
   int thread_count = strtol(argv[1], NULL, 10); 

#  pragma omp parallel num_threads(thread_count)
   {
      int my_rank = omp_get_thread_num();
      int thread_count = omp_get_num_threads();

      printf("Before code - thread %d of %d\n", my_rank, thread_count);

      #pragma omp sections nowait
      {
         #pragma omp section
         {
            int my_rank = omp_get_thread_num();
            printf("  * Code for a thread - thread %d of %d\n", my_rank, thread_count);
            sleep(3);
            printf("  * Done for a thread - thread %d of %d\n", my_rank, thread_count);
         }

         #pragma omp section
         {
            int my_rank = omp_get_thread_num();
            printf("  =                 Code for anther thread - thread %d of %d\n", my_rank, thread_count);
            sleep(1);
            printf("  =                 Done for anther thread - thread %d of %d\n", my_rank, thread_count);
         }
      }

      printf("After code - thread %d of %d\n", my_rank, thread_count);

   }

   return 0; 
}  /* main */
