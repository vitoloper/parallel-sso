/*
 * Test cases parameters initialization.
 *
 * (C) 2021 Giuseppe Vitolo
 */

#include "sso.h"

/*
 * This function initialized the test cases array with the appropriate
 * parameters.
 * 
 * Output parameters
 * - tc_params: test cases array
 */
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

    /* Rastrigin function (two decision variables) */
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

    /* Rastrigin function (five decision variables) */
    tc_params[4].nd = 5;
    tc_params[4].low = -20;
    tc_params[4].high = 20;
    tc_params[4].goal = MIN_GOAL;
    tc_params[4].obj_func = rastrigin;
    tc_params[4].eta = 0.9;
    tc_params[4].alpha = 0.1;
    tc_params[4].beta = 4;
    tc_params[4].delta_t = 1;
    tc_params[4].m_points = 20;
    tc_params[4].k_max = 30;
    tc_params[4].initial_velocity = 0.5;

    /* Griewangk function (two decision variables) */
    tc_params[5].nd = 2;
    tc_params[5].low = -600;
    tc_params[5].high = 600;
    tc_params[5].goal = MIN_GOAL;
    tc_params[5].obj_func = griewangk;
    tc_params[5].eta = 0.9;
    tc_params[5].alpha = 0.1;
    tc_params[5].beta = 4;
    tc_params[5].delta_t = 1;
    tc_params[5].m_points = 20;
    tc_params[5].k_max = 30;
    tc_params[5].initial_velocity = 0.5;

    /* Griewangk function (five decision variables) */
    tc_params[6].nd = 5;
    tc_params[6].low = -600;
    tc_params[6].high = 600;
    tc_params[6].goal = MIN_GOAL;
    tc_params[6].obj_func = griewangk;
    tc_params[6].eta = 0.9;
    tc_params[6].alpha = 0.1;
    tc_params[6].beta = 4;
    tc_params[6].delta_t = 1;
    tc_params[6].m_points = 20;
    tc_params[6].k_max = 30;
    tc_params[6].initial_velocity = 0.5;

    /* Schaffer function */
    tc_params[7].nd = 2;
    tc_params[7].low = -100;
    tc_params[7].high = 100;
    tc_params[7].goal = MIN_GOAL;
    tc_params[7].obj_func = schaffer;
    tc_params[7].eta = 0.9;
    tc_params[7].alpha = 0.1;
    tc_params[7].beta = 4;
    tc_params[7].delta_t = 1;
    tc_params[7].m_points = 20;
    tc_params[7].k_max = 30;
    tc_params[7].initial_velocity = 0.5;
}
