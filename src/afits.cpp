#include"typedef.h"

/*drop neighborhood*/
void drop_operation(solution_data &sd, instance_data &ins, int *tl, int iter, double &delta_max, candidate_move *can_move, int &can_move_len, double &tabu_delta_max, candidate_move *tabu_move, int &tabu_move_len, 
	double pe_factor, const int cost_gs)
{
	double cost_delta;
	double cost_gs_delta;
	for (int i = 0; i < ins.num_items; i++)
	{
		if (sd.sol[i])													//always feasible solution
		{
			cost_delta = double(-ins.item_profit[i] + sd.forfeit_delta[i]);
			if (sd.weight - ins.item_weight[i] <= ins.capacity)
				cost_gs_delta = double(-cost_gs);
			else
				cost_gs_delta = double(sd.weight - ins.item_weight[i] - ins.capacity - cost_gs);
			cost_delta -= pe_factor*cost_gs_delta;
			if (tl[i] <= iter)
			{
				if (cost_delta > delta_max + PRECISION)
				{
					delta_max = cost_delta;
					can_move_len = 0;
					can_move[can_move_len].type = 'd';
					can_move[can_move_len].item1 = i;
					can_move_len++;
				}
				else if (fabs(cost_delta - delta_max) <= PRECISION && can_move_len < MAXCAN)
				{
					can_move[can_move_len].type = 'd';
					can_move[can_move_len].item1 = i;
					can_move_len++;
				}
			}
			else
			{
				if (cost_delta > tabu_delta_max + PRECISION)
				{
					tabu_delta_max = cost_delta;
					tabu_move_len = 0;
					tabu_move[tabu_move_len].type = 'd';
					tabu_move[tabu_move_len].item1 = i;
					tabu_move_len++;
				}
				else if (fabs(cost_delta - tabu_delta_max) <= PRECISION && tabu_move_len < MAXCAN)
				{
					tabu_move[tabu_move_len].type = 'd';
					tabu_move[tabu_move_len].item1 = i;
					tabu_move_len++;
				}
			}
		}
	}
}

/*add neighborhood*/
void add_operation(solution_data &sd, instance_data &ins, int *tl, int iter, double &delta_max, candidate_move *can_move, int &can_move_len, double &tabu_delta_max, candidate_move *tabu_move, int &tabu_move_len,
	double pe_factor, const int cost_gs)
{
	double cost_delta;
	double cost_gs_delta;
	for (int i = 0; i < ins.num_items; i++)
	{
		if (!sd.sol[i])							//relax the capacity constraint
		{
			cost_delta = double(ins.item_profit[i] - sd.forfeit_delta[i]);
			if (sd.weight + ins.item_weight[i] <= ins.capacity)
				cost_gs_delta = double(-cost_gs);
			else
				cost_gs_delta = double(sd.weight + ins.item_weight[i] - ins.capacity - cost_gs);
			cost_delta -= pe_factor*cost_gs_delta;
			if (tl[i] <= iter)
			{
				if (cost_delta > delta_max + PRECISION)
				{
					delta_max = cost_delta;
					can_move_len = 0;
					can_move[can_move_len].type = 'a';
					can_move[can_move_len].item1 = i;
					can_move_len++;
				}
				else if (fabs(cost_delta - delta_max) <= PRECISION && can_move_len < MAXCAN)
				{
					can_move[can_move_len].type = 'a';
					can_move[can_move_len].item1 = i;
					can_move_len++;
				}
			}
			else
			{
				if (cost_delta > tabu_delta_max + PRECISION)
				{
					tabu_delta_max = cost_delta;
					tabu_move_len = 0;
					tabu_move[tabu_move_len].type = 'a';
					tabu_move[tabu_move_len].item1 = i;
					tabu_move_len++;
				}
				else if (fabs(cost_delta - tabu_delta_max) <= PRECISION && tabu_move_len < MAXCAN)
				{
					tabu_move[tabu_move_len].type = 'a';
					tabu_move[tabu_move_len].item1 = i;
					tabu_move_len++;
				}
			}
		}
	}
}

