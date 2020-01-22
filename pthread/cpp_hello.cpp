/* File:  
 *    cpp_hello.cpp
 *
 * Purpose:
 *    Illustrate basic use of std::threads:  create some threads,
 *    each of which prints a message.
 *
 * Input:
 *    none
 * Output:
 *    message from each thread
 *
 * Compile:  g++ -g -Wall -o cpp_hello cpp_hello.cpp -lpthread
 * Usage:    ./cpp_hello <thread_count>
 *
 */
#include <iostream>
#include <string>
#include <thread>

const int MAX_THREADS = 64;

/* Global variable:  accessible to all threads */
int thread_count;  

void usage(std::string prog_name);
void Hello(long rank);  /* Thread function */

/*--------------------------------------------------------------------*/
int main(int argc, char* argv[]) {
  long       thread;  /* Use long in case of a 64-bit system */

  /* Get number of threads from command line */
  if (argc != 2) usage(argv[0]);
  thread_count = std::stol(argv[1]);  
  if (thread_count <= 0 || thread_count > MAX_THREADS) usage(argv[0]);

  std::thread thread_handles[thread_count]; 

  for (thread = 0; thread < thread_count; thread++)  
    thread_handles[thread] = std::thread(Hello, thread);

  std::cout << "Hello from the main thread\n";

  for (thread = 0; thread < thread_count; thread++) 
    thread_handles[thread].join(); 

  return 0;
}  /* main */

/*-------------------------------------------------------------------*/
void Hello(long rank) {
  std::cout << "Hello from thread "<< rank << " of " << thread_count << std::endl;
}  /* Hello */

/*-------------------------------------------------------------------*/
void usage(std::string prog_name) {
  std::cerr << "usage: " << prog_name << " <number of threads>\n";
  std::cerr << "0 < number of threads <= "<< MAX_THREADS << std::endl;
  exit(0);
}  /* Usage */
