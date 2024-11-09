#include"typedef.h"
#include"func_state.h"

int calculate_common_nodes(const instance_data &ins, const solution_data &sd1, const solution_data &sd2);

void build_neighbors(const instance_data &ins, candidate_move *neighbors)
{	
	int count = 0;
	//int total_neighbors = ins.num_items*(ins.num_items - 1) / 2 + ins.num_items;
	for (int i = 0; i < ins.num_items; i++)
	{		
		neighbors[count].type = 'o';
		neighbors[count].item1 = i;
		neighbors[count].item2 = -1;
		count++;	
	}
	for (int i = 0; i < ins.num_items; i++)
	{
		for (int j = i + 1; j < ins.num_items; j++)
		{
			neighbors[count].type = 's';
			neighbors[count].item1 = i;
			neighbors[count].item2 = j;
			count++;
		}
	}
}

void initial_population(char file_in[], instance_data &ins, solution_data *pop, int pop_num, int ini_num, solution_data &sd_best, int *tabu_list, int tabu_tenure, int tt_base, int ts_depth,
	clock_t start_time, double &run_time, double time_limit, double incre, int lambda)
{
	//calculate the item_map data
	solution_data *sd_can = new solution_data[ini_num];
	int *sol_map = new int[ini_num];
	bool is_exist;
	int real_num = 0, index = 0;
	int map_num;
	for (int i = 0; i < ini_num; i++)
	{
		sd_can[i].initial_sol_random(ins);
		sd_can[i].verify_solution(ins);
		cout << "i=" << i << ", cost=" << sd_can[i].cost << endl;
	}
	vector<map_data> sort_sols(ini_num);
	for (int i = 0; i < ini_num; i++)
	{
		sort_sols[i].item = i;
		sort_sols[i].pi_wi = float(sd_can[i].cost);
	}
	sort(sort_sols.begin(), sort_sols.end(), compare_descent);
	for (int i = 0; i < ini_num; i++)
		sol_map[i] = sort_sols[i].item;	
	while (real_num < pop_num && index < ini_num)
	{
		is_exist = false;
		map_num = sol_map[index++];
		pop[real_num].copy_solution(ins, sd_can[map_num]);
		adaptive_fea_inf_tabu_search(file_in, ins, pop[real_num], sd_best, tabu_list, tabu_tenure, tt_base, ts_depth, start_time, run_time, time_limit, incre, lambda);
		for (int i = 0; i < real_num; i++)
		{
			is_exist = pop[real_num].is_same_solution(ins, pop[i]);
			if (is_exist)
				break;
		}
		cout << "real_num=" << real_num << ", cost=" << pop[real_num].cost << ", index=" << index << endl;
		if (!is_exist)
			real_num++;
	}
	
	while (real_num < pop_num)
	{
		is_exist = false;
		pop[real_num].initial_sol_random(ins);
		adaptive_fea_inf_tabu_search(file_in, ins, pop[real_num], sd_best, tabu_list, tabu_tenure, tt_base, ts_depth, start_time, run_time, time_limit, incre, lambda);

		for (int i = 0; i < real_num; i++)
		{
			is_exist = pop[real_num].is_same_solution(ins, pop[i]);
			if (is_exist)
				break;
		}
		//cout << "real_num2=" << real_num << ", cost=" << pop[real_num].cost << endl;
		if (!is_exist)
			real_num++;
	}
	delete[]sol_map; sol_map = NULL;
}

void uniform_crossover(const instance_data &ins, solution_data &child, const solution_data *pop, int pop_num)
{
	int rx;
	int parent1 = rand() % pop_num;
	int parent2 = rand() % pop_num;
	while (parent2 == parent1)
		parent2 = rand() % pop_num;
	child.initialing_solution_datas(ins.num_items);							//initialing the solution datas
	for (int i = 0; i < ins.num_items; i++)
	{
		if (pop[parent1].sol[i] == pop[parent2].sol[i])
			child.sol[i] = pop[parent1].sol[i];
		else
		{
			rx = rand() % 2;
			if (rx == 0)
				rx = parent1;
			else
				rx = parent2;
			child.sol[i] = pop[rx].sol[i];
		}
		if (child.sol[i])
		{
			child.weight += ins.item_weight[i];
			child.cost += ins.item_profit[i];
		}
	}
	//cost_forfeits = child.calculate_forfeit_cost(ins);
	//child.cost -= cost_forfeits;
}

