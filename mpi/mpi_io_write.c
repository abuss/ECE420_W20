#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>

#include "mpi.h"

#define N 10


int main( int argc, char *argv[] )
{
    int errs = 0;
    int size, rank, *buf, count;
    MPI_File fh;
    MPI_Comm comm;
    MPI_Status status;
 
    MPI_Init( &argc, &argv );
 
    comm = MPI_COMM_WORLD;
    MPI_Comm_size( comm, &size );
    MPI_Comm_rank( comm, &rank );

    MPI_File_open( comm, "test.ord", MPI_MODE_RDWR | MPI_MODE_CREATE, MPI_INFO_NULL, &fh );

    buf = (int*)malloc(N * sizeof(int) );
    for(int i = 0; i < N; ++i)
      buf[i] = rank*N+i;
 
    /* Write to file */
    MPI_File_write_ordered( fh, buf, N, MPI_INT, &status );
    MPI_Get_count( &status, MPI_INT, &count );
    if (count != N) {
        errs++;
        fprintf( stderr, "Wrong count (%d) on write-ordered\n", count );fflush(stderr);
    }
  
    MPI_Offset fsize;
    MPI_File_get_size( fh, &fsize);
    printf("File size: %lld\n", fsize);

    free( buf );
    MPI_File_close( &fh );
 
    MPI_Finalize();
    return errs;
}