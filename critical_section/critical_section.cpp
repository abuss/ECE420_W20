/* File:  
 *    critical_section.cpp
 *
 * Purpose:
 *    Illustrate the critical section problem between executing threads sharing a resource
 *    each thread will prints a message.
 *
 * Input:
 *    none
 * Output:
 *    message from each thread
 *
 * Compile:  g++ -Wall -o critical_section_cpp critical_section.cpp -lpthread
 * Usage:    ./critical_section_cpp
 *
 */

#include <iostream>
#include <thread>

int main(int argc, const char** argv) {
  int counter = 0;
  int n = 10000;

  auto t1 = std::thread([&]{for(int i=0;i<n;++i) 
                              std::cout << "t1:" << ++counter << std::endl;});
  auto t2 = std::thread([&]{for(int i=0;i<n;++i) 
                              std::cout << "t2:" << --counter << std::endl;});

  t1.join();
  t2.join();

  std::cout << "Final counter: " << counter << std::endl;
  return 0;
}
