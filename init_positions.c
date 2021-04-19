/*
 * Initial solution vectors generation.
 *
 * (C) 2021 Giuseppe Vitolo
 */

#include <stdlib.h>

#include "sso.h"

/*
 * This function initializes the NP solution vectors with values randomly
 * sampled from a [low, high] interval.
 *
 * Input parameters
 * - np: population size
 * - nd: number of decision variables
 * - low: lowest sampled value
 * - high: highest sampled value
 *
 * Output parameters
 * - X: initial solution matrix (dimensions: np * nd)
 */
void init_positions(num_t **X, int np, int nd, num_t low, num_t high)
{
    int i, j;

    for (i = 0; i < np; i++) {
        for (j = 0; j < nd; j++) {
            X[i][j] = (num_t)rand() / RAND_MAX; /* [0,1] */
            X[i][j] = (high - low) * X[i][j];   /* [0, high - low] */
            X[i][j] = X[i][j] + low;            /* [low, high] */
        }
    }
}
