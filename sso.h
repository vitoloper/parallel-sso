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

/* Basic C language type to use */
typedef double num_t;

/* test case parameters struct */
/* TODO: maybe include a function pointer the objective function */
struct tc_params_s {
    int nd;
    num_t low;
    num_t high;
};

/* Function declarations */
void print_usage(char *);
void init_tc_params(struct tc_params_s *);
void print_matrix(int, num_t **, int, int);
void init_positions(num_t **, int, int, num_t, num_t);

#endif /* SSO_H */
