#ifndef SSO_H
#define SSO_H

#include "mpi.h"

/* Block utility macros */
#define BLOCK_LOW(id, p, n) ((id) * (n) / (p))
#define BLOCK_HIGH(id, p, n) (BLOCK_LOW((id) + 1, p, n) - 1)
#define BLOCK_SIZE(id, p, n) (BLOCK_HIGH(id, p, n) - BLOCK_LOW(id, p, n) + 1)

/* Basic MPI datatype to use */
#define NUM_DT MPI_DOUBLE

/* Number of available test cases */
#define NUM_OF_TC 1

/* Minimization or maximization of the objective function */
#define MIN_GOAL -1
#define MAX_GOAL 1

/* Differentiation increment */
#define D_INCR 0.00001

/* Basic C language type to use */
typedef double num_t;

/* test case parameters struct */
struct tc_params_s {
    int nd;
    num_t low;
    num_t high;
    int goal;
    double (*obj_func)(num_t *, int nd);
};

/* Function declarations */

void print_usage(char *);
void init_tc_params(struct tc_params_s *);
void print_matrix(int, num_t **, int, int);
void print_vector(int, num_t *, int);

/* Matrix allocation functions */
int allocate_cont_matrix(num_t ***, num_t **, int, int);
int allocate_2d_matrix(num_t ***, int, int);
void free_2d_matrix(num_t ***, int);
int allocate_3d_matrix(num_t ****, int, int, int);
void free_3d_matrix(num_t ****, int, int);

void init_positions(num_t **, int, int, num_t, num_t);
int gradient(num_t (*)(num_t *, int), num_t *, int nd, num_t *);
int min_abs(num_t, num_t);

/* Objective functions */
num_t elliptic_paraboloid(num_t *, int nd);

#endif /* SSO_H */
