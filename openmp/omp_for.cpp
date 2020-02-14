/* File:     omp_for.cpp
 *
 * Purpose:  A parallel for hello world program that uses OpenMP
 *
 * Compile:  g++ -g -Wall -fopenmp -o omp_for_cpp omp_for.cpp
 * Run:      ./omp_for_cpp <number of threads>
 * 
 * Input:    none
 * Output:   A message from each thread
 *
 * IPP:      Section 5.1 (pp. 211 and ff.)
 */
#include <iostream>
#include <vector>

#include <omp.h>   

constexpr int NUM_ELEMENTS = 15;

int main(int argc, char* argv[]) {
  int thread_count = std::stol(argv[1]); 

  std::vector<int> vec(NUM_ELEMENTS);
  
  int i = 0;
  #pragma omp parallel for num_threads(thread_count)  
  for (auto& e : vec)
  {
    int my_rank = omp_get_thread_num();
    printf("Iteration %d - Thread %d of %d\n", i, my_rank, thread_count);      
    e = ++i;
  }

  for (auto e : vec)
  {
    std::cout << e << ", ";
  }

  // printf("\n----------------------\n");

  // i = 0;
  // #pragma omp parallel for num_threads(thread_count)  
  // for (auto itr=vec.begin(); itr!=vec.end(); ++itr)
  // {
  //   *itr = ++i;
  //   int my_rank = omp_get_thread_num();
  //   printf("Iteration %d - Thread %d of %d\n", i, my_rank, thread_count);      
  // }

  // for (auto e : vec)
  // {
  //   std::cout << e << ", ";
  // }

  // printf("\n----------------------\n");

  // #pragma omp parallel for num_threads(thread_count)  
  // for (size_t i=0; i<vec.size(); ++i)
  // {
  //   vec[i] = i+1;
  //   int my_rank = omp_get_thread_num();
  //   printf("Iteration %ld - Thread %d of %d\n", i, my_rank, thread_count);      
  // }

  // for (auto e : vec)
  // {
  //   std::cout << e << ", ";
  // }

  return 0; 
}  /* main */