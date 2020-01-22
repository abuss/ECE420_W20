/* File:  
 *    critical_section_mutex.c
 *
 * Purpose:
 *    Illustrate the use of pthread_mutex to solve the critical section problem 
 *    between executing threads sharing a resource,
 *    each thread will prints a message.
 *
 * Input:
 *    none
 * Output:
 *    message from each thread
 *
 * Compile:  gcc -Wall -o critical_section_mutex_c critical_section_mutex.c -lpthread
 * Usage:    ./critical_section_mutex_c
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h> 
  
// Share counter
int counter = 0;
pthread_mutex_t mutex;

void* incThread(void* data) {
  long n = (long)data;
  for(long i=0;i<n;++i) {
    pthread_mutex_lock(&mutex);    // locking mutex
    printf("t1: %d\n",++counter);
    pthread_mutex_unlock(&mutex);  // unlocking mutex 
  }
  return NULL;
}

void* decThread(void* data) {
  long n = (long)data;
  for(long i=0;i<n;++i) {
    pthread_mutex_lock(&mutex);
    printf("t2: %d\n",--counter);  
    pthread_mutex_unlock(&mutex);
  }
  return NULL;
}

int main() {
  long n = 10000;
  pthread_t t1,t2;

  pthread_mutex_init(&mutex, NULL);

  pthread_create(&t1, NULL, incThread, (void*)n);
  pthread_create(&t2, NULL, decThread, (void*)n);

  pthread_join(t1, NULL);
  pthread_join(t2, NULL);

  pthread_mutex_destroy(&mutex);
  printf("Final counter: %d\n", counter);
  return 0;
}