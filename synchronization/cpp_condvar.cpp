/* File:     cpp_condvar.cpp
 *
 * Purpose:  Illustrate a synchronization of three threads using conditional variables 
 *           each of which creates and prints a message.
 *
 * Input:    none
 * Output:   message from each thread
 *
 * Compile:  g++ -g -Wall -o cpp_condvar cpp_condvar.cpp -lpthread
 * Usage:    cpp_condvar
 *
 */

#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

#include <unistd.h>

int count;
std::mutex ready_mutex;
std::condition_variable ready_cv;

constexpr int n = 10;

void countDown(int n) {
  std::cout << "Starting count down\n";
  std::unique_lock<std::mutex> lock(ready_mutex);
  for(count = n; count > 0; --count) {
    std::cout << count << "..." << std::flush;
    sleep(1);
  }
  ready_cv.notify_one();
  std::cout << "0 -> ";
}

void launch() {
  std::cout << "Ready to launch, waiting for count down\n";
  std::unique_lock<std::mutex> lock(ready_mutex); 
  ready_cv.wait(lock, []{return count==0;});
  std::cout << "LUNCHING!!!\n";
} 

void telemetry() {
  std::cout << "Ready telemetry, waiting for count down\n";
  std::unique_lock<std::mutex> lock(ready_mutex); 
  ready_cv.wait(lock, []{return count==0;});
  std::cout << "\nStarting Telemetry\n";
} 

int main(int argc, const char** argv) {

  auto t1 = std::thread(countDown, n);
  auto t2 = std::thread(launch);
  auto t3 = std::thread(telemetry);

  t1.join();
  t2.join();
  t3.join();

  std::cout << "Done\n";

  return 0;
}