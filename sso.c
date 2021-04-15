/*
 * Parallel Shark Smell Optimization algorithm implementation
 * Author: Giuseppe Vitolo
 *
 */

#include "sso.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "mpi.h"

int main(int argc, char *argv[])
{
    int rank;         /* rank */
    int size;         /* number of processes */
    int np;           /* population size */
    int tc;           /* test case to run */
    int i;            /* loop control variable */
    num_t low, high;  /* allowed decision variables range */
    num_t **X;        /* position (solution) vectors (np*nd matrix) */
    num_t *X_storage; /* storage for X values (contiguous memory) */
    struct tc_params_s tc_params[NUM_OF_TC]; /* Test cases parameters array */
    double elapsed_time;                     /* elapsed time */

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
        exit(ARGC_ERROR);
    }

    /*TODO: check strtol errno values */
    /* Set population size */
    np = (int)strtol(argv[1], NULL, 10);

    /* TODO: check TC existence */
    /* Set test case */
    tc = (int)strtol(argv[2], NULL, 10);

    /* Initialize test case parameters array */
    init_tc_params(tc_params);

    /* DEBUG */
    // printf("(%d): %d %f %f \n", rank, tc_params[0].nd, tc_params[0].low,
    // tc_params[0].high);

    /* Set the seed for the pseudo-random number generator */
    srand((unsigned int)time(NULL) + rank);

    /* Process 0: generate np*nd matrix */
    if (rank == 0) {
        /* Allocate space for X matrix storage */
        X_storage = (num_t *)malloc(np * tc_params[tc].nd * sizeof(num_t));
        if (X_storage == NULL) {
            printf("(0): malloc error (X_storage)\n");
            fflush(stdout);
            MPI_Abort(MPI_COMM_WORLD, MALLOC_ERROR);
        }

        /* Allocate space for np pointers to num_t */
        X = (num_t **)malloc(np * sizeof(num_t *));
        if (X == NULL) {
            printf("(0): malloc error (X)\n");
            fflush(stdout);
            MPI_Abort(MPI_COMM_WORLD, MALLOC_ERROR);
        }

        /* Initialize pointers */
        for (i = 0; i < np; i++) {
            X[i] = &X_storage[i * tc_params[tc].nd];
        }

        /* Generate initial solutions within the feasible search domain */
        init_positions(X, np, tc_params[tc].nd, tc_params[tc].low,
                       tc_params[tc].high);

        /* Print initial solution vectors */
        print_matrix(X, np, tc_params[tc].nd);
        // fflush(stdout)
    } /* end if (rank == 0) */

    /* TODO
     * Process 0 scatters data among all processes
     *
     * Each process compute its share of solutions:
     * compute_movement(X_local, np_local, tc)
     *
     * Process 0 gathers data from all processes
     * Process 0 prints solution vectors (matrix)
     * Process 0 prints total elapsed time
     */

    /* Stop the timer (get the total elapsed time) */
    MPI_Barrier(MPI_COMM_WORLD);
    elapsed_time += MPI_Wtime();

    if (rank == 0) {
        /* free heap space */
        free(X);
        free(X_storage);

        printf("(0): Total elapsed time (seconds): %8.6f\n", elapsed_time);
        fflush(stdout);
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
    printf("1) Rastrigin Benchmark Function\n\n");
    fflush(stdout);
}
