/* Application used to test the compute_best_solution function
 * Compile with:
 * mpicc -Wall -g -lm test_compute_best_solution.c compute_best_solution.c
 * init_positions.c of.c utils.c -o test_compute_best_solution
 *
 * Check for memory leaks with valgrind:
 * valgrind --leak-check=yes ./test_compute_best_solution
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "sso.h"

int main()
{
    num_t **X;
    int np = 10;
    int nd = 2;
    num_t *best_solution;
    num_t best_minval;

    /* Seed the PRNG */
    srand(time(NULL));

    /* Allocate a 2d matrix */
    allocate_2d_matrix(&X, np, nd);

    /* Allocate space for the solution vector */
    best_solution = (num_t *)malloc(nd * sizeof(num_t));

    /* Initialize solution vectors */
    init_positions(X, np, nd, -20, 20);

    printf("Initial positions:\n");
    print_matrix(0, X, np, nd);

    /* Compute best solution */
    compute_best_solution(elliptic_paraboloid, MIN_GOAL, X, np, nd,
                          best_solution, &best_minval);

    printf("Best solution: [%f, %f]\n", best_solution[0], best_solution[1]);
    printf("Minimum value: %f\n", best_minval);

    /* Free heap space */
    free(best_solution);
    free_2d_matrix(&X, np);

    return 0;
}
