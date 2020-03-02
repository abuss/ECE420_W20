/* File:       
 *    mpi_bcast.c
 *
 * Purpose:    
 *    A "hello,world" program that uses MPI
 *
 * Compile:    
 *    mpicc -g -Wall -o mpi_bcast mpi_bcast.c
 * Usage:        
 *    mpiexec -n<number of processes> ./mpi_bcast
 *
 * Input:      
 *    None
 * Output:     
 *    A Pi calculated value
 *
 * Algorithm:  
 *    Use of Monte Carlo technique to compute Pi
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

#define N 1E9

int main (int argc, char* argv[])
{
  int        comm_sz;               /* Number of processes    */
  int        my_rank;               /* My process rank        */
  int        result=0, sum=0;
  
  double pi=0.0, begin=0.0, end=0.0, x, y;
    
   /* Start up MPI */
   MPI_Init(NULL, NULL); 

   /* Get the number of processes */
   MPI_Comm_size(MPI_COMM_WORLD, &comm_sz); 

   /* Get my rank among all the processes */
   MPI_Comm_rank(MPI_COMM_WORLD, &my_rank); 
    
  //Synchronize all processes and get the begin time
  MPI_Barrier(MPI_COMM_WORLD);

  begin = MPI_Wtime();
    
  srand((int)time(0));
    
  //Each process will caculate a part of the sum
  for (int i = my_rank; i < N; i += comm_sz) {
    x = rand()/(RAND_MAX+1.0);
    y = rand()/(RAND_MAX+1.0);
    if(x*x + y*y < 1.0)
      result++;
  }
    
  //Sum up all results
  MPI_Reduce(&result, &sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    
  //Synchronize all processes and get the end time
  MPI_Barrier(MPI_COMM_WORLD);
  end = MPI_Wtime();
    
  //Caculate and print PI
  if (my_rank==0) {
    pi = 4*sum/N;
    printf("np=%2d;    Time=%fs;    PI=%0.6f\n", comm_sz, end-begin, pi);
  }
    
  /* Shut down MPI */
  MPI_Finalize(); 

  return 0;
}  /* main */
