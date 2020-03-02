#include <stdio.h>
#include <math.h>
#include "mpi.h"


int main(int argc, char* argv[])
{
  int        comm_sz;               /* Number of processes    */
  int        my_rank;               /* My process rank        */

  double     PI25DT = 3.141592653589793238462643;
  int        n;
  double     mypi, pi, x;
  double     begin, end;
  

  /* Start up MPI */
  MPI_Init(&argc,&argv);

  /* Get the number of processes */
  MPI_Comm_size(MPI_COMM_WORLD, &comm_sz); 

  /* Get my rank among all the processes */
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank); 

  if (my_rank == 0) {
    printf("Enter the number of intervals: \n");
    scanf("%d",&n);
  }

  //Synchronize all processes and get the begin time
  MPI_Barrier(MPI_COMM_WORLD);

  begin = MPI_Wtime();

  /* Broadcast message to all processes */
	MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
  
	double h = 1.0 / n;
	double sum = 0.0;
	for (int i = my_rank; i < n; i += comm_sz) {
    x = h * (i - 0.5);
    sum += 4.0 / (1.0 + x*x);
	}
	mypi = h * sum;
    
	MPI_Reduce(&mypi, &pi, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    
  //Synchronize all processes and get the end time
  MPI_Barrier(MPI_COMM_WORLD);
  end = MPI_Wtime();

	if (my_rank == 0) {
    printf("np=%2d    Time=%fs    PI=%0.10f   Error=%.16f\n",
      comm_sz, end-begin, pi, fabs(pi - PI25DT));
  }

  MPI_Finalize();

  return 0;
}