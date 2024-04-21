#include <mpi.h> //include MPI header to have access to MPI functions.
#include <stdio.h>

int main( int argc, char **argv ) {
    int rank, size;                                     
    MPI_Init( &argc, &argv );               //initialize the MPI execution environment.
    MPI_Comm_rank( MPI_COMM_WORLD, &rank ); //Each MPI process is assigned a unique integer ID starting at 0. This function retrieves the local ID and stores it in the local variable rank.
    MPI_Comm_size( MPI_COMM_WORLD, &size ); //retrieves the total number of MPI processes running and stores it in the local variable size.

    //Message Passing Example
    if ( rank == 0 ) {
        /* Rank 0 sends an integer to each of the other process ranks */
        int i;
        int value = 0;
        for (i = 1; i < size; i++) {
            value = value + i;
            MPI_Send(&value, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
    }
    else {
        /* All other process ranks receive one number from Rank 0 */
        int value;
        MPI_Status status;
        MPI_Recv(&value, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        printf("Rank %d received value %d\n", rank, value);
    }


    MPI_Finalize( ); //This must be done before the end of the program.

    return 0;
}
