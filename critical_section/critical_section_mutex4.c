/* File:  
 *    critical_section_mutex4.c
 *
 * Purpose:
 *    Illustrate the use of pthread_mutex to solve the critical section problem 
 *    between executing threads sharing a resource (4 threads),
 *    each thread will prints a message.
 *
 * Input:
 *    none
 * Output:
 *    message from each thread
 *
 * Compile:  gcc -Wall -o critical_section_mutex4_c critical_section_mutex4.c -lpthread
 * Usage:    ./critical_section_mutex4_c
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h> 
  
int counter = 0;
pthread_mutex_t mutex;

void* incThread(void* data) {
  long n = (long)data;
  for(long i=0;i<n;++i) {
    pthread_mutex_lock(&mutex);
    printf("t1: %d\n",++counter);
    pthread_mutex_unlock(&mutex);
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
  long n = 100000;
  pthread_t t1, t2, t3, t4;

  pthread_mutex_init(&mutex, NULL);

  pthread_create(&t1, NULL, incThread, (void*)n);
  pthread_create(&t2, NULL, decThread, (void*)n);
  pthread_create(&t3, NULL, incThread, (void*)n);
  pthread_create(&t4, NULL, decThread, (void*)n);

  pthread_join(t1, NULL);
  pthread_join(t2, NULL);
  pthread_join(t3, NULL);
  pthread_join(t4, NULL);

  pthread_mutex_destroy(&mutex);
  printf("Final counter: %d\n", counter);
  return 0;
}