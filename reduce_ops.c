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
 */
void find_max_val(void *in_param, void *out_param, int *len, MPI_Datatype *dt)
{
}

/*
 * This function implements a custom min reduce operation by comparing the last
 * element in each input vector.
 */
void find_min_val(void *in_param, void *inout_param, int *len, MPI_Datatype *dt)
{
    int ni, na, nd, combiner;
    int i[1];
    MPI_Aint a[1];
    MPI_Datatype d[1];
    num_t min_val;
    num_t *in = (num_t *)in_param;
    num_t *inout = (num_t *)inout_param;
    int min_val_idx;

    /* Get datatype information. For a contiguous datatype we have ni = 1, na =
     * 0, nd = 1. The information we need is the number of elements in the
     * solution vector, so we can get the index of the minimum value in the
     * array (last element). */
    MPI_Type_get_envelope(*dt, &ni, &na, &nd, &combiner);
    MPI_Type_get_contents(*dt, ni, na, nd, i, a, d);
    min_val_idx = i[0] - 1; /*(nd+1) - 1*/

    /* NOTE: *len = 1 because we passed a single element of type row_result_type
     * from the caller */

    min_val = MIN(in[min_val_idx], inout[min_val_idx]);

    /* If the output vector (inout) already is the minimum, do nothing.
     * Otherwise, the other vector (in) is the minimum: copy it in 'inout'. */
    if (inout[min_val_idx] != min_val) {
        memcpy(inout, in, (min_val_idx + 1) * sizeof(num_t));
    }
}
