#ifndef _FUNC_STATE_H
#define _FUNC_STATE_H
#include"typedef.h"

/*multi-start multi neighborood tabu search for local improvement
three neighborhoods including drop, add and swap neighborhoods are examined in a union way*/
void adaptive_fea_inf_tabu_search(char file[], instance_data &ins, solution_data &sd, solution_data &sd_best, int *tabu_list, int tabu_tenure, int tt_base, int ts_depth, clock_t start_time, double &run_time, double time_limit, double incre, int lambda);
void evolutionary_search(char file[], char file_out[], char sol_out[], instance_data &ins, int tabu_tenure, int tt_base, int ts_depth, double time_limit, double incre, int lambda, int pop_num, int ini_num, double mut_str);
/* sort in an ascending order */
bool compare_descent(const map_data &a, const map_data &b);

int calculate_fitness(const instance_data &ins, const bool* sol, int& weight);


#endif