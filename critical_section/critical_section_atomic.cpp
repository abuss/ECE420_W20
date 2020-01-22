/* File:  
 *    critical_section_atomic.cpp
 *
 * Purpose:
 *    Illustrate use of std::atomic to synchronize thread execution
 *    each thread will prints a message.
 *
 * Input:
 *    none
 * Output:
 *    message from each thread
 *
 * Compile:  g++ -Wall -o critical_section_atomic critical_section_atomic.cpp -lpthread
 * Usage:    ./critical_section_atomic
 *
 */

#include <iostream>
#include <thread>
#include <atomic>

int main(int argc, const char** argv) {
  std::atomic<int> counter {0};
  int n = 100000;

  auto t1 = std::thread([&]{for(int i=0;i<n;++i) 
                              std::cout << "t1:" << ++counter << std::endl;});
  auto t2 = std::thread([&]{for(int i=0;i<n;++i) 
                              std::cout << "t2:" << --counter << std::endl;});

  t1.join();
  t2.join();

/*
  auto t1 = std::thread([&]{for(int i=0;i<n;++i) 
                              printf("t1: %d\n",++counter);});
  auto t2 = std::thread([&]{for(int i=0;i<n;++i) 
                              printf("t2: %d\n",--counter);});
  auto t3 = std::thread([&]{for(int i=0;i<n;++i) 
                              printf("t3: %d\n",++counter);});
  auto t4 = std::thread([&]{for(int i=0;i<n;++i) 
                              printf("t4: %d\n",--counter);});

  t1.join();
  t2.join();
  t3.join();
  t4.join();
*/

  std::cout << "Final counter: " << counter << std::endl;
  return 0;
}
