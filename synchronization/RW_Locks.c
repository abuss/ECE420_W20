/*
Created by Di Niu on Jan 26, 2016
Demonstrate the use of Read-Write locks in a 
program to find the minimum value in a list.
*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <pthread.h>
#include "timer.h"

typedef struct {
	int readers;
	int writer;
	pthread_cond_t readers_proceed;
	pthread_cond_t writer_proceed;
	int pending_writers;
	pthread_mutex_t read_write_lock;
} mylib_rwlock_t;

const int MAX_INT  = 10000000;
int partial_list_size;
int min_value;

pthread_mutex_t mutex;
mylib_rwlock_t rwlock; 


void mylib_rwlock_init (mylib_rwlock_t *l) {
	l -> readers = l -> writer = l -> pending_writers = 0;
	pthread_mutex_init(&(l -> read_write_lock), NULL);
	pthread_cond_init(&(l -> readers_proceed), NULL);
	pthread_cond_init(&(l -> writer_proceed), NULL);
}


void mylib_rwlock_rlock(mylib_rwlock_t *l) {
	/* if there is a write lock or pending writers, perform condition wait, 
	   else increment count of readers and grant read lock
	*/
	pthread_mutex_lock(&(l -> read_write_lock));
	while ((l -> pending_writers > 0) || (l -> writer > 0))
		pthread_cond_wait(&(l -> readers_proceed),
			&(l -> read_write_lock));
	l -> readers ++;
	pthread_mutex_unlock(&(l -> read_write_lock));
}

void mylib_rwlock_wlock(mylib_rwlock_t *l) {
	/* if there are readers or writers, increment pending writers count and wait. On being woken, 
	   decrement pending writers count and increment writer count
  */
	pthread_mutex_lock(&(l -> read_write_lock));
	while ((l -> writer > 0) || (l -> readers > 0)) {
		l -> pending_writers ++;
		pthread_cond_wait(&(l -> writer_proceed),
			&(l -> read_write_lock));
    	l -> pending_writers --;
	}
	l -> writer ++;
	pthread_mutex_unlock(&(l -> read_write_lock));
}

void mylib_rwlock_unlock(mylib_rwlock_t *l) {
	/* if there is a write lock then unlock, else if there are read locks, decrement count of read locks. 
		 If the count is 0 and there is a pending writer, let it through, else if there are pending readers, 
		 let them all go through
	*/
	pthread_mutex_lock(&(l -> read_write_lock));
	if (l -> writer > 0)
		l -> writer = 0;
	else if (l -> readers > 0)
		l -> readers --;
	pthread_mutex_unlock(&(l -> read_write_lock));
	if ((l -> readers == 0) && (l -> pending_writers > 0))
		pthread_cond_signal(&(l -> writer_proceed));
	else if (l -> readers > 0)
		pthread_cond_broadcast(&(l -> readers_proceed));
}

void *find_min_rw(void *list_ptr) {
	int *partial_list_pointer, my_min, i;
	my_min = MAX_INT;
	partial_list_pointer = (int *) list_ptr;
	for (i = 0; i < partial_list_size; i++)
		if (partial_list_pointer[i] <= my_min)
			my_min = partial_list_pointer[i];
	// lock the mutex associated with min_value and update the variable as required 
	mylib_rwlock_rlock(&rwlock);
	
	if (my_min < min_value) {
		mylib_rwlock_unlock(&rwlock);
		mylib_rwlock_wlock(&rwlock);
		min_value = my_min;
	}
	
	// and unlock the mutex
	mylib_rwlock_unlock(&rwlock);
	pthread_exit(0);
}


int main(int argc, char* argv[]) {
   int       thread, i;  
   pthread_t* thread_handles;
   double start, finish, elapsed;
   long thread_count = strtol(argv[1], NULL, 10);
   printf("%ld threads\n", thread_count);

	int n = 1000000; // problem size
   	int *list = (int *)malloc(n*sizeof(int));//input array
	for (i = 0; i < n; i++)
		list[i] = 99;
	list[n/3] = 10;
	partial_list_size = n/thread_count;	//problem size for each thread
	min_value = MAX_INT;
	
   thread_handles = (pthread_t*) malloc (thread_count*sizeof(pthread_t)); 
   pthread_mutex_init(&mutex, NULL);
   mylib_rwlock_init(&rwlock);
   
   GET_TIME(start);
   
	 for (thread = 0; thread < thread_count; thread++)  
      pthread_create(&thread_handles[thread], NULL,
          find_min_rw, (void*)&list[thread*partial_list_size]);  

   for (thread = 0; thread < thread_count; thread++) 
      pthread_join(thread_handles[thread], NULL); 
   
	 GET_TIME(finish);
   elapsed = finish - start;
   
	 printf("Time taken: %f seconds\n", elapsed);
   printf("Min value is: %d\n", min_value);

   pthread_mutex_destroy(&mutex);
   free(thread_handles);
   return 0;
}  /* main */
