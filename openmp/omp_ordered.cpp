/* File:     omp_ordered.cpp
 *
 * Purpose:  A parallel to show ordered execution using OpenMP
 *
 * Compile:  g++ -g -Wall -fopenmp -o omp_ordered omp_ordered.cpp
 * Run:      ./omp_ordered <number of threads>
 * 
 * Input:    none
 * Output:   A message from each thread
 *
 */
#include <iostream>
#include <vector>

#include <omp.h>   

constexpr int NUM_ELEMENTS = 15;

int main(int argc, char* argv[]) {
  int thread_count = std::stol(argv[1]); 

  std::vector<int> vec(NUM_ELEMENTS);
  
  int i = 0;
# pragma omp parallel for num_threads(thread_count) ordered 
  for (auto& e : vec)
  {
    int my_rank = omp_get_thread_num();
    printf("Iteration %d - Thread %d of %d\n", i, my_rank, thread_count);
#   pragma omp ordered
    e = ++i;
  }

  for (auto e : vec)
  {
    std::cout << e << ", ";
  }

  printf("\n----------------------\n");
  std::vector<int> vec2;

  #pragma omp parallel for num_threads(thread_count)  schedule(dynamic,2) ordered
  for (size_t i=0; i<vec.size(); ++i)
  {
    int my_rank = omp_get_thread_num();
    printf("Iteration %ld - Thread %d of %d\n", i, my_rank, thread_count);      
    #pragma omp ordered
    vec2.push_back(i+1);
  }

  for (auto e : vec2)
  {
    std::cout << e << ", ";
  }

  return 0; 
}  /* main */