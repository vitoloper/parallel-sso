/*
 * Movement of shark toward prey: evolution of the SSO algorithm.
 * Find optimal solution.
 *
 * (C) 2021 Giuseppe Vitolo
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include "sso.h"

/*
 * This function computes the best solution for a given objective function.
 *
 * Input parameters
 * - tc_params: test case parameters
 * - X: initial solution vectors (dimensions: np * nd)
 * - np: population size
 *
 * Output parameters
 * - best_solution: optimal solution vector (length: nd)
 * - best_val: objective function value at best_solution
 *
 * Return value
 * It returns -1 if a memory allocation problem occurred.
 * It returns 1 on success.
 */
int compute_best_solution(struct tc_params_s tc_params, num_t **X, int np,
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

    /* Allocate space for V matrix */
    if (allocate_2d_matrix(&V, np, tc_params.nd) == -1) {
        return -1;
    }

    /* Allocate space for Y matrix */
    if (allocate_2d_matrix(&Y, np, tc_params.nd) == -1) {
        return -1;
    }

    /* Allocate space for Z matrix */
    if (allocate_3d_matrix(&Z, np, tc_params.m_points, tc_params.nd) == -1) {
        return -1;
    }

    /* Allocate space for best_OF_vals vector */
    best_OF_vals = (num_t *)malloc(np * sizeof(num_t));
    if (best_OF_vals == NULL) {
        return -1;
    }

    /* Allocate space for gradient_result vector */
    gradient_result = (num_t *)malloc(tc_params.nd * sizeof(num_t));
    if (gradient_result == NULL) {
        return -1;
    }

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
            gradient(tc_params.obj_func, X[i], tc_params.nd, gradient_result);

            /* Compute velocities */
            for (j = 0; j < tc_params.nd; j++) {
                vel = tc_params.goal * tc_params.eta * R1 * gradient_result[j] +
                      tc_params.alpha * R2 * V[i][j];

                /* Compute velocity limit */
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

            /* Set rotational movement positions (local search) */
            for (m = 0; m < tc_params.m_points; m++) {
                R3 = (num_t)rand() / RAND_MAX; /* [0,1] */
                R3 = 2 * R3;                   /* [0,2] */
                R3 = R3 - 1;                   /* [-1,1] */

                for (j = 0; j < tc_params.nd; j++) {
                    Z[i][m][j] = Y[i][j] + R3 * Y[i][j];
                }
            }

            /* Choose the best position for solution i among forward and
             * rotational positions */
            memcpy(X[i], Y[i], tc_params.nd * sizeof(num_t));
            best_OF_vals[i] = tc_params.obj_func(Y[i], tc_params.nd);

            for (m = 0; m < tc_params.m_points; m++) {
                current_OF_val = tc_params.obj_func(Z[i][m], tc_params.nd);

                /* Use tc_params.goal to make comparison work either on both
                 * maximization and minimization goals */
                if (tc_params.goal * current_OF_val >
                    tc_params.goal * best_OF_vals[i]) {
                    memcpy(X[i], Z[i][m], tc_params.nd * sizeof(num_t));
                    best_OF_vals[i] = current_OF_val;
                }
            }
        } /* end NP loop */
    }     /* end K_MAX loop */

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

    return 1;
}
