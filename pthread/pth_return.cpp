/* File:  
 *    pth_return.cpp
 *
 * Purpose:
 *    Illustrate basic use of pthreads compiled using C++:  create some threads,
 *    each of which prints a message and return a value.
 *
 * Input:
 *    none
 * Output:
 *    message from each thread
 *
 * Compile:  g++ -g -Wall -o pth_return pth_return.cpp -lpthread
 * Usage:    ./pth_return
 *
 */

#include <iostream>
#include <cstdlib>
#include <pthread.h>

constexpr int NUM_THREADS = 5;

struct thread_data {
   int  thread_id;
   std::string message;
};

int main () {
  pthread_t threads[NUM_THREADS];
  thread_data td[NUM_THREADS];
  int rc;
  int i;

  auto proc = [](void* thread_info) -> void* {
    thread_data* t_info = (thread_data*)thread_info; 
    std::cout << "  -> Thread ID : " << t_info->thread_id ; 
    std::cout << " Message : " << t_info->message << std::endl;
    pthread_exit((void*)&(t_info->thread_id));
  };

  for( i = 0; i < NUM_THREADS; i++ ) {
    td[i].thread_id = i;
    td[i].message = "This is a message";
    std::cout << "main() : creating thread, " << i << std::endl;
    rc = pthread_create(&threads[i], nullptr, proc, (void *)&td[i]);
      
    if (rc) {
      std::cout << "Error:unable to create thread," << rc << std::endl;
      exit(-1);
    }
  }

  for( i = 0; i < NUM_THREADS; i++ ) {
    void* res;
    pthread_join(threads[i], &res);
    auto tid = (int*)res;
    std::cout << "Thread " << i << " joined returned " 
              << *tid << std::endl;
  }
  pthread_exit(NULL);
}