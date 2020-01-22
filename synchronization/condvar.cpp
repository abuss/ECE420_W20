/******************************************************************************
 * FILE: condvar.cpp
 * DESCRIPTION:
 *   Example code for using Pthreads condition variables.  The main thread
 *   creates three threads.  Two of those threads increment a "count" variable,
 *   while the third thread watches the value of "count".  When "count"
 *   reaches a predefined limit, the waiting thread is signaled by one of the
 *   incrementing threads. The waiting thread "awakens" and then modifies
 *   count. The program continues until the incrementing threads reach
 *   TCOUNT. The main program prints the final value of count.
 * SOURCE: Adapted from example code in "Pthreads Programming", B. Nichols
 *   et al. O'Reilly and Associates.
 * Code adapted to C++ by Antal Buss
 ******************************************************************************/

#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>


#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_THREADS  3
#define TCOUNT 10
#define COUNT_LIMIT 12

int     count = 0;
// pthread_mutex_t count_mutex;
// pthread_cond_t count_threshold_cv;
std::mutex count_mutex;
std::condition_variable count_threshold_cv;

void inc_count(long my_id)
{
  for (int i=0; i < TCOUNT; ++i) {
    std::unique_lock<std::mutex> lock(count_mutex);
    count++;
        
    /*
      Check the value of count and signal waiting thread when condition is
      reached.  Note that this occurs while mutex is locked.
    */
    if (count == COUNT_LIMIT) {
      printf("inc_count(): thread %ld, count = %d  Threshold reached. ",
              my_id, count);
      count_threshold_cv.notify_one();
      printf("Just sent signal.\n");
    }
    printf("inc_count(): thread %ld, count = %d, unlocking mutex\n",
              my_id, count);
    lock.unlock();
        
    /* Do some work so threads can alternate on mutex lock */
    sleep(1);
  }
}

void watch_count(long my_id)
{
  printf("Starting watch_count(): thread %ld\n", my_id);
    
  /*
    Lock mutex and wait for signal.  Note that the pthread_cond_wait routine
    will automatically and atomically unlock mutex while it waits.
    Also, note that if COUNT_LIMIT is reached before this routine is run by
    the waiting thread, the loop will be skipped to prevent pthread_cond_wait
    from never returning.
  */
  std::unique_lock<std::mutex> lock(count_mutex);  

  while (count < COUNT_LIMIT) {
    printf("watch_count(): thread %ld Count= %d. Going into wait...\n", my_id,count);
    count_threshold_cv.wait(lock);
    printf("watch_count(): thread %ld Condition signal received. Count= %d\n", my_id,count);
  }
    
  printf("watch_count(): thread %ld Updating the value of count...\n", my_id);
  count += 125;
  printf("watch_count(): thread %ld count now = %d.\n", my_id, count);
    
  printf("watch_count(): thread %ld Unlocking mutex.\n", my_id);
}

int main(int argc, char *argv[])
{
    // int i;
    long t1=1, t2=2, t3=3;
    std::thread threads[3];

    threads[0] = std::thread(watch_count, t1);
    threads[1] = std::thread(inc_count, t2);
    threads[2] = std::thread(inc_count, t3);

    /* Wait for all threads to complete */
    for (auto& thread : threads)
      thread.join();
  
    printf("Main(): Waited and joined with %d threads. Final value of count = %d. Done.\n",
            NUM_THREADS, count);
}
