/* 
 * The 2D matrix and vector multiplication
 * Read the data from the Matrix and RHS files
 * And then split the matrix into blocks, the RHS is devied into first process of each row processes.
 * To compile the code: mpicc -g -Wall -o main 2D_Matrix_vector.c -lm
 * To run the code:     mpirun -np 4 ./main
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <mpi.h>

void Loadinput_Matrix(int *n, int *local_n, double **local_A,int my_rank,int comm_size, MPI_Comm comm);

void Loadinput_RHS_vector(int n, int local_n, double **local_b, int my_rank, int comm_size, MPI_Comm comm);

void MatrixVectorMultiply_2D(int n, double *a, double *b, double *x,MPI_Comm comm);

int SaveOutput(int n, int local_n, int my_rank, MPI_Comm comm, double* local_x, double Time);

/*-------------------------------------------------------------------*/
int main(void) {
   double* local_A;
   double* local_b;
   int n, local_n;
   int my_rank, comm_sz,my2drank;
   
   MPI_Comm comm,comm_2d;
   int ROW=0, COL=1;
   int dims[2], periods[2];

   
   double start, finish, loc_elapsed, elapsed;
   MPI_Init(NULL, NULL);
   comm = MPI_COMM_WORLD;
   MPI_Comm_size(comm, &comm_sz);
   MPI_Comm_rank(comm, &my_rank);
   
   dims[ROW]= dims[COL] = sqrt(comm_sz);
   periods[ROW] = periods[COL] = 1;
   MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periods, 1, &comm_2d);
   MPI_Comm_rank(comm_2d, &my2drank); /* Get my rank in the new topology */
   
   start = MPI_Wtime();
   /*Load matrix A from file data_input_matrix */
   Loadinput_Matrix(&n, &local_n, &local_A, my_rank, comm_sz, comm);
   /*Load vector b from file data_input_RHS */
   Loadinput_RHS_vector(n, local_n, &local_b, my_rank, comm_sz, comm);
   
   double *local_x;
   local_x = malloc(local_n*sizeof(double));
   MatrixVectorMultiply_2D( n,  local_A,  local_b,  local_x, comm);

   finish = MPI_Wtime();
   loc_elapsed = finish-start;
   MPI_Reduce(&loc_elapsed, &elapsed, 1, MPI_DOUBLE, MPI_MAX, 0, comm);
   if (my_rank == 0)
      printf("Elapsed time = %e\n", elapsed);
   
   SaveOutput(n, local_n, my_rank, comm, local_x, elapsed);
   
   free(local_A);
   free(local_b);
   free(local_x);
   MPI_Finalize();
   return 0;
}  /* main */

void Loadinput_Matrix(int *n, int *local_n, double **local_A,int my_rank,int comm_size, MPI_Comm comm)
{
    FILE* ip;
    int i,j;
    double* A = NULL;
    int npes;
    MPI_Comm_size(comm, &npes);
    int ROW=0, COL=1;
    int dims[2];
    dims[ROW]= dims[COL] = sqrt(npes);
    
    if (my_rank == 0) {
        if ((ip = fopen("data_input_matrix","r")) == NULL){
            printf("error opening the input data.\n");
        }
        fscanf(ip, "%d\n", n);
        
        A = malloc((*n)*(*n)*sizeof(double));
        for (i = 0; i < *n; ++i){
            for (j = 0; j< *n; ++j){
                fscanf(ip, "%lf\t", &A[i*(*n)+j]);
            }
        }
        fclose(ip);
    }
    MPI_Bcast(n, 1, MPI_INT, 0, comm);
    *local_n = *n/dims[ROW];
    *local_A = malloc((*local_n)*(*local_n)*sizeof(double));
    
    int COLS = *n;
    const int NPROWS=*local_n;  /* number of rows in _decomposition_ */
    const int NPCOLS=*local_n;  /* number of cols in _decomposition_ */
    const int BLOCKROWS = NPROWS;  /* number of rows in _block_ */
    const int BLOCKCOLS = NPCOLS; /* number of cols in _block_ */
    MPI_Datatype blocktype;
    MPI_Datatype blocktype2;

    MPI_Type_vector(BLOCKROWS, BLOCKCOLS, COLS, MPI_DOUBLE, &blocktype2);
    MPI_Type_create_resized( blocktype2, 0, sizeof(double), &blocktype);
    MPI_Type_commit(&blocktype);
    
    int ii,jj;
    int disps[NPROWS*NPCOLS];
    int counts[NPROWS*NPCOLS];
    for (ii=0; ii<NPROWS; ii++) {
        for (jj=0; jj<NPCOLS; jj++) {
            disps[ii*NPCOLS+jj] = ii*COLS*BLOCKROWS+jj*BLOCKCOLS;
            counts [ii*NPCOLS+jj] = 1;
        }
    }
    MPI_Scatterv(A, counts, disps, blocktype, *local_A, BLOCKROWS*BLOCKCOLS, MPI_DOUBLE, 0, MPI_COMM_WORLD);
}

