#include "sso.h"

/* Initialize position vectors */
void init_positions(num_t **X, int np, int nd) {
    int i,j;

    for (i = 0; i < np; i++) {
        for (j = 0; j < nd; j++) {
            /* TODO: generate feasible solutions */
            X[i][j] = 10.10;            
        }
    }
}
