/*
 * Utility functions.
 *
 * (C) 2021 Giuseppe Vitolo
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "sso.h"

/*
 * This function prints an m*n matrix where each element is of num_t type.
 *
 * Input parameters
 * - rank: process rank
 * - matrix: the matrix to display
 * - m: number of rows
 * - n: number of columns
 */
void print_matrix(int rank, num_t **matrix, int m, int n)
{
    int i, j;

    for (i = 0; i < m; i++) {
        for (j = 0; j < n; j++) {
            printf("(%d) [%d,%d]: %9.6f ", rank, i, j, matrix[i][j]);
        }
        putchar('\n');
    }
}

/*
 * This function prints a vector of length 'length'.
 *
 * Input parameters
 * - rank: process rank
 * - v: the vector to display
 * - length: vector length
 */
void print_vector(int rank, num_t *v, int length)
{
    int i;

    for (i = 0; i < length; i++) {
        printf("(%d): %9.6f ", rank, v[i]);
    }
    putchar('\n');
}

/*
 * This function allocates contiguous memory space for a matrix.
 *
 * Input parameters
 * - m: number of rows
 * - n: number of columns
 *
 * Output parameters
 * - M: matrix
 * - M_storage: memory storage space
 *
 * Return value
 * It returns -1 if a memory allocation problem occurred.
 * It returns 1 on success.
 */
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
 * This function allocates memory space for a 2d matrix.
 *
 * Input parameters
 * - m: number of rows
 * - n: number of columns
 *
 * Output parameters
 * - M: matrix
 *
 * Return value
 * It returns -1 if a memory allocation problem occurred.
 * It returns 1 on success.
 */
int allocate_2d_matrix(num_t ***M, int m, int n)
{
    int i;

    *M = (num_t **)malloc(m * sizeof(num_t *));
    if (*M == NULL) {
        return -1;
    }

    for (i = 0; i < m; i++) {
        (*M)[i] = (num_t *)malloc(n * sizeof(num_t));
        if ((*M)[i] == NULL) {
            return -1;
        }
    }

    return 1;
}

/*
 * This function frees space allocated for a 2d matrix.
 *
 * Input parameters
 *  - M: matrix that is freed
 *  - m: number of rows
 */
void free_2d_matrix(num_t ***M, int m)
{
    int i;

    for (i = 0; i < m; i++) {
        free((*M)[i]);
    }

    free(*M);
}

/*
 * This function allocates memory space for a 3d matrix.
 *
 * Input parameters
 * - m: first dimension
 * - n: second dimension
 * - p: third dimension
 *
 * Output parameters
 * - M: matrix
 *
 * Return value
 * It returns -1 if a memory allocation problem occurred.
 * It returns 1 on success.
 */
int allocate_3d_matrix(num_t ****M, int m, int n, int p)
{
    int i, j;

    *M = (num_t ***)malloc(m * sizeof(num_t **));
    if (*M == NULL) {
        return -1;
    }

    for (i = 0; i < m; i++) {
        (*M)[i] = (num_t **)malloc(n * sizeof(num_t *));
        if ((*M)[i] == NULL) {
            return -1;
        }
        for (j = 0; j < n; j++) {
            (*M)[i][j] = (num_t *)malloc(p * sizeof(num_t));
            if ((*M)[i][j] == NULL) {
                return -1;
            }
        }
    }

    return 1;
}

/*
 * This function frees space allocated for a 3d matrix.
 *
 * Input parameters
 * - M: matrix that is freed
 * - m: first dimension
 * - p:  second dimension
 */
void free_3d_matrix(num_t ****M, int m, int n)
{
    int i, j;

    for (i = 0; i < m; i++) {
        for (j = 0; j < n; j++) {
            free((*M)[i][j]);
        }
        free((*M)[i]);
    }

    free(*M);
}

/*
 * This function computes the numerical gradient of a function at a given point
 * using central difference approximation.
 *
 * Input parameters
 * - f: function
 * - X: input variables (decision variables) vector
 * - nd: number of decision variables
 *
 * Output parameters
 * -result: computed gradient
 *
 * Return value
 * It returns -1 if a memory allocation error occurred.
 * It retuns 1 on success.
 */
int gradient(num_t (*f)(num_t *, int), num_t *X, int nd, num_t *result)
{
    int i;
    num_t *current_X_right; /* x + h*/
    num_t *current_X_left;  /* x - h*/

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

        current_X_right[i] = X[i] + D_INCR;
        current_X_left[i] = X[i] - D_INCR;

        result[i] = (f(current_X_right, nd) - f(current_X_left, nd)) / (2.0 * D_INCR);
    }

    /* Free heap space */
    free(current_X_right);
    free(current_X_left);

    return 1;
}

/*
 * This function returns 0 if a is lower than b (absolute value comparison).
 *
 * Input parameters
 * - a: number
 * - b: number
 * 
 * Return value
 * It returns 0 if |a| < |b|.
 * It returns 1 otherwise.
 */
int min_abs(num_t a, num_t b)
{
    if (fabs((double)a) < fabs((double)b)) {
        return 0;
    } else {
        return 1;
    }
}
