#ifndef SSO_H
#define SSO_H

#include "mpi.h"

/* Error definition (use Michael J. Quinn convention as a starting point) */
#define OPEN_FILE_ERROR -1
#define MALLOC_ERROR -2
#define TYPE_ERROR -3
#define ARGC_ERROR -4

/* Block utility macros */
#define BLOCK_LOW(id, p, n) ((id) * (n) / (p))
#define BLOCK_HIGH(id, p, n) (BLOCK_LOW((id) + 1, p, n) - 1)
#define BLOCK_SIZE(id, p, n) (BLOCK_HIGH(id, p, n) - BLOCK_LOW(id, p, n) + 1)

/* Basic MPI datatype to use */
#define NUM_DT MPI_DOUBLE

/* Basic C language type to use */
typedef double num_t;

/* Function declarations */
void print_usage(char *);
void print_matrix(num_t **, int, int);
void init_positions(num_t **, int, int, num_t, num_t);

#endif /* SSO_H */
