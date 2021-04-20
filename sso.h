/*
 * Parallel SSO application header.
 *
 * (C) 2021 Giuseppe Vitolo
 */
#ifndef SSO_H
#define SSO_H

#include "mpi.h"

/* Block utility macros */
#define BLOCK_LOW(id, p, n) ((id) * (n) / (p))
#define BLOCK_HIGH(id, p, n) (BLOCK_LOW((id) + 1, p, n) - 1)
#define BLOCK_SIZE(id, p, n) (BLOCK_HIGH(id, p, n) - BLOCK_LOW(id, p, n) + 1)

/* MAX/MIN macros */
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define MIN(a, b) (((a) < (b)) ? (a) : (b))

/* Basic MPI datatype to use */
#define NUM_DT MPI_DOUBLE

/* Number of available test cases */
#define NUM_OF_TC 7

/* Minimization or maximization of the objective function */
#define MIN_GOAL -1
#define MAX_GOAL 1

/* Basic C language type to use */
typedef double num_t;

/* test case parameters struct */
struct tc_params_s {
    int nd;                              /* number of decision variables */
    num_t low;                           /* decision variables lower bound */
    num_t high;                          /* decision veriables upper bound */
    int goal;                            /* MIN_GOAL / MAX_GOAL */
    double (*obj_func)(num_t *, int nd); /* objective function */
    num_t eta;                           /* eta */
    num_t alpha;                         /* alpha */
    num_t beta;                          /* beta */
    num_t delta_t;                       /* delta_t*/
    num_t m_points;                      /* M (local search) */
    num_t k_max;                         /* total steps */
    num_t initial_velocity;              /* initial velocity */
};

/* Function declarations */

void print_usage(char *name);
void init_tc_params(struct tc_params_s *tc_params);
void print_matrix(int rank, num_t **matrix, int m, int n);
void print_vector(int rank, num_t *v, int length);

/* Matrix allocation functions */
int allocate_cont_matrix(num_t ***M, num_t **M_storage, int m, int n);
int allocate_2d_matrix(num_t ***M, int m, int n);
void free_2d_matrix(num_t ***M, int m);
int allocate_3d_matrix(num_t ****M, int m, int n, int p);
void free_3d_matrix(num_t ****M, int m, int n);

void init_positions(num_t **X, int np, int nd, num_t low, num_t high);
int gradient(num_t (*f)(num_t *, int), num_t *X, int nd, num_t *result);
int min_abs(num_t a, num_t b);
int compute_best_solution(struct tc_params_s tc_params, num_t **X, int np,
                          num_t *best_solution, num_t *best_val);

/* Custom reduce operations */
void find_max_val(void *in_param, void *inout_param, int *len,
                  MPI_Datatype *dt);
void find_min_val(void *in_param, void *inout_param, int *len,
                  MPI_Datatype *dt);

/* Objective functions */
num_t elliptic_paraboloid(num_t *X, int nd);
num_t goldstein_price(num_t *X, int nd);
num_t flipped_goldstein_price(num_t *X, int nd);
num_t rastrigin(num_t *X, int nd);
num_t griewangk(num_t *X, int nd);

#endif /* SSO_H */
