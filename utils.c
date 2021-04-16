#include <stdio.h>
#include <stdlib.h>

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
int allocate_contiguous_matrix(num_t ***M, num_t **M_storage, int m, int n) {
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
        *M[i] = &(*M_storage[i * n]);        
    }

    return 1;
}