void random_mutation(const instance_data &ins, solution_data &sd, const candidate_move *neighbors, int total_neighbors, double mut_str)
{	
	int cur_index, count = 0;
	int item1, item2;
	int count_theta = int(mut_str*ins.num_items);
	do
	{
		cur_index = rand() % total_neighbors;
		if (neighbors[cur_index].type == 'o')
		{
			item1 = neighbors[cur_index].item1;			
			if (sd.sol[item1])
			{
				sd.weight -= ins.item_weight[item1];
				sd.cost -= ins.item_profit[item1];
				sd.sol[item1] = false;
			}
			else
			{
				sd.weight += ins.item_weight[item1];
				sd.cost += ins.item_profit[item1];
				sd.sol[item1] = true;
			}
			count++;		
		}
		else if (neighbors[cur_index].type == 's')
		{
			item1 = neighbors[cur_index].item1;
			item2 = neighbors[cur_index].item2;
			if (sd.sol[item1] && !sd.sol[item2])
			{
				sd.weight += ins.item_weight[item2] - ins.item_weight[item1];
				sd.cost += ins.item_profit[item2] - ins.item_profit[item1];
				sd.sol[item1] = false;
				sd.sol[item2] = true;
				count++;
			}
			else if (!sd.sol[item1] && sd.sol[item2])
			{
				sd.weight += ins.item_weight[item1] - ins.item_weight[item2];
				sd.cost += ins.item_profit[item1] - ins.item_profit[item2];
				sd.sol[item1] = true;
				sd.sol[item2] = false;
				count++;
			}			
		}
	} while (count < count_theta);
} 

void update_population(const instance_data &ins, solution_data &child, solution_data *pop, int pop_num)
{
	int worst_cost = MAXVALUE;
	int worst_ind = -1;
	bool is_exist = false;
	for (int i = 0; i < pop_num; i++)
	{
		is_exist = child.is_same_solution(ins, pop[i]);
		if (is_exist)
			break;
	}
	if (!is_exist)
	{
		for (int i = 0; i < pop_num; i++)
		{
			if (pop[i].cost < worst_cost)
			{
				worst_cost = pop[i].cost;
				worst_ind = i;
			}
		}
		if (child.cost > pop[worst_ind].cost)
			pop[worst_ind].copy_solution(ins, child);
	}
}

/* update the population according to distance-quality updateing rule */
void update_population_distance_quality(const instance_data &ins, solution_data &child, solution_data *pop, int pop_num)
{
	int *distance = new int[pop_num + 1];
	int *f_obj = new int[pop_num + 1];
	double *score = new double[pop_num + 1];
	int f_min = MAXVALUE, f_max = -MAXVALUE;
	int dis_min = MAXVALUE, dis_max = -MAXVALUE;
	double beta = 0.6, score_worst = MAXVALUE;	
	int select_index, common_nodes;
	bool is_exist = false;
	for (int i = 0; i < pop_num; i++)
	{
		is_exist = child.is_same_solution(ins, pop[i]);						//child is not the same as any individual  of the population
		if (is_exist)
			return;
	}
	for (int i = 0; i < pop_num + 1; i++)
		distance[i] = ins.num_items + 1;
	for (int i = 0; i < pop_num; i++)
		f_obj[i] = pop[i].cost;
	f_obj[pop_num] = child.cost;
	for (int i = 0; i < pop_num + 1; i++)
	{
		if (f_obj[i] < f_min)
			f_min = f_obj[i];
		if (f_obj[i] > f_max)
			f_max = f_obj[i];
	}
	for (int i = 0; i < pop_num; i++)
	{		
		for (int j = i + 1; j < pop_num; j++)
		{
			common_nodes = calculate_common_nodes(ins, pop[i], pop[j]);
			if (ins.num_items - common_nodes < distance[i])
				distance[i] = ins.num_items - common_nodes;
			if (ins.num_items - common_nodes < distance[j])
				distance[j] = ins.num_items - common_nodes;
		}
		common_nodes = calculate_common_nodes(ins, pop[i], child);
		if (ins.num_items - common_nodes < distance[i])
			distance[i] = ins.num_items - common_nodes;
		if (ins.num_items - common_nodes < distance[pop_num])
			distance[pop_num] = ins.num_items - common_nodes;
	}
	for (int i = 0; i < pop_num + 1; i++)
	{
		if (distance[i] < dis_min)
			dis_min = distance[i];
		if (distance[i] > dis_max)
			dis_max = distance[i];
	}
	for (int i = 0; i < pop_num + 1; i++)
	{
		double f_ay = 1.0*(f_obj[i] - f_min) / (f_max - f_min + 1);
		double d_ay = 1.0*(distance[i] - dis_min) / (dis_max - dis_min + 1);
		score[i] = beta*f_ay + (1 - beta)*d_ay;
	}
	//for (int i = 0; i < pop_num + 1; i++)
	//	cout << "i=" << i << score[i] << endl;
	for (int i = 0; i < pop_num + 1; i++)
	{
		if (score[i] < score_worst)
		{
			score_worst = score[i];
			select_index = i;
		}
	}
	if (select_index != pop_num)
		pop[select_index].copy_solution(ins, child);
	delete []distance; distance = NULL;
	delete []f_obj; f_obj = NULL;
	delete []score; score = NULL;
}


