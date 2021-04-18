/* Objective functions definitions */

#include <math.h>

#include "sso.h"

/*
 * X:  array of decision variables
 * nd:  number of decision variables
 */
num_t elliptic_paraboloid(num_t *X, int nd)
{
    return X[0] * X[0] - 4 * X[0] * X[1] + 5 * X[1] * X[1] - 4 * X[1] + 3;
}
