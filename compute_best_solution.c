#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include "sso.h"

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
void compute_best_solution(struct tc_params_s tc_params, num_t **X, int np,
                           num_t *best_solution, num_t *best_val)
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
    num_t current_OF_val;

    /* Allocate space (TODO: check result) */
    allocate_2d_matrix(&V, np, tc_params.nd);

    /* Allocate space (TODO: check result) */
    allocate_2d_matrix(&Y, np, tc_params.nd);

    /* Allocate space (TODO: check result) */
    allocate_3d_matrix(&Z, np, tc_params.m_points, tc_params.nd);

    best_OF_vals = (num_t *)malloc(np * sizeof(num_t));

    gradient_result = (num_t *)malloc(tc_params.nd * sizeof(num_t));

    /* Initialize velocities */
    for (i = 0; i < np; i++) {
        for (j = 0; j < tc_params.nd; j++) {
            V[i][j] = tc_params.initial_velocity;
        }
    }

    for (k = 0; k < tc_params.k_max; k++) {
        R1 = (num_t)rand() / RAND_MAX; /* [0,1] */
        R2 = (num_t)rand() / RAND_MAX; /* [0,1] */

        /* Each row is a solution of nd decision variables */
        for (i = 0; i < np; i++) {
            /* Compute gradient */
            // print_vector(0, X[i], nd);
            gradient(tc_params.obj_func, X[i], tc_params.nd, gradient_result);
            // printf("k = %d, NP = %d  ", k, i);
            // print_vector(0, gradient_result, tc_params.nd);

            /* Compute velocities */
            for (j = 0; j < tc_params.nd; j++) {
                vel = tc_params.goal * tc_params.eta * R1 * gradient_result[j] +
                      tc_params.alpha * R2 * V[i][j];

                vel_limit = tc_params.beta * V[i][j];
                vel_limit_idx = min_abs(vel, vel_limit);

                /* Choose the velocity with the smallest abs value */
                if (vel_limit_idx == 0) {
                    V[i][j] = vel;
                } else {
                    V[i][j] = vel_limit;
                }

                /* Set forward movement */
                Y[i][j] = X[i][j] + V[i][j] * tc_params.delta_t;
            }

            // printf("velocities = ");
            // print_vector(0, V[i], tc_params.nd);
            // printf("forward position = ");
            // print_vector(0, Y[i], tc_params.nd);

            /* Set next rotational movement positions (local search) */
            for (m = 0; m < tc_params.m_points; m++) {
                R3 = (num_t)rand() / RAND_MAX; /* [0,1] */
                R3 = 2 * R3;                   /* [0,2] */
                R3 = R3 - 1;                   /* [-1,1] */

                for (j = 0; j < tc_params.nd; j++) {
                    Z[i][m][j] = Y[i][j] + R3 * Y[i][j];
                }

                // printf("rot position = ");
                // print_vector(0, Z[i][m], tc_params.nd);
            }

            /* Choose the best position for solution i */
            memcpy(X[i], Y[i], tc_params.nd * sizeof(num_t));
            best_OF_vals[i] = tc_params.obj_func(Y[i], tc_params.nd);

            for (m = 0; m < tc_params.m_points; m++) {
                current_OF_val = tc_params.obj_func(Z[i][m], tc_params.nd);

                if (tc_params.goal * current_OF_val >
                    tc_params.goal * best_OF_vals[i]) {
                    memcpy(X[i], Z[i][m], tc_params.nd * sizeof(num_t));
                    best_OF_vals[i] = current_OF_val;
                }
            }
            // printf("\n");
        } /* end NP loop */

        // printf("Velocities:\n");
        // print_matrix(0, V, np, nd);
        // print_vector(0, best_OF_vals, np);
    } /* end K_MAX loop */

    // printf("Final solution vectors:\n");
    // print_matrix(0, X, np, tc_params.nd);

    /* Choose the best solution among the NP solutions */
    memcpy(best_solution, X[0], tc_params.nd * sizeof(num_t));
    *best_val = tc_params.obj_func(best_solution, tc_params.nd);

    for (i = 1; i < np; i++) {
        current_OF_val = tc_params.obj_func(X[i], tc_params.nd);

        if (tc_params.goal * current_OF_val > tc_params.goal * (*best_val)) {
            memcpy(best_solution, X[i], tc_params.nd * sizeof(num_t));
            *best_val = current_OF_val;
        }
    }

    /* Free heap space */
    free_2d_matrix(&V, np);
    free_2d_matrix(&Y, np);
    free_3d_matrix(&Z, np, tc_params.m_points);
    free(gradient_result);
    free(best_OF_vals);
}