void Loadinput_RHS_vector(int n, int local_n, double **local_b, int my_rank, int comm_size, MPI_Comm comm)
{
    FILE* ip;
    int i;
    double* b = NULL;
    int ROW=0, COL=1;
    int nlocal;
    int npes, dims[2], periods[2], keep_dims[2];
    int myrank, my2drank, mycoords[2];
    MPI_Comm comm_2d, comm_row,comm_col;
    MPI_Comm_size(comm, &npes);
    MPI_Comm_rank(comm, &myrank);
    
    dims[ROW]= dims[COL] = sqrt(npes);
    
    nlocal = n/dims[ROW];
    *local_b = malloc(nlocal*sizeof(double));
    
    periods[ROW] = periods[COL] = 1;
    //Create a 2D Cartesian topology and get the rank and coordinates of the process
    MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periods, 1, &comm_2d);
    
    MPI_Comm_rank(comm_2d, &my2drank); /* Get my rank in the new topology */
    
    MPI_Cart_coords(comm_2d, my2drank, 2, mycoords); /* Get my coordinates */
    
    /* Create the row-based sub-topology*/
    keep_dims[ROW] = 0;
    keep_dims[COL] = 1; /* Column is still connected*/
    
    MPI_Cart_sub(comm_2d, keep_dims, &comm_row); /*Each row get its own row communicator*/
    /* Create the column-based sub-topology*/
    keep_dims[ROW] = 1;
    keep_dims[COL] = 0;
    
    MPI_Cart_sub(comm_2d, keep_dims, &comm_col);
    
    if (my2drank == 0) {
        if ((ip = fopen("data_input_RHS","r")) == NULL){
            printf("error opening the input data.\n");
        }
        b = malloc(n*sizeof(double));
        for (i = 0; i < n; ++i){
            fscanf(ip, "%lf\t", &b[i]);
        }
        fclose(ip);
    }
    
    if (my2drank==0)
    {
        MPI_Scatter(b, nlocal, MPI_DOUBLE, 
                    *local_b, nlocal, MPI_DOUBLE, 0, comm_col);
    }
    
    else if (mycoords[COL]==0)  {
        MPI_Scatter(b, nlocal, MPI_DOUBLE, 
                    *local_b, nlocal, MPI_DOUBLE, 0, comm_col);
    }
    
}

