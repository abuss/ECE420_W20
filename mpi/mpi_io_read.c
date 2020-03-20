#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <math.h>

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
    MPI_File_open( comm, "test.ord", MPI_MODE_RDWR | MPI_MODE_CREATE, MPI_INFO_NULL, &fh );
    MPI_Comm_size( comm, &size );
    MPI_Comm_rank( comm, &rank );

    MPI_Offset fsize;
    MPI_File_get_size( fh, &fsize);
    int block_size = ceil((fsize/sizeof(int))/size);
    
    if (rank == 0) {
      printf("File size to read: %lld\n", fsize);
      printf("Block size read: %d (int)\n", block_size);
    }

    buf = (int*)malloc(block_size * sizeof(int) );
    
    /* Set the individual pointer to 0, since we want to use a read_all */
    // MPI_File_seek( fh, 0, MPI_SEEK_SET );
 
    MPI_File_seek_shared( fh, 0, MPI_SEEK_SET );
    for (int i=0; i<block_size; i++) buf[i] = -1;

    MPI_File_read_ordered(fh, buf, block_size, MPI_INT, &status );
    MPI_Get_count( &status, MPI_INT, &count );
    if (count == 0) {
        errs++;
        fprintf( stderr, "%d: buf = %d\n", rank, buf[0] );fflush(stderr);
    }
   
    printf("[%d]: ",rank);
    for(int i = 0; i < count; ++i)
      printf("%d ",buf[i]);
    printf("\n");

    free( buf );
    MPI_File_close( &fh );
 
    MPI_Finalize();
    return errs;
}