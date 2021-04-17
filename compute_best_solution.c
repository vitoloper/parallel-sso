#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include "sso.h"

/* TODO
 * - change function to support maximization goal
 * - improve function by assigning OF values to variables
 * - improve final min/max search
 * - write test application
 * - check with valgrind
 * - change Makefile to add this file
 */

/*
 * Movement of shark toward prey: evolution of the SSO algorithm.
 *
 * obj_func: function to optimize
 * goal: MIN_GOAL or MAX_GOAL
 * X: np*nd matrix
 * np: population size
 * nd: number of decision variables
 * best_solution [OUT]: result array of length nd
 * best_val [OUT]: best min/max value
 *
 */
void compute_best_solution(num_t (*obj_func)(num_t *, int), int goal, num_t **X,
                           int np, int nd, num_t *best_solution,
                           num_t *best_val)
{
    num_t **V;              /* velocities */
    num_t **Y;              /* next forward position */
    num_t ***Z;             /* next rotational positions */
    num_t *gradient_result; /* gradient */
    num_t *best_OF_vals;    /* best values calculated from the OF */
    int i;
    int j;
    int k; /* step */
    int m;
    num_t R1;
    num_t R2;
    num_t R3;
    num_t vel, vel_limit;
    int vel_limit_idx;

    /* Allocate space (TODO: check result) */
    allocate_2d_matrix(&V, np, nd);

    /* Allocate space (TODO: check result) */
    allocate_2d_matrix(&Y, np, nd);

    /* Allocate space (TODO: check result) */
    allocate_3d_matrix(&Z, np, M_POINTS, nd);

    best_OF_vals = (num_t *)malloc(np * sizeof(num_t));

    gradient_result = (num_t *)malloc(nd * sizeof(num_t));

    /* Initialize velocities */
    for (i = 0; i < np; i++) {
        for (j = 0; j < nd; j++) {
            V[i][j] = INITIAL_VELOCITY;
        }
    }

    for (k = 0; k < K_MAX; k++) {
        R1 = (num_t)rand() / RAND_MAX; /* [0,1] */
        R2 = (num_t)rand() / RAND_MAX; /* [0,1] */

        /* Each row is a solution of nd decision variables */
        for (i = 0; i < np; i++) {
            /* Compute gradient */
            // print_vector(0, X[i], nd);
            gradient(obj_func, X[i], nd, gradient_result);
            // print_vector(0, gradient_result, nd);

            /* Compute velocities */
            for (j = 0; j < nd; j++) {
                vel =
                    goal * ETA * R1 * gradient_result[j] + ALPHA * R2 * V[i][j];
                vel_limit = BETA * V[i][j];
                vel_limit_idx = min_abs(vel, vel_limit);
                /* Choose the velocity with the smallest abs value */
                if (vel_limit_idx == 0) {
                    V[i][j] = vel;
                } else {
                    V[i][j] = vel_limit;
                }

                /* Set next forward position */
                Y[i][j] = X[i][j] + V[i][j] * DELTA_T;
            }

            /* Set next rotational movement positions (local search) */
            for (m = 0; m < M_POINTS; m++) {
                R3 = (num_t)rand() / RAND_MAX; /* [0,1] */
                R3 = 2 * R3;                   /* [0,2] */
                R3 = R3 - 1;                   /* [-1,1] */

                for (j = 0; j < nd; j++) {
                    Z[i][m][j] = Y[i][j] + R3 * Y[i][j];
                }
            }

            /* Choose the best position for solution i */
            memcpy(X[i], Y[i], nd * sizeof(num_t));
            best_OF_vals[i] = obj_func(Y[i], nd);

            for (m = 0; m < M_POINTS; m++) {
                if (obj_func(Z[i][m], nd) < best_OF_vals[i]) {
                    memcpy(X[i], Z[i][m], nd * sizeof(num_t));
                    best_OF_vals[i] = obj_func(Z[i][m], nd);
                }
            }
        } /* end NP loop */

        // printf("Velocities:\n");
        // print_matrix(0, V, np, nd);
        // print_vector(0, best_OF_vals, np);
    } /* end K_MAX loop */

    /* Choose the best solution among the NP solutions */
    printf("Final solution vectors:\n");
    print_matrix(0, X, np, nd);

    memcpy(best_solution, X[0], nd * sizeof(num_t));
    *best_val = obj_func(best_solution, nd);
    for (i = 1; i < np; i++) {
        if (obj_func(X[1], nd) < *best_val) {
            memcpy(best_solution, X[i], nd * sizeof(num_t));
            *best_val = obj_func(X[1], nd);
        }
    }

    /* Free heap space */
    free_2d_matrix(&V, np);
    free_2d_matrix(&Y, np);
    free_3d_matrix(&Z, np, M_POINTS);
    free(gradient_result);
    free(best_OF_vals);
}
