/* Application used to test the compute_best_solution function
 * Compile with:
 * mpicc -Wall -g -lm test_compute_best_solution.c compute_best_solution.c init_positions.c of.c tc.c utils.c -o test_compute_best_solution
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
    num_t best_val;
    struct tc_params_s tc_params[NUM_OF_TC]; /* Test cases parameters array */

    /* Seed the PRNG */
    srand(time(NULL));

    /* Allocate a 2d matrix */
    allocate_2d_matrix(&X, np, nd);

    /* Allocate space for the solution vector */
    best_solution = (num_t *)malloc(nd * sizeof(num_t));

    /* Initialize test case parameters array */
    init_tc_params(tc_params);

    /* Minimization of an ellpitic paraboloid function */
    printf("Minimization of an elliptic paraboloid function\n");
    init_positions(X, np, nd, -20, 20);
    // printf("Initial positions:\n");
    // print_matrix(0, X, np, nd);
    compute_best_solution(tc_params[0], X, np,
                          best_solution, &best_val);
    printf("Best solution: [%f, %f]\n", best_solution[0], best_solution[1]);
    printf("Minimum value: %f\n\n", best_val);

    /* Minimization of Goldstein-Price function */
    printf("Minimization of Goldstein-Price function\n");
    init_positions(X, np, nd, -2, 2);
    // printf("Initial positions:\n");
    // print_matrix(0, X, np, nd);
    compute_best_solution(tc_params[1], X, np,
                          best_solution, &best_val);
    printf("Best solution: [%f, %f]\n", best_solution[0], best_solution[1]);
    printf("Minimum value: %f\n\n", best_val);

    /* Maximization of "flipped" Goldstein-Price function */
    printf("Maximization of \"flipped\" of Goldstein-Price function\n");
    init_positions(X, np, nd, -2, 2);
    // printf("Initial positions:\n");
    // print_matrix(0, X, np, nd);
    compute_best_solution(tc_params[2], X, np,
                          best_solution, &best_val);
    printf("Best solution: [%f, %f]\n", best_solution[0], best_solution[1]);
    printf("Maximum value: %f\n\n", best_val);

    /* Free heap space */
    free(best_solution);
    free_2d_matrix(&X, np);

    return 0;
}
