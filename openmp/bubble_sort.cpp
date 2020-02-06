#include <iostream>
#include <vector>

#include <omp.h>
#include <stdlib.h>

constexpr int N = 20;

int main(int argc, const char** argv) {
  std::vector<int> vec(20);

  for(auto& e : vec) {
    e = rand() % N;
  }

  #pragma omp parallel for num_threads(4)
  for (int length = N; length >= 2; --length) {
    int my_rank = omp_get_thread_num();
    printf("Thread %d length %d\n", my_rank, length);
    for (int i = 0; i < length-1; i++)
      if (vec[i] > vec[i+1]) {
        auto tmp = vec[i];
        vec[i] = vec[i+1];
        vec[i+1] = tmp;
      }
  }

  for(auto e : vec) {
    std::cout << e << ", ";
  }
  
  return 0;
}