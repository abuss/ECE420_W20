/* File:  
 *    hello_fork.cpp
 *
 * Purpose:
 *    Illustrate basic use of fork to create new processes,
 *    each of which prints a message.
 *
 * Input:
 *    none
 * Output:
 *    message from each thread
 *
 * Compile:  g++ -g -Wall -o hello_fork hello_fork.cpp
 * Usage:    ./hello_fork
 *
 */

#include <iostream>
#include <unistd.h>

int main(int argc, const char** argv) {
  std::cout << "Main Proc ID: " << getpid() << std::endl;

  int pid = fork();

  if (pid==0)
    std::cout << "I'm a Child Process with ID: " << getpid() 
              << " and parent process ID " << getppid() 
              << std::endl;
  else
    std::cout << "Parent Process after fork with child process ID: " 
              << pid << std::endl;

  return 0;
}
