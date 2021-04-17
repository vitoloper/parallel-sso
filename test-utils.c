/* Application used to test the utility functions in utils.c
 * Compile with:
 * mpicc -Wall -g -lm test-utils.c utils.c -o test-utils
 *
 * Check for memory leaks with valgrind:
 * valgrind --leak-check=yes ./test-utils
 *
 */
#include <stdio.h>
#include <stdlib.h>

#include "sso.h"

/* Matrix dimensions */
#define M 3
#define N 4
#define P 5

int main()
{
    int i, j, k;

    num_t **cont_matrix;
    num_t *storage;
    num_t **matrix2d;
    num_t ***matrix3d;

    printf("*** Utility function test application ***\n\n");

    printf("Testing matrix allocation with storage (contiguous memory)\n");
    allocate_cont_matrix(&cont_matrix, &storage, M, N);
    /* Initialize matrix */
    for (i = 0; i < M; i++) {
        for (j = 0; j < N; j++) {
            cont_matrix[i][j] = i + j;
        }
    }
    printf("Done\n\n");

    printf("Testing print matrix\n");
    print_matrix(0, cont_matrix, M, N);
    printf("Done\n\n");

    printf("Testing print vector (third matrix row)\n");
    print_vector(0, cont_matrix[2], N);
    printf("Done\n\n");

    printf("Testing 2d matrix allocation\n");
    allocate_2d_matrix(&matrix2d, M, N);
    for (i = 0; i < M; i++) {
        for (j = 0; j < N; j++) {
            matrix2d[i][j] = 10 + i + j;
        }
    }
    print_matrix(0, matrix2d, M, N);
    printf("Done\n\n");

    printf("Testing 3d matrix allocation\n");
    allocate_3d_matrix(&matrix3d, M, N, P);
    for (i = 0; i < M; i++) {
        for (j = 0; j < N; j++) {
            for (k = 0; k < P; k++) {
                matrix3d[i][j][k] = 100 + i + j + k;
            }
        }
    }

    for (i = 0; i < M; i++) {
        printf("i = %d\n", i);
        print_matrix(0, matrix3d[i], N, P);
    }
    printf("Done\n\n");

    free(cont_matrix);
    free(storage);
    free_2d_matrix(&matrix2d, M);
    free_3d_matrix(&matrix3d, M, N);

    printf("*** END ***\n");
    return 0;
}
