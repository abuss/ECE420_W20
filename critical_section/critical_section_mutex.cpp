/* File:  
 *    critical_section_mutex.cpp
 *
 * Purpose:
 *    Illustrate the use of std::mutex to solve the critical section problem 
 *    between executing threads sharing a resource,
 *    each thread will prints a message.
 *
 * Input:
 *    none
 * Output:
 *    message from each thread
 *
 * Compile:  gcc -Wall -o critical_section_mutex_cpp critical_section_mutex.cpp -lpthread
 * Usage:    ./critical_section_mutex_cpp
 *
 */

#include <iostream>
#include <thread>
#include <mutex>

int main(int argc, const char** argv) {
  int counter = 0;
  int n = 10000;
  std::mutex mtx;

  auto t1 = std::thread([&](int n){for(int i=0;i<n;++i) {
                              mtx.lock();
                              std::cout << "t1:" << ++counter << std::endl;
                              mtx.unlock();
                            } }, n); 
  auto t2 = std::thread([&](int n){for(int i=0;i<n;++i) {
                              mtx.lock();
                              std::cout << "t2:" << --counter << std::endl;
                              mtx.unlock();
                            } }, n); 

  t1.join();
  t2.join();

  std::cout << "Final counter: " << counter << std::endl;
  return 0;
}
