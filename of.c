/* Objective functions definitions */

#include <math.h>

#include "sso.h"

/*
 * Elliptic paraboloid function
 *
 * X:  array of decision variables
 * nd:  number of decision variables
 */
num_t elliptic_paraboloid(num_t *X, int nd)
{
    return X[0] * X[0] - 4 * X[0] * X[1] + 5 * X[1] * X[1] - 4 * X[1] + 3;
}

/*
 * Goldstein-Price function
 *
 * X:  array of decision variables
 * nd:  number of decision variables
 */
num_t goldstein_price(num_t *X, int nd)
{
    num_t a;
    num_t b;

    a = (1 + ((X[0] + X[1] + 1) * (X[0] + X[1] + 1)) *
                 (19 - 14 * X[0] + 3 * X[0] * X[0] - 14 * X[1] +
                  6 * X[0] * X[1] + 3 * X[1] * X[1]));
    b = (30 + ((2 * X[0] - 3 * X[1]) * (2 * X[0] - 3 * X[1])) *
                  (18 - 32 * X[0] + 12 * X[0] * X[0] + 48 * X[1] -
                   36 * X[0] * X[1] + 27 * X[1] * X[1]));

    return a * b;
}

/*
 * "Flipped" Golstein-Price function
 */
num_t flipped_goldstein_price(num_t *X, int nd)
{
    num_t a;
    num_t b;

    a = (1 + ((X[0] + X[1] + 1) * (X[0] + X[1] + 1)) *
                 (19 - 14 * X[0] + 3 * X[0] * X[0] - 14 * X[1] +
                  6 * X[0] * X[1] + 3 * X[1] * X[1]));
    b = (30 + ((2 * X[0] - 3 * X[1]) * (2 * X[0] - 3 * X[1])) *
                  (18 - 32 * X[0] + 12 * X[0] * X[0] + 48 * X[1] -
                   36 * X[0] * X[1] + 27 * X[1] * X[1]));

    return -1 * a * b;
}
