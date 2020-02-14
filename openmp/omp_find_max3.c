/* File:     omp_find_max3.c
 *
 * Purpose:  A parallel program to find the maximum value of an array using OpenMP
 *
 * Compile:  gcc -g -Wall -fopenmp -o omp_find_max3 omp_find_max3.c
 * Run:      ./omp_find_max3 <number of threads>
 */
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>   

#include "timer.h"

#define NUM_ELEMENTS 100000000

int* generate_values(int n)
{
  int* vec = malloc(n*sizeof(int));
  for(int i=0; i<n; ++i)
    vec[i] = rand() % NUM_ELEMENTS;
  return vec;
}


int main(int argc, char* argv[]) {
  int thread_count = strtol(argv[1], NULL, 10); 

  int* data = generate_values(NUM_ELEMENTS); 

  double start, finish, elapsed;

  GET_TIME(start);
   
  /* Third Edition */
  int largest = 0;
  int lp;
  #pragma omp parallel private(lp) num_threads(thread_count)
  {
    lp = 0;
    #pragma omp for
    for ( int i = 0; i < NUM_ELEMENTS; i++) {
      if ( data[i] > lp )
        lp = data[i];
    }
    if ( lp > largest ) {
      #pragma omp critical
      {
        if ( lp > largest )
          largest = lp;
        }
      }
  }
   
  GET_TIME(finish);
  elapsed = finish - start;
  printf("The elapsed time is %f seconds\n", elapsed);

  return 0; 
}  /* main */
