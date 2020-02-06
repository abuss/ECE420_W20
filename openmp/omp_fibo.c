/* File:     omp_fibo.c
 *
 * Purpose:  A parallel for fibonacci program that uses OpenMP
 *
 * Compile:  gcc -g -Wall -fopenmp -o omp_fibo omp_fibo.c
 * Run:      ./omp_fibo <number of threads>
 * 
 * Input:    none
 * Output:   A message from each thread
 *
 * IPP:      Section 5.1 (pp. 211 and ff.)
 */
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>   

#define NUM_ELEMENTS 15

/*--------------------------------------------------------------------*/
int main(int argc, char* argv[]) {
  int thread_count = strtol(argv[1], NULL, 10); 

  int* fibo = malloc(NUM_ELEMENTS*sizeof(int));
  
  fibo[0] = 0;
  fibo[1] = 1;
  #pragma omp parallel for num_threads(thread_count)  
  for (int i = 2; i < NUM_ELEMENTS; ++i)
  {
    fibo[i] = fibo[i-1] + fibo[i-2];
    int my_rank = omp_get_thread_num();
    printf("Iteration %d - Thread %d of %d\n", i, my_rank, thread_count);      

  }

  for (int i = 0; i < NUM_ELEMENTS; ++i)
  {
    printf("%d, ",fibo[i]);
  }
  
   return 0; 
}  /* main */