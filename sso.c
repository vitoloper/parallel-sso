/*
 * Parallel Shark Smell Optimization algorithm implementation
 * Author: Giuseppe Vitolo
 *
 */

#include "sso.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>

#include "mpi.h"

int main(int argc, char *argv[])
{
    int rank;               /* rank */
    int size;               /* number of processes */
    int np;                 /* population size */
    int np_local;           /* population size (local) */
    int tc;                 /* test case to run */
    int i;                  /* loop control variable */
    num_t **X;              /* position (solution) vectors (np*nd matrix) */
    num_t *X_storage;       /* storage for X values (contiguous memory) */
    num_t **X_local;        /* subset of position vectors (local) */
    num_t *X_local_storage; /* storage for X_local values (contiguous) */
    // num_t **X_gathered;        /* solutions gathered from processes */
    // num_t *X_gathered_storage; /* storage for X_gathered values (contiguous)
    // */
    struct tc_params_s tc_params[NUM_OF_TC]; /* Test cases parameters array */
    char *endptr; /* location of the first invalid char (strtol) */
    int *counts;  /* number of elements to send/recv to/from each proc */
    int *displs;  /* array of displacements relative to X_storage */
    MPI_Datatype row_type; /* row datatype */
    double elapsed_time;   /* elapsed time */

    MPI_Init(&argc, &argv);

    /* Start the timer */
    MPI_Barrier(MPI_COMM_WORLD);
    elapsed_time = -MPI_Wtime();

    MPI_Comm_rank(MPI_COMM_WORLD, &rank); /* Get my rank */
    MPI_Comm_size(MPI_COMM_WORLD, &size); /* Get number of processes */

    /* Check the number of arguments passed */
    if (argc != 3) {
        if (rank == 0) {
            print_usage(argv[0]);
        }
        MPI_Finalize();
        exit(EXIT_FAILURE);
    }

    /* Set population size (check strtol errors) */
    errno = 0;
    np = (int)strtol(argv[1], &endptr, 10);
    if (errno != 0 || endptr == argv[1]) {
        if (rank == 0) {
            printf("%s: error: invalid NP parameter\n", argv[0]);
        }
        MPI_Finalize();
        exit(EXIT_FAILURE);
    }

    /* Set test case (check strtol errors) */
    errno = 0;
    tc = (int)strtol(argv[2], &endptr, 10);
    if (errno != 0 || endptr == argv[1]) {
        if (rank == 0) {
            printf("%s: error: invalid TC parameter\n", argv[0]);
        }
        MPI_Finalize();
        exit(EXIT_FAILURE);
    }
    if (tc < 0 || tc >= NUM_OF_TC) {
        if (rank == 0) {
            printf("%s: error: the selected test case does not exist\n",
                   argv[0]);
        }
        MPI_Finalize();
        exit(EXIT_FAILURE);
    }

    /* Check whether there are too many processes */
    if (size > np) {
        if (rank == 0) {
            printf("%s: error: too many processes\n", argv[0]);
        }
        MPI_Finalize();
        exit(EXIT_FAILURE);
    }

    /* Initialize test case parameters array */
    init_tc_params(tc_params);

    /* Set the seed for the pseudo-random number generator */
    srand((unsigned int)time(NULL) + rank);

    /* Process 0: generate np*nd matrix (solution vectors) */
    if (rank == 0) {
        /* Print information */
        printf("NP (population size): %d\n", np);
        printf("TC (test case): %d\n", tc);
        printf("nd (number of decision variables): %d\n", tc_params[tc].nd);
        printf("low (decision variables lower limit): %9.6f\n",
               tc_params[tc].low);
        printf("high (decision variables upper limit): %9.6f\n\n",
               tc_params[tc].high);

        /* Allocate X and X_storage */
        if (allocate_cont_matrix(&X, &X_storage, np, tc_params[tc].nd) == -1) {
            printf("(0): matrix allocation error (X, X_storage)\n");
            fflush(stdout);
            MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
        }

        /* Generate initial solutions within the feasible search domain */
        init_positions(X, np, tc_params[tc].nd, tc_params[tc].low,
                       tc_params[tc].high);

        /* Print initial solution vectors */
        print_matrix(rank, X, np, tc_params[tc].nd);

        /* Set counts and displs */
        counts = (int *)malloc(size * sizeof(int));
        if (counts == NULL) {
            printf("(0): malloc error (sendcounts)\n");
            fflush(stdout);
            MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
        }

        displs = (int *)malloc(size * sizeof(int));
        if (displs == NULL) {
            printf("(0): malloc error (displs)\n");
            fflush(stdout);
            MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
        }

        /* counts[i]: how many rows to send/recv to/from process i */
        /* displs[i]: where send/recv to/from process i should start */
        for (i = 0; i < size; i++) {
            counts[i] = BLOCK_SIZE(i, size, np);
            displs[i] = BLOCK_LOW(i, size, np);
        }
    } /* end if (rank == 0) */

    /* Number of local solutions (local population size) */
    np_local = BLOCK_SIZE(rank, size, np);

    /* Allocate X_local and X_local_storage */
    if(allocate_cont_matrix(&X_local, &X_local_storage, np_local, tc_params[tc].nd) == -1) {
        printf("(%d): matrix allocation error (X_local, X_local_storage)\n", rank);
        MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    }

    /* Initialize row datatype */
    MPI_Type_contiguous(tc_params[tc].nd, NUM_DT, &row_type);
    MPI_Type_commit(&row_type);

    /* Scatter initial solutions */
    MPI_Scatterv(X_storage, counts, displs, row_type, X_local_storage, np_local,
                 row_type, 0, MPI_COMM_WORLD);

    /* Print local initial solutions */
    print_matrix(rank, X_local, np_local, tc_params[tc].nd);

    /***********************************/
    /* NOTE: add in tc_params if it's a min or max problem */
    /* TODO: compute_movement(X_local, np_local, tc_params) */
    /* TODO: mpi_Gatherv */
    /* TODO: process 0 prints solutions */
    /* TODO (maybe): process 0 prints OF min/max found */
    /***********************************/

    

    /* Process 0: gather results from all processes */
    /* TODO: Change. Gather just ONE solution (the best one) from each proc */
    MPI_Gatherv(X_local_storage, np_local, row_type, X_storage, counts, displs,
                row_type, 0, MPI_COMM_WORLD);

    /* Stop the timer (get the total elapsed time) */
    MPI_Barrier(MPI_COMM_WORLD);
    elapsed_time += MPI_Wtime();

    /* Free the datatype */
    MPI_Type_free(&row_type);

    /* Free heap space */
    free(X_local_storage);
    free(X_local);

    if (rank == 0) {
        /* Print final solution vectors */
        print_matrix(rank, X, np, tc_params[tc].nd);

        printf("(0): Total elapsed time (seconds): %8.6f\n", elapsed_time);
        fflush(stdout);

        /* Free heap space */
        free(X);
        free(X_storage);
        free(counts);
        free(displs);
    }

    MPI_Finalize();
    return 0;
}

/* Print usage information */
void print_usage(char *name)
{
    printf("Usage: %s NP TC\n", name);
    printf("NP: population size\n");
    printf("TC: test case\n\n");
    printf("TC is a number which can assume the following values:\n");
    printf("0) Elliptic Paraboloid\n");
    // printf("1) Rastrigin Benchmark Function\n\n");
    fflush(stdout);
}
