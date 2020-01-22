/* File:  
 *    pth_hello.cpp
 *
 * Purpose:
 *    Illustrate basic use of pthreads compiled using C++:  create some threads,
 *    each of which prints a message.
 *
 * Input:
 *    none
 * Output:
 *    message from each thread
 *
 * Compile:  g++ -g -Wall -o pth_hello_cpp pth_hello.cpp -lpthread
 * Usage:    ./pth_hello_cpp <thread_count>
 *
 * IPP:   Section 4.2 (p. 153 and ff.)
 */

#include <iostream>
#include <string>
#include <pthread.h> 

const int MAX_THREADS = 64;

/* Global variable:  accessible to all threads */
int thread_count;  

void usage(std::string prog_name);
void* Hello(void* rank);  /* Thread function */

/*--------------------------------------------------------------------*/
int main(int argc, char* argv[]) {
  long       thread;  /* Use long in case of a 64-bit system */

  /* Get number of threads from command line */
  if (argc != 2) usage(argv[0]);
  thread_count = std::stol(argv[1]);  
  if (thread_count <= 0 || thread_count > MAX_THREADS) usage(argv[0]);

  pthread_t thread_handles[thread_count]; 

  for (thread = 0; thread < thread_count; thread++)  
    pthread_create(&thread_handles[thread], NULL,
          Hello, (void*) thread);  

  printf("Hello from the main thread\n");

  for (thread = 0; thread < thread_count; thread++) 
    pthread_join(thread_handles[thread], NULL); 

  return 0;
}  /* main */

/*-------------------------------------------------------------------*/
void *Hello(void* rank) {
  long my_rank = (long) rank;  /* Use long in case of 64-bit system */ 

  std::cout << "Hello from thread "<< my_rank << " of " << thread_count << std::endl;

  return nullptr;
}  /* Hello */

/*-------------------------------------------------------------------*/
void usage(std::string prog_name) {
  std::cerr << "usage: " << prog_name << " <number of threads>\n";
  std::cerr << "0 < number of threads <= "<< MAX_THREADS << std::endl;
  exit(0);
}  /* Usage */
