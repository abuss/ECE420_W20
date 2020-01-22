/* File:  
 *    critical_section4.cpp
 *
 * Purpose:
 *    Illustrate the critical section problem between executing threads (4 threads) 
 *    sharing a resource
 *    each thread will prints a message.
 *
 * Input:
 *    none
 * Output:
 *    message from each thread
 *
 * Compile:  g++ -Wall -o critical_section4_cpp critical_section4.cpp -lpthread
 * Usage:    ./critical_section4_cpp
 *
 */

#include <iostream>
#include <thread>

int main(int argc, const char** argv) {
  int counter = 0;
  int n = 100000;

  auto t1 = std::thread([&]{for(int i=0;i<n;++i) 
                              std::cout << "t1:" << ++counter << std::endl;});
  auto t2 = std::thread([&]{for(int i=0;i<n;++i) 
                              std::cout << "t2:" << --counter << std::endl;});
  auto t3 = std::thread([&]{for(int i=0;i<n;++i) 
                              std::cout << "t3:" << ++counter << std::endl;});
  auto t4 = std::thread([&]{for(int i=0;i<n;++i) 
                              std::cout << "t4:" << --counter << std::endl;});

/*
  auto t1 = std::thread([&]{for(int i=0;i<n;++i) 
                              ++counter;});
  auto t2 = std::thread([&]{for(int i=0;i<n;++i) 
                              --counter;});
  auto t3 = std::thread([&]{for(int i=0;i<n;++i) 
                              ++counter;});
  auto t4 = std::thread([&]{for(int i=0;i<n;++i) 
                              --counter;});
*/
/*
  auto t1 = std::thread([&]{for(int i=0;i<n;++i) 
                              printf("t1: %d\n",++counter);});
  auto t2 = std::thread([&]{for(int i=0;i<n;++i) 
                              printf("t2: %d\n",--counter);});
  auto t3 = std::thread([&]{for(int i=0;i<n;++i) 
                              printf("t3: %d\n",++counter);});
  auto t4 = std::thread([&]{for(int i=0;i<n;++i) 
                              printf("t4: %d\n",--counter);});
*/

  t1.join();
  t2.join();
  t3.join();
  t4.join();

  std::cout << "Final counter: " << counter << std::endl;
  return 0;
}
