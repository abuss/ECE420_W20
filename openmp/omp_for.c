/* File:     omp_for.c
 *
 * Purpose:  A parallel for hello world program that uses OpenMP
 *
 * Compile:  gcc -g -Wall -fopenmp -o omp_for omp_for.c
 * Run:      ./omp_for <number of threads>
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

  #pragma omp parallel num_threads(thread_count)  
  {
    #pragma omp for
    for (int i = 0; i < 11; ++i)
    {
      int my_rank = omp_get_thread_num();
      printf("Iteration %d - Thread %d of %d\n", i, my_rank, thread_count);      
    }
   }

  printf("\n----------------------\n");

  #pragma omp parallel for num_threads(thread_count)  
  for (int i = 0; i < 11; ++i)
  {
    int my_rank = omp_get_thread_num();
    printf("Iteration %d - Thread %d of %d\n", i, my_rank, thread_count);      
  }

  printf("\n----------------------\n");

  #pragma omp parallel for num_threads(thread_count)  
  for (int i = 0; i < 11; )
  {
    int my_rank = omp_get_thread_num();
    if (my_rank>12) {
      printf("Thead not executing code");
      break;
    }
    else
      printf("Iteration %d - Thread %d of %d\n", i, my_rank, thread_count);      
  }


   return 0; 
}  /* main */