int calculate_common_nodes(const instance_data &ins, const solution_data &sd1, const solution_data &sd2)
{
	int common = 0;
	for (int i = 0; i < ins.num_items; i++)
		if (sd1.sol[i] == sd2.sol[i])
			common++;
	return common;
}

/*calculate similarity of the population */
double calculate_similarity_pop(const instance_data &ins, const solution_data *pop, int pop_num)
{
	double sim = 0.0;
	int common_nodes = 0;
	int sum_common = 0;
	for (int i = 0; i < pop_num; i++)
	{
		for (int j = i + 1; j < pop_num; j++)
		{
			common_nodes = calculate_common_nodes(ins, pop[i], pop[j]);
			sum_common += common_nodes;
		}
	}
	sim = 1.0*sum_common / (ins.num_items *pop_num*(pop_num - 1) / 2);
	return sim;
}

void output_stat_sol(char out_file[], char sol_file[], char in_file[], const instance_data &ins, const solution_data &sd, double run_time)
{
	FILE *fp;
	char buff[MAXCAN];
	int select_num = 0;
	sprintf(buff, "%s", out_file);
	fp = fopen(buff, "a+");
	for (int i = 0; i < ins.num_items; i++)
		if (sd.sol[i])
			select_num++;
	fprintf(fp, "%s cost:%d run_time:%f  cap:%d  weight:%d  items:%d  sels:%d \n", in_file, sd.cost, run_time, ins.capacity, sd.weight, ins.num_items, select_num);
	fclose(fp);
	sprintf(buff, "%s", sol_file);
	fp = fopen(buff, "a+");
	fprintf(fp, "%s cost:%d : ", in_file, sd.cost);
	for (int i = 0; i < ins.num_items; i++)
		if (sd.sol[i])
			fprintf(fp, "%d ", i);
	fprintf(fp, "\n");
	fclose(fp);
}

void evolutionary_search(char file_in[], char file_out[], char sol_out[], instance_data &ins, int tabu_tenure, int tt_base, int ts_depth, double time_limit, double incre, int lambda, int pop_num, int ini_num, double mut_str)
{
	int *tabu_list = new int[ins.num_items];
	int total_neighbors = ins.num_items*(ins.num_items - 1) / 2 + ins.num_items;
	candidate_move *neighbors = new candidate_move[total_neighbors];
	clock_t start_time = clock();
	bool verify;
	int genes = 0;
	int cost_forfeits;
	double elasped_time;
	double run_time;
	solution_data sd(ins);
	solution_data sd_best(ins);
	NUM_ITEMS = ins.num_items;
	solution_data *pop = new solution_data[pop_num];	
	build_neighbors(ins, neighbors); 	
	initial_population(file_in, ins, pop, pop_num, ini_num, sd_best, tabu_list, tabu_tenure, tt_base, ts_depth, start_time, run_time, time_limit, incre, lambda);
	//for (int i = 0; i < pop_num; i++)
	//	cout << "i=" << i << ", cost=" << pop[i].cost << endl;
	cout << "after initial population" << endl;
	elasped_time = 1.0*(clock() - start_time) / CLOCKS_PER_SEC;
	while (elasped_time < time_limit)
	{
		uniform_crossover(ins, sd, pop, pop_num);
		//random_mutation(ins, sd, neighbors, total_neighbors, mut_str);
		cost_forfeits = sd.calculate_forfeit_cost(ins);
		sd.cost -= cost_forfeits;
		adaptive_fea_inf_tabu_search(file_in, ins, sd, sd_best, tabu_list, tabu_tenure, tt_base, ts_depth, start_time, run_time, time_limit, incre, lambda);
		//update_population(ins, sd, pop, pop_num);
		update_population_distance_quality(ins, sd, pop, pop_num);
		genes++;
		sd_best.verify_solution(ins);
		double similarity = calculate_similarity_pop(ins, pop, pop_num);
		//cout << "genes=" << genes << ", sim=" << similarity << ", sd_best.cost=" << sd_best.cost << ", run_time=" << run_time << endl;
		elasped_time = 1.0*(clock() - start_time) / CLOCKS_PER_SEC;
	}
	verify = sd_best.verify_solution(ins);
	if (verify)
	{
		cout << file_in << ", the obtained best solution is feasible, and its cost = " << sd_best.cost << ", run_time = " << run_time << endl;
		output_stat_sol(file_out, sol_out, file_in, ins, sd_best, run_time);
	}
	else
		cout << file_in << ", the obtained best solution is infeasible!!!!!!!!!!!!!!!!!!!!!!" << endl;
}