/*swap neighborhood*/
void swap_operation(solution_data &sd, instance_data &ins, int *tl, int iter, double &delta_max, candidate_move *can_move, int &can_move_len, double &tabu_delta_max, candidate_move *tabu_move, int &tabu_move_len,
	double pe_factor, const int cost_gs)
{
	double cost_delta;
	double cost_gs_delta;
	for (int i = 0; i < ins.num_items; i++)
	{
		if (sd.sol[i])			
		{
			for (int j = i + 1; j < ins.num_items; j++)				
			{
				if (!sd.sol[j])												//relax the capacity constraint
				{
					cost_delta = double(ins.item_profit[j] - ins.item_profit[i] + sd.forfeit_delta[i] - sd.forfeit_delta[j] + ins.cost_forfeit[i][j]);
					if (sd.weight + ins.item_weight[j] - ins.item_weight[i] <= ins.capacity)
						cost_gs_delta = double(-cost_gs);
					else
						cost_gs_delta = double(sd.weight + ins.item_weight[j] - ins.item_weight[i] - ins.capacity - cost_gs);
					cost_delta -= pe_factor*cost_gs_delta;
					if (tl[i] <= iter && tl[j] <= iter)
					{
						if (cost_delta > delta_max + PRECISION)
						{
							delta_max = cost_delta;
							can_move_len = 0;
							can_move[can_move_len].type = 's';
							can_move[can_move_len].item1 = i;
							can_move[can_move_len].item2 = j;
							can_move_len++;
						}
						else if (fabs(cost_delta - delta_max) <= PRECISION && can_move_len < MAXCAN)
						{
							can_move[can_move_len].type = 's';
							can_move[can_move_len].item1 = i;
							can_move[can_move_len].item2 = j;
							can_move_len++;
						}
					}
					else
					{
						if (cost_delta > tabu_delta_max + PRECISION)
						{
							tabu_delta_max = cost_delta;
							tabu_move_len = 0;
							tabu_move[tabu_move_len].type = 's';
							tabu_move[tabu_move_len].item1 = i;
							tabu_move[tabu_move_len].item2 = j;
							tabu_move_len++;
						}
						else if (fabs(cost_delta - tabu_delta_max) <= PRECISION && tabu_move_len < MAXCAN)
						{
							tabu_move[tabu_move_len].type = 's';
							tabu_move[tabu_move_len].item1 = i;
							tabu_move[tabu_move_len].item2 = j;
							tabu_move_len++;
						}
					}
				}
			}
		}
	}
}

/*remove some items until the capacity of the solution is not violated according the pi_wi value */
void repair_solution(const instance_data &ins, solution_data &fg_sol, solution_data &sd)
{
	bool is_feasible = false;
	int index = ins.num_items - 1;
	int item;
	int cost_forfeits = 0;
	fg_sol.cost = 0;
	fg_sol.weight = 0;
	for (int i = 0; i < ins.num_items; i++)
	{
		if (fg_sol.sol[i])
		{
			fg_sol.weight += ins.item_weight[i];
			fg_sol.cost += ins.item_profit[i];
		}
	}
	while (!is_feasible)
	{
		is_feasible = false;
		while (index >= 0)
		{
			item = ins.item_map[index];
			if (fg_sol.sol[item])
			{
				fg_sol.weight -= ins.item_weight[item];
				fg_sol.cost -= ins.item_profit[item];
				fg_sol.sol[item] = false;
			}
			if (fg_sol.weight <= ins.capacity)					//satisfy the capacity constraint
			{
				is_feasible = true;
				break;
			}
			else
				index--;
		}
	}
	cost_forfeits = fg_sol.calculate_forfeit_cost(ins);
	fg_sol.cost -= cost_forfeits;
	sd.copy_solution(ins, fg_sol);
}

