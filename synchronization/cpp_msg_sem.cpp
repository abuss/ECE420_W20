/* File:     cpp_msg.cpp
 *
 * Purpose:  Illustrate a synchronization problem with pthreads:  create 
 *           some threads, each of which creates and prints a message.
 *
 * Input:    none
 * Output:   message from each thread
 *
 * Compile:  g++ -g -Wall -o cpp_msg cpp_msg.cpp -lpthread
 * Usage:    cpp_msg <thread_count>
 *
 */

#include <iostream>
#include <string>
#include <vector>
#include <thread>

#include "semaphore.hpp"
#include "timer.h"

#include <stdio.h>

constexpr int MAX_THREADS = 1024*1024;
constexpr int MSG_MAX = 100;

/* Global variables:  accessible to all threads */
int thread_count;
std::vector<std::string> messages;
std::vector<Semaphore>   semaphores;

void Usage(std::string prog_name);
void Send_msg(long rank);  /* Thread function */

/*--------------------------------------------------------------------*/
int main(int argc, char* argv[]) {
  long       thread;
    
  double start, finish, elapsed;
    
  if (argc != 2) Usage(argv[0]);
  thread_count = std::stol(argv[1]);  

  if (thread_count <= 0 || thread_count > MAX_THREADS) Usage(argv[0]);

  std::thread thread_handles[thread_count];
  
  messages = decltype(messages)(thread_count);
  semaphores = decltype(semaphores)(thread_count);

  GET_TIME(start);
  
  for (thread = 0; thread < thread_count; thread++)
    thread_handles[thread] = std::thread(Send_msg, thread);

  for (auto& thread : thread_handles)
    thread.join();
  
  GET_TIME(finish);
  elapsed = finish - start;
  printf("   Elapsed time = %e seconds\n", elapsed);

  return 0;
}  /* main */


/*--------------------------------------------------------------------
 * Function:    Usage
 * Purpose:     Print command line for function and terminate
 * In arg:      prog_name
 */
void Usage(std::string prog_name) {
  std::cerr << "usage: " << prog_name << " <number of threads>\n";
  exit(0);
}  /* Usage */


/*-------------------------------------------------------------------
 * Function:       Send_msg
 * Purpose:        Create a message and ``send'' it by copying it
 *                 into the global messages array.  Receive a message
 *                 and print it.
 * In arg:         rank
 * Global in:      thread_count
 * Global in/out:  messages
 * Return val:     Ignored
 * Note:           The my_msg buffer is freed in main
 */
void Send_msg(long my_rank) {
  long dest = (my_rank + 1) % thread_count;
  char* my_msg = (char*) malloc(MSG_MAX*sizeof(char));

  sprintf(my_msg, "Hello to %ld from %ld", dest, my_rank);
  messages[dest] = my_msg;
  free(my_msg);
  semaphores[dest].notify();

  semaphores[my_rank].wait();
  std::cout << "Thread " << my_rank << " > " << messages[my_rank] << std::endl;
}  /* Send_msg */
