#include "mpi.h"
#include <stdio.h>

int main(int argc, char* argv[])
{
  int npes, myrank;
  char name[32];
  int name_size;

  MPI_Init(&argc, &argv);

  MPI_Comm_size(MPI_COMM_WORLD, &npes);

  MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

  MPI_Get_processor_name(name, &name_size);

  printf("Hello from process %d out of %d from %s\n", myrank, npes, name);

  MPI_Finalize();
  return 0;
}