/*adaptive feasible and infeassible multi neighborood tabu search for local improvement
three neighborhoods including drop, add and swap neighborhoods are examined in a union way
*/
void adaptive_fea_inf_tabu_search(instance_data &ins, solution_data &sd, solution_data &sd_best, int *tabu_list, int tabu_tenure, int tt_base, int ts_depth, clock_t start_time, double &run_time, double time_limit, double incre, int lambda)
{
	candidate_move can_move[MAXCAN];
	candidate_move tabu_move[MAXCAN];
	solution_data sd_local_best(ins);
	solution_data fgsol_local_best(ins);
	double cost_delta_max, tabu_cost_delta_max;
	int can_move_len = 0;
	int tabu_move_len = 0;
	int iter = 0;
	int inf_count = 0;			
	int non_improve = 0;
	int cost_gs = sd.weight - ins.capacity <= 0 ? 0 : sd.weight - ins.capacity;	
	double penalty_factor = 1.0;
	double cost_fg = double(sd.cost - penalty_factor*cost_gs);
	double fg_local_best = cost_fg;
	fgsol_local_best.copy_solution(ins, sd);
	if (cost_gs <= 0)
		sd_local_best.copy_solution(ins, sd);
	else
		sd_local_best.cost = 0;
	double elasped_time = 1.0*(clock() - start_time) / CLOCKS_PER_SEC;
	memset(tabu_list, 0, sizeof(int)*ins.num_items);									//clear the tabu list
	//while (elasped_time < time_limit)
	while (iter < ts_depth && elasped_time < time_limit)
	//while (non_improve < ts_depth)
	{
		cost_delta_max = -MAXVALUE;
		tabu_cost_delta_max = -MAXVALUE;
		can_move_len = 0;
		tabu_move_len = 0;
		drop_operation(sd, ins, tabu_list, iter, cost_delta_max, can_move, can_move_len, tabu_cost_delta_max, tabu_move, tabu_move_len, penalty_factor, cost_gs);
		add_operation(sd, ins, tabu_list, iter, cost_delta_max, can_move, can_move_len, tabu_cost_delta_max, tabu_move, tabu_move_len, penalty_factor, cost_gs);
		swap_operation(sd, ins, tabu_list, iter, cost_delta_max, can_move, can_move_len, tabu_cost_delta_max, tabu_move, tabu_move_len, penalty_factor, cost_gs);
		//aspiration criterion
		if (tabu_move_len > 0 && tabu_cost_delta_max > cost_delta_max + PRECISION && cost_fg + tabu_cost_delta_max > fg_local_best)
		{
			sd.execute_move(ins, tabu_move, tabu_move_len, tabu_list, tabu_tenure, tt_base, iter);
			cost_fg += tabu_cost_delta_max;
		}
		else
		{
			if (can_move_len > 0)
			{
				sd.execute_move(ins, can_move, can_move_len, tabu_list, tabu_tenure, tt_base, iter);
				cost_fg += cost_delta_max;
			}
		}
		cost_gs = sd.weight - ins.capacity <= 0 ? 0 : sd.weight - ins.capacity;
		if (cost_fg > fg_local_best + PRECISION)
		{
			fg_local_best = cost_fg;
			fgsol_local_best.copy_solution(ins, sd);
			non_improve = 0;
		}
		else
			non_improve++;
		if (sd.cost > sd_local_best.cost && cost_gs == 0)
			sd_local_best.copy_solution(ins, sd);
		if (sd.cost > sd_best.cost && cost_gs == 0)
		{
			sd_best.copy_solution(ins, sd);
			run_time = elasped_time;
		}
		if (cost_gs > 0)
			inf_count++;
		if (iter % lambda == 0)
		{
			if (inf_count == lambda)
				penalty_factor *= incre;
			if (inf_count == 0)
				penalty_factor /= incre;
			if (penalty_factor < PRECISION)
				penalty_factor = 1;
			inf_count = 0;
		}
		//sd.check_move(ins, cost_gs);
		elasped_time = 1.0*(clock() - start_time) / CLOCKS_PER_SEC;
		iter++;
		/*if (iter % 1000 == 0)
		{
			//cout << "iter=" << iter << ", sd.cost=" << sd.cost << ", sd_best.cost =" << sd_best.cost << ", delta_max=" << cost_delta_max << ", tabu_max=" << tabu_cost_delta_max <<
			//	" can_len=" << can_move_len << ", tabu_len=" << tabu_move_len << ", pe_factor=" << penalty_factor << ", inf_count=" << inf_count << endl;
			cout << "iter=" << iter << ", sd.cost=" << sd.cost << ", sd_local_best.cost=" << sd_local_best.cost << ", sd_best.cost =" << sd_best.cost <<
				", pe_factor=" << penalty_factor << ", inf_count=" << inf_count << endl;
		}*/
	}
	cout << "sd_local_best.cost=" << sd_local_best.cost << endl;
	if (sd_local_best.cost > 0)
		sd.copy_solution(ins, sd_local_best);
	else
	{
		cout << "during the local search, there is no feasible solution encountered, and then is repaired" << endl;
		repair_solution(ins, fgsol_local_best, sd);
		sd.verify_solution(ins);
	}
}