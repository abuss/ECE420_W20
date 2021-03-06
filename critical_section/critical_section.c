/* File:  
 *    critical_section.c
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
 * Compile:  gcc -Wall -o critical_section_c critical_section.c -lpthread
 * Usage:    ./critical_section_c
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
  long n = 10000;
  pthread_t t1,t2;

  pthread_create(&t1, NULL, incThread, (void*)n);
  pthread_create(&t2, NULL, decThread, (void*)n);

  pthread_join(t1, NULL);
  pthread_join(t2, NULL);

  printf("Final counter: %d\n", counter);
  return 0;
}