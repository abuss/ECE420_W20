/* File:     omp_collapse.c
 *
 * Purpose:  A parallel for collapsing for-loops using OpenMP
 *
 * Compile:  gcc -g -Wall -fopenmp -o omp_collapse omp_collapse.c
 * Run:      ./omp_collapse <number of threads>
 * 
 * Input:    none
 * Output:   A message from each thread
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>   

#define SIZE 10

/*--------------------------------------------------------------------*/
int main(int argc, char* argv[]) {
  int thread_count = strtol(argv[1], NULL, 10); 

  int* mat = malloc(SIZE*SIZE*sizeof(int));
  int k = 0;

  for(int i = 0; i<SIZE; ++i)
    for(int j = 0; j<SIZE; ++j)
      mat[j + i*SIZE] = ++k;

  for(k = 0; k < SIZE*SIZE; ++k) {
    printf("%4d ",mat[k]);
    if ((k+1) % SIZE == 0) printf("\n");
  }

  printf("\n----------------------\n");

  k = 0;
# pragma omp parallel for num_threads(thread_count) shared(k) collapse(2)
  for(int i = 0; i<SIZE; ++i)
    for(int j = 0; j<SIZE; ++j) {
      mat[j + i*SIZE] = ++k;
      int my_rank = omp_get_thread_num();
      printf("Iteration %d,%d - Thread %d of %d\n", i, j, my_rank, thread_count);      
    }

  for(k = 0; k < SIZE*SIZE; ++k) {
    printf("%4d ",mat[k]);
    if ((k+1) % SIZE == 0) printf("\n");
  }


  // #pragma omp parallel num_threads(thread_count)  
  // {
  //   #pragma omp for
  //   for (int i = 0; i < 11; ++i)
  //   {
  //     int my_rank = omp_get_thread_num();
  //     printf("Iteration %d - Thread %d of %d\n", i, my_rank, thread_count);      
  //   }
  //  }

  // printf("\n----------------------\n");

  // #pragma omp parallel for num_threads(thread_count)  
  // for (int i = 0; i < 11; ++i)
  // {
  //   int my_rank = omp_get_thread_num();
  //   printf("Iteration %d - Thread %d of %d\n", i, my_rank, thread_count);      
  // }

  // printf("\n----------------------\n");

  // #pragma omp parallel for num_threads(thread_count)  
  // for (int i = 0; i < 11; )
  // {
  //   int my_rank = omp_get_thread_num();
  //   if (my_rank>12) {
  //     printf("Thead not executing code");
  //     break;
  //   }
  //   else
  //     printf("Iteration %d - Thread %d of %d\n", i, my_rank, thread_count);      
  // }


   return 0; 
}  /* main */