void MatrixVectorMultiply_2D(int n, double *a, double *b, double *x,MPI_Comm comm)
{
    int ROW=0, COL=1; /* Improve readability */
    
    int i, j, nlocal;
    
    double *px; /* Will store partial dot products */
    
    int npes, dims[2], periods[2], keep_dims[2];
    
    int myrank, my2drank, mycoords[2];
    
    int other_rank, coords[2];
    
    MPI_Status status;
    
    MPI_Comm comm_2d, comm_row, comm_col;
    
    MPI_Comm_size(comm, &npes);
    MPI_Comm_rank(comm, &myrank);
    
    dims[ROW]= dims[COL] = sqrt(npes);
    
    nlocal = n/dims[ROW];
    
    px = malloc(nlocal*sizeof(double));
    
    periods[ROW] = periods[COL] = 1;
    //Create a 2D Cartesian topology and get the rank and coordinates of the process
    MPI_Cart_create(comm, 2, dims, periods, 1, &comm_2d);
    
    MPI_Comm_rank(comm_2d, &my2drank); /* Get my rank in the new topology */
    
    MPI_Cart_coords(comm_2d, my2drank, 2, mycoords); /* Get my coordinates */
    
    /* Create the row-based sub-topology*/
    keep_dims[ROW] = 0;
    keep_dims[COL] = 1; /* Column is still connected*/
    
    MPI_Cart_sub(comm_2d, keep_dims, &comm_row); /*Each row get its own row communicator*/
    /* Create the column-based sub-topology*/
    keep_dims[ROW] = 1;
    keep_dims[COL] = 0;
    
    MPI_Cart_sub(comm_2d, keep_dims, &comm_col);
    
    
    
    /* the vector b is in the first column nlocal is the last column*/
    if (mycoords[COL] == 0 && mycoords[ROW] != 0) {
        coords[ROW] = mycoords[ROW]; /* coords[0] = mycoords[0], mycoords[0] = 1 */
        coords[COL] = mycoords[ROW];
        MPI_Cart_rank(comm_2d, coords, &other_rank); /* other rank is the place you need to send*/
        MPI_Send(b, nlocal, MPI_DOUBLE, other_rank, 1, comm_2d);
    }
    if (mycoords[ROW] == mycoords[COL] && mycoords[ROW] != 0) {
        coords[ROW] = mycoords[ROW];
        coords[COL] = 0;
        MPI_Cart_rank(comm_2d, coords, &other_rank);
        MPI_Recv(b, nlocal, MPI_DOUBLE, other_rank, 1, comm_2d, &status);
    }
    
    
    coords[0] = mycoords[COL];
    MPI_Cart_rank(comm_col, coords, &other_rank);
    MPI_Bcast(b, nlocal, MPI_DOUBLE, other_rank, comm_col);
    
    for (i=0; i<nlocal; i++) { 
        px[i] = 0.0;
        for (j=0; j<nlocal; j++){
            px[i] += a[i*nlocal+j]*b[j];
        }
    }
    coords[0] = mycoords[ROW];; /*In the course coords[0] = 0; */
    MPI_Cart_rank(comm_row, coords, &other_rank);
    MPI_Reduce(px, x, nlocal, MPI_DOUBLE, MPI_SUM, other_rank, comm_row);
    MPI_Comm_free(&comm_2d); /* Free up communicator */ 
    MPI_Comm_free(&comm_row); /* Free up communicator */ 
    MPI_Comm_free(&comm_col); /* Free up communicator */
    
    free(px);
}

int SaveOutput(int n, int local_n, int my_rank, MPI_Comm comm, double* local_x, double Time){
    int i;
    int ROW=0, COL=1;
    int npes, dims[2], periods[2], keep_dims[2];
    int myrank, my2drank, mycoords[2];
    MPI_Comm comm_2d, comm_row,comm_col;
    MPI_Comm_size(comm, &npes);
    MPI_Comm_rank(comm, &myrank);
    dims[ROW]= dims[COL] = sqrt(npes);
    periods[ROW] = periods[COL] = 1;
    //Create a 2D Cartesian topology and get the rank and coordinates of the process
    MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periods, 1, &comm_2d);
    
    MPI_Comm_rank(comm_2d, &my2drank); /* Get my rank in the new topology */
    
    MPI_Cart_coords(comm_2d, my2drank, 2, mycoords); /* Get my coordinates */
    
    /* Create the row-based sub-topology*/
    keep_dims[ROW] = 0;
    keep_dims[COL] = 1; /* Column is still connected*/
    
    MPI_Cart_sub(comm_2d, keep_dims, &comm_row); /*Each row get its own row communicator*/
    /* Create the column-based sub-topology*/
    keep_dims[ROW] = 1;
    keep_dims[COL] = 0;
    
    MPI_Cart_sub(comm_2d, keep_dims, &comm_col);
    if (mycoords[COL] == mycoords[ROW] )
    {
      for (i = 0; i < local_n; ++i){
          printf("the local x is %f the rank is %d \n",local_x[i],my2drank );
      }
    }
    return 0;
}