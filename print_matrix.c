#include <stdio.h>

#include "sso.h"

/* Print an m*n matrix where each element is of num_t type */
void print_matrix(num_t **matrix, int m, int n)
{
    int i, j;

    for (i = 0; i < m; i++) {
        for (j = 0; j < n; j++) {
            printf("(%d,%d): %9.6f ", i, j, matrix[i][j]);
        }
        printf("\n");
    }
}
