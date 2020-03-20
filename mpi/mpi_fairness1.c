#define large 128
#include "mpi.h"
#include <stdio.h>
int main(argc, argv)
int argc;
char **argv;
{
    int rank, size, i, sbuf = 1, cnt;
    
    MPI_Init( &argc, &argv );
    MPI_Comm_rank( MPI_COMM_WORLD, &rank );
    MPI_Comm_size( MPI_COMM_WORLD, &size );
    if (rank == 0) {
        MPI_Request requests[large];
        MPI_Status  statuses[large];
        int         indices[large];
        int         buf[large];
        int         j, ndone;
        
        cnt = (size-1)*100;
        for (i=1; i<size; i++)
            MPI_Irecv( buf+i-1, 1, MPI_INT, i,
                      MPI_ANY_TAG, MPI_COMM_WORLD, &requests[i-1] );
        while(cnt > 0) {
            MPI_Waitsome( size-1, requests, &ndone, indices, statuses );
            for (i=0; i<ndone; i++) {
                // Source is j+1
                j = indices[i];
                printf( "Msg from %d with tag %d\n",
                       statuses[i].MPI_SOURCE,
                       statuses[i].MPI_TAG );
                //printf( "Msg from %d with tag %d\n",
                //       j+1,
                //       statuses[i].MPI_TAG );
                MPI_Irecv( buf+j, 1, MPI_INT, j+1,
                          MPI_ANY_TAG, MPI_COMM_WORLD, &requests[j] );
            }
            cnt -= ndone;
        }
        /* We should really cancel the pending receives */
        for (i=0; i<size-1; i++) 
            MPI_Cancel( &requests[i] );
        // Print the buffer at process 0
        for (i = 0; i<size-1; i++)
            printf("%d\n", buf[i]);
    }
    else {
        for (i=0; i<100; i++) 
            MPI_Send( &sbuf, 1, MPI_INT, 0, i, MPI_COMM_WORLD );
    }
    MPI_Finalize();
    return 0;
}

