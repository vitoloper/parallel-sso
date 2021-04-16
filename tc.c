#include "sso.h"

/* Initialize test case parameters array */
void init_tc_params(struct tc_params_s *tc_params)
{
    /* Elliptic paraboloid */
    tc_params[0].nd = 2;
    tc_params[0].low = -100;
    tc_params[0].high = 100;
    tc_params[0].goal = MIN_GOAL;
    tc_params[0].obj_func = elliptic_paraboloid;
}
