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
    tc_params[0].eta = 0.3;
    tc_params[0].alpha = 0.1;
    tc_params[0].beta = 4;
    tc_params[0].delta_t = 1;
    tc_params[0].m_points = 20;
    tc_params[0].k_max = 30;
    tc_params[0].initial_velocity = 0.5;

    /* Goldstein-Price function */
    tc_params[1].nd = 2;
    tc_params[1].low = -2;
    tc_params[1].high = 2;
    tc_params[1].goal = MIN_GOAL;
    tc_params[1].obj_func = goldstein_price;
    tc_params[1].eta = 0.002;
    tc_params[1].alpha = 0.1;
    tc_params[1].beta = 3;
    tc_params[1].delta_t = 1;
    tc_params[1].m_points = 20;
    tc_params[1].k_max = 30;
    tc_params[1].initial_velocity = 0.5;

    /* "Flipped" Goldstein-Price function */
    tc_params[2].nd = 2;
    tc_params[2].low = -2;
    tc_params[2].high = 2;
    tc_params[2].goal = MAX_GOAL;
    tc_params[2].obj_func = flipped_goldstein_price;
    tc_params[2].eta = 0.002;
    tc_params[2].alpha = 0.1;
    tc_params[2].beta = 3;
    tc_params[2].delta_t = 1;
    tc_params[2].m_points = 20;
    tc_params[2].k_max = 30;
    tc_params[2].initial_velocity = 0.5;

    /* Rastrigin function */
    tc_params[3].nd = 2;
    tc_params[3].low = -20;
    tc_params[3].high = 20;
    tc_params[3].goal = MIN_GOAL;
    tc_params[3].obj_func = rastrigin;
    tc_params[3].eta = 0.9;
    tc_params[3].alpha = 0.1;
    tc_params[3].beta = 4;
    tc_params[3].delta_t = 1;
    tc_params[3].m_points = 20;
    tc_params[3].k_max = 30;
    tc_params[3].initial_velocity = 0.5;
}
