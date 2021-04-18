#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

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

/* Print a vector of length 'length' */
void print_vector(int rank, num_t *v, int length)
{
    int i;

    for (i = 0; i < length; i++) {
        printf("(%d): %9.6f ", rank, v[i]);
    }
    printf("\n");
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

/* Allocate 2d matrix */
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

/* Free 2d matrix */
void free_2d_matrix(num_t ***M, int m)
{
    int i;

    for (i = 0; i < m; i++) {
        free((*M)[i]);
    }

    free(*M);
}

/* Allocate 3d matrix */
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

/* Free 3d matrix */
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
 * Compute numerical gradient using central difference approximation.
 *
 * f: function pointer
 * X: parameter array
 * nd: number of decision variables
 * result [OUT]: result vector of length nd
 */
int gradient(num_t (*f)(num_t *, int), num_t *X, int nd, num_t *result)
{
    int i;
    num_t *current_X_right;
    num_t *current_X_left;
    num_t h = 0.00001;

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

/* Given two values, return 0 if the first number has a lower abs value. Return
 * 1 otherwise. */
int min_abs(num_t a, num_t b)
{
    if (fabs((double)a) < fabs((double)b)) {
        return 0;
    } else {
        return 1;
    }
}
