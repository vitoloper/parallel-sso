#include <stdio.h>
#include <stdlib.h>

#include "mpi.h"

int main(int argc, char *argv[])
{
    int rank, size;
    double elapsed_time;

    MPI_Init(&argc, &argv);

    /* Start the timer */
    MPI_Barrier(MPI_COMM_WORLD);
    elapsed_time = -MPI_Wtime();

    MPI_Comm_rank(MPI_COMM_WORLD, &rank); /* Get my rank */
    MPI_Comm_size(MPI_COMM_WORLD, &size); /* Get number of processes */

    printf("I'm process %d\n", rank);

    /* Stop the timer (get the total elapsed time) */
    MPI_Barrier(MPI_COMM_WORLD);
    elapsed_time += MPI_Wtime();

    if (rank == 0) {
        printf("Total elapsed time (s): %8.6f\n", elapsed_time);
        fflush(stdout);
    }

    MPI_Finalize();
    return 0;
}
