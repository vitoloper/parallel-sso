/*
 * Parallel Shark Smell Optimization algorithm implementation.
 *
 * (C) 2021 Giuseppe Vitolo
 */

#include "sso.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>

#include "mpi.h"

int main(int argc, char *argv[])
{
    int rank;                                /* rank */
    int size;                                /* number of processes */
    int np;                                  /* population size */
    int np_local;                            /* population size (local) */
    int tc;                                  /* test case to run */
    struct tc_params_s tc_params[NUM_OF_TC]; /* Test cases parameters array */
    char *endptr;    /* location of the first invalid char (strtol) */
    num_t **X_local; /* local solutions matrix (np_local * nd) */
    num_t *best_solution_local; /* local solution vector (length: nd+1) */
    num_t best_val_local;       /* best objective function value (min or max) */
    num_t *best_solution;       /* best solution vector (length: nd+1)*/
    MPI_Datatype row_result_type; /* row plus one element datatype */
    MPI_Op custom_min_op;         /* custom min reduce operation */
    MPI_Op custom_max_op;         /* custom max reduce operation */
    double elapsed_time;          /* elapsed time */

    MPI_Init(&argc, &argv);

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

    /* Check if there are too many processes */
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

    /* Process 0: print information */
    if (rank == 0) {
        printf("NP (population size): %d\n", np);
        printf("TC (test case): %d\n", tc);
        printf("Goal: %s\n", tc_params[tc].goal == MIN_GOAL ? "minimization"
                                                            : "maximization");
        printf("nd (number of decision variables): %d\n", tc_params[tc].nd);
        printf("low (decision variables lower limit): %9.6f\n",
               tc_params[tc].low);
        printf("high (decision variables upper limit): %9.6f\n\n",
               tc_params[tc].high);
    }

    /* Create datatype */
    MPI_Type_contiguous(tc_params[tc].nd + 1, NUM_DT, &row_result_type);
    MPI_Type_commit(&row_result_type);

    /* Create custom reduce operations */
    MPI_Op_create((MPI_User_function *)&find_min_val, 0, &custom_min_op);
    MPI_Op_create((MPI_User_function *)&find_max_val, 0, &custom_max_op);

    /* Allocate space for local solution vectors (matrix) */
    /* Use BLOCK_SIZE macro to compute how many rows (solution vectors) each
     * process should handle */
    np_local = BLOCK_SIZE(rank, size, np);
    if (allocate_2d_matrix(&X_local, np_local, tc_params[tc].nd) == -1) {
        printf("(%d): matrix allocation error (X_local)\n", rank);
        MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    }

    /* Allocate space for local best solution vector */
    best_solution_local =
        (num_t *)malloc((tc_params[tc].nd + 1) * sizeof(num_t));
    if (best_solution_local == NULL) {
        printf("(%d): vector allocation error (best_solution_local)\n", rank);
        MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    }

    /* Allocate space for best solution vector (only significant at root) */
    best_solution = (num_t *)malloc((tc_params[tc].nd + 1) * sizeof(num_t));
    if (best_solution == NULL) {
        printf("(%d): vector allocation error (best_solution)\n", rank);
        MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    }

    /* Start the timer */
    MPI_Barrier(MPI_COMM_WORLD);
    elapsed_time = -MPI_Wtime();

    /* Initialize local solution vectors */
    init_positions(X_local, np_local, tc_params[tc].nd, tc_params[tc].low,
                   tc_params[tc].high);

    /* Print initial solution matrix (each process) */
    // print_matrix(rank, X_local, np_local, tc_params[tc].nd);

    /* Compute best solution */
    compute_best_solution(tc_params[tc], X_local, np_local, best_solution_local,
                          &best_val_local);

    /* Put best_val_local in the last vector position */
    best_solution_local[tc_params[tc].nd] = best_val_local;

    /* Print best solution and best objective function value (each process) */
    // print_vector(rank, best_solution_local, tc_params[tc].nd + 1);

    /* Use MPI_Reduce to get the solution vector and the best OF value */
    if (tc_params[tc].goal == MIN_GOAL) {
        MPI_Reduce(best_solution_local, best_solution, 1, row_result_type,
                   custom_min_op, 0, MPI_COMM_WORLD);
    } else {
        MPI_Reduce(best_solution_local, best_solution, 1, row_result_type,
                   custom_max_op, 0, MPI_COMM_WORLD);
    }

    /* Stop the timer (get the total elapsed time) */
    MPI_Barrier(MPI_COMM_WORLD);
    elapsed_time += MPI_Wtime();

    /* Process 0: print result vector, OF value and total elapsed time */
    if (rank == 0) {
        printf("Final solution vector: ");
        print_vector(0, best_solution, tc_params[tc].nd);
        printf("Best objective function value: %f\n",
               best_solution[tc_params[tc].nd]);
        printf("Total elapsed time (seconds): %8.6f\n", elapsed_time);
        fflush(stdout);
    }

    /* Free datatype */
    MPI_Type_free(&row_result_type);

    /* Free function handlers */
    MPI_Op_free(&custom_min_op);
    MPI_Op_free(&custom_max_op);

    /* Free heap space */
    free(best_solution_local);
    free_2d_matrix(&X_local, np_local);
    free(best_solution);

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
    printf("1) Goldstein-Price function\n");
    printf("2) \"Flipped\" Goldstein-Price function\n");
    printf("3) Rastrigin function (two decision variables)\n");
    printf("4) Rastrigin function (five decision variables)\n");
    printf("5) Griewangk function (two decision variables)\n");
    printf("6) Griewangk function (five decision variables)\n\n");
    fflush(stdout);
}
