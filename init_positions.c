#include <stdlib.h>

#include "sso.h"

/* Initialize position vectors with values randomly sampled from the [low, high]
 * interval */
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
