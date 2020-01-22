/* File:  
 *    critical_section4.c
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
 * Compile:  gcc -Wall -o critical_section4_c critical_section4.c -lpthread
 * Usage:    ./critical_section4_c
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h> 
  
int counter = 0;

void* incThread(void* data) {
  long n = (long)data;
  for(long i=0;i<n;++i) 
    printf("t1: %d\n",++counter);
  return NULL;
}

void* decThread(void* data) {
  long n = (long)data;
  for(long i=0;i<n;++i) 
    printf("t2: %d\n",--counter);  
  return NULL;
}

int main() {
  long n = 100000;
  pthread_t t1,t2,t3,t4;

  pthread_create(&t1, NULL, incThread, (void*)n);
  pthread_create(&t2, NULL, decThread, (void*)n);
  pthread_create(&t3, NULL, incThread, (void*)n);
  pthread_create(&t4, NULL, decThread, (void*)n);

  pthread_join(t1, NULL);
  pthread_join(t2, NULL);
  pthread_join(t3, NULL);
  pthread_join(t4, NULL);

  printf("Final counter: %d\n", counter);
  return 0;
}