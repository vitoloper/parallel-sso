/*
 * Objective functions.
 *
 * (C) 2021 Giuseppe Vitolo
 */

#include <math.h>

#include "sso.h"

/*
 * Elliptic paraboloid function
 *
 * Input parameters
 * - X:  input variables (decision variables) vector
 * - nd:  number of decision variables
 *
 * Return value
 * Function value at a given point
 */
num_t elliptic_paraboloid(num_t *X, int nd)
{
    return X[0] * X[0] - 4 * X[0] * X[1] + 5 * X[1] * X[1] - 4 * X[1] + 3;
}

/*
 * Goldstein-Price function
 *
 * Input parameters
 * - X:  input variables (decision variables) vector
 * - nd:  number of decision variables
 *
 * Return value
 * Function value at a given point
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
 *
 * Input parameters
 * - X:  input variables (decision variables) vector
 * - nd:  number of decision variables
 *
 * Return value
 * Function value at a given point
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

/*
 * Rastrigin function
 *
 * Input parameters
 * - X:  input variables (decision variables) vector
 * - nd:  number of decision variables
 *
 * Return value
 * Function value at a given point
 */
num_t rastrigin(num_t *X, int nd)
{
    int i;
    num_t val = 0;

    for (i = 0; i < nd; i++) {
        val += X[i] * X[i] - 10 * cos(2 * M_PI * X[i]);
    }

    return 10 * nd + val;
}

/*
 * Griewangk function
 *
 * Input parameters
 * - X:  input variables (decision variables) vector
 * - nd:  number of decision variables
 *
 * Return value
 * Function value at a given point
 */
num_t griewangk(num_t *X, int nd)
{
    int i;
    num_t a = 0.0;
    num_t b = 1.0;

    for (i = 0; i < nd; i++) {
        a += (X[i] * X[i]) / (num_t)4000;
    }

    for (i = 0; i < nd; i++) {
        b *= cos(X[i] / sqrt((double)i+1));
    }

    return a - b + 1;
}
