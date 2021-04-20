/*
 * Custom reduce operations.
 *
 * (C) 2021 Giuseppe Vitolo
 */
#include "sso.h"

#include <string.h>

#include "mpi.h"

/*
 * This function implements a custom max reduce operation by comparing the last
 * element in each input vector.
 * Note that inout_param is both an input and an output parameter.
 *
 * Input parameters
 * - in_param: array of len element (first operand)
 * - inout_param: array of len element (second operand)
 * - len: # of elements in the comm buffers (in_param and inout_param
 * - dt: datatype
 *
 * Output parameters
 * -inout_param: array of len element
 */
void find_max_val(void *in_param, void *inout_param, int *len, MPI_Datatype *dt)
{
    int ni; /* # of input integers used in the call constructing combiner */
    int na; /* # of input addresses used in the call constructing combiner */
    int nd; /* # of input datatypes used in the call constructing combiner */
    int combiner;  /* combiner: reflects the MPI data type constructor call */
    int i[1];      /* integer arguments used in constructing datatype */
    MPI_Aint a[1]; /* address arguments used in constructing datatype */
    MPI_Datatype d[1]; /* datatype arguments used in constructing datatype */
    num_t max_val;     /* current maximum value */
    int max_val_idx;   /* index of max value inside input vector */
    num_t *in = (num_t *)in_param;
    num_t *inout = (num_t *)inout_param;

    /* Get datatype information. For a contiguous datatype we have ni = 1, na =
     * 0, nd = 1. The information we need is the number of elements in the
     * solution vector, so we can get the index of the maximum value in the
     * array (last element). */
    MPI_Type_get_envelope(*dt, &ni, &na, &nd, &combiner);
    MPI_Type_get_contents(*dt, ni, na, nd, i, a, d);
    max_val_idx = i[0] - 1; /* (nd+1) - 1 */

    /* NOTE: *len = 1 because we passed a single element of type row_result_type
     * from the caller */

    max_val = MAX(in[max_val_idx], inout[max_val_idx]);

    /* If the output vector (inout) already is the maximum, do nothing.
     * Otherwise, the other vector (in) is the maximum: copy it in 'inout'. */
    if (inout[max_val_idx] != max_val) {
        memcpy(inout, in, (max_val_idx + 1) * sizeof(num_t));
    }
}

/*
 * This function implements a custom min reduce operation by comparing the last
 * element in each input vector.
 * Note that inout_param is both an input and an output parameter.
 *
 * Input parameters
 * - in_param: array of len element (first operand)
 * - inout_param: array of len element (second operand)
 * - len: # of elements in the comm buffers (in_param and inout_param)
 * - dt: datatype
 *
 * Output parameters
 * -inout_param: array of len element
 */
void find_min_val(void *in_param, void *inout_param, int *len, MPI_Datatype *dt)
{
    int ni; /* # of input integers used in the call constructing combiner */
    int na; /* # of input addresses used in the call constructing combiner */
    int nd; /* # of input datatypes used in the call constructing combiner */
    int combiner;  /* combiner: reflects the MPI data type constructor call */
    int i[1];      /* integer arguments used in constructing datatype */
    MPI_Aint a[1]; /* address arguments used in constructing datatype */
    MPI_Datatype d[1]; /* datatype arguments used in constructing datatype */
    num_t min_val;     /* current minimum value */
    int min_val_idx;   /* index of min value inside input vector */
    num_t *in = (num_t *)in_param;
    num_t *inout = (num_t *)inout_param;

    /* Get datatype information. For a contiguous datatype we have ni = 1, na =
     * 0, nd = 1. The information we need is the number of elements in the
     * solution vector, so we can get the index of the minimum value in the
     * array (last element). */
    MPI_Type_get_envelope(*dt, &ni, &na, &nd, &combiner);
    MPI_Type_get_contents(*dt, ni, na, nd, i, a, d);
    min_val_idx = i[0] - 1; /* (nd+1) - 1 */

    /* NOTE: *len = 1 because we passed a single element of type row_result_type
     * from the caller */

    min_val = MIN(in[min_val_idx], inout[min_val_idx]);

    /* If the output vector (inout) already is the minimum, do nothing.
     * Otherwise, the other vector (in) is the minimum: copy it in 'inout'. */
    if (inout[min_val_idx] != min_val) {
        memcpy(inout, in, (min_val_idx + 1) * sizeof(num_t));
    }
}
