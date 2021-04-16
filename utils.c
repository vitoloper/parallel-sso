#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sso.h"

/* Print an m*n matrix where each element is of num_t type */
void print_matrix(int rank, num_t **matrix, int m, int n)
{
    int i, j;

    for (i = 0; i < m; i++) {
        for (j = 0; j < n; j++) {
            printf("(%d) [%d,%d]: %9.6f ", rank, i, j, matrix[i][j]);
        }
        printf("\n");
    }
}

/* Allocate matrix using a storage for memory contiguity */
int allocate_cont_matrix(num_t ***M, num_t **M_storage, int m, int n)
{
    int i;

    /* Allocate space for matrix storage */
    *M_storage = (num_t *)malloc(m * n * sizeof(num_t));
    if (*M_storage == NULL) {
        return -1;
    }

    /* Allocate space for m pointers to num_t */
    *M = (num_t **)malloc(m * sizeof(num_t *));
    if (*M == NULL) {
        return -1;
    }

    /* Initialize pointers (one pointer per row) */
    for (i = 0; i < m; i++) {
        (*M)[i] = &((*M_storage)[i * n]);
    }

    return 1;
}

/*
 * Compute numerical gradient using central difference approximation.
 *
 * f: function pointer
 * X: parameter array
 * nd: number of decision variables
 * result [OUT]: result vector of length nd
 */
int gradient(num_t (*f)(num_t *, int), num_t *X, int nd, num_t *result)
{
    num_t h;
    int i;
    num_t *current_X_right;
    num_t *current_X_left;

    /* Set differentiation increment */
    h = D_INCR;

    /* Allocate current_X_right */
    current_X_right = (num_t *)malloc(nd * sizeof(num_t));
    if (current_X_right == NULL) {
        return -1;
    }

    /* Allocate current_X_left */
    current_X_left = (num_t *)malloc(nd * sizeof(num_t));
    if (current_X_left == NULL) {
        return -1;
    }

    /* Compute each gradient component */
    for (i = 0; i < nd; i++) {
        memcpy(current_X_right, X, nd * sizeof(num_t));
        memcpy(current_X_left, X, nd * sizeof(num_t));

        current_X_right[i] = X[i] + h;
        current_X_left[i] = X[i] - h;

        result[i] = (f(current_X_right, nd) - f(current_X_left, nd)) / (2 * h);
    }

    /* Free heap space */
    free(current_X_right);
    free(current_X_left);

    return 1;
}
