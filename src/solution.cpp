#include"typedef.h"

int NUM_ITEMS;
/*initial solution_data */
void solution_data::initialing_solution_datas(int num_items)
{
	weight = 0;
	cost = 0;
	memset(sol, false, sizeof(bool)*num_items);
	memset(forfeit_delta, 0, sizeof(int)*num_items);
}

solution_data::solution_data(const instance_data &ins)
{
	sol = new bool[ins.num_items];
	forfeit_delta = new int[ins.num_items];
	initialing_solution_datas(ins.num_items);
	//cout << "calling the deafult construction method" << endl;
}

solution_data::solution_data()
{
	sol = new bool[NUM_ITEMS];
	forfeit_delta = new int[NUM_ITEMS];
	initialing_solution_datas(NUM_ITEMS);
	//cout << "calling the deafult construction method" << endl;
}

solution_data::~solution_data()
{
	//cout << "Object is being deleted" << endl;
	delete[]sol; sol = NULL;
	delete[]forfeit_delta; forfeit_delta = NULL;
}

void solution_data::copy_solution(const instance_data &ins, const solution_data &sd_src)
{
	memcpy(sol, sd_src.sol, sizeof(bool)*ins.num_items);
	memcpy(forfeit_delta, sd_src.forfeit_delta, sizeof(int)*ins.num_items);
	cost = sd_src.cost;
	weight = sd_src.weight;
}

/*create an initial solution in a greedy way */
void solution_data::initial_sol_greedy(const instance_data &ins)
{
	int item, index = 0;
	int cost_forfeits = 0;
	bool no_full = true;	
	initialing_solution_datas(ins.num_items);
	while (no_full)
	{
		no_full = false;
		while (index < ins.num_items)
		{
			item = ins.item_map[index];
			if (weight + ins.item_weight[item] <= ins.capacity)					//satisfy the capacity constraint
			{
				weight += ins.item_weight[item];
				cost += ins.item_profit[item];
				sol[item] = true;
				no_full = true;
				index++;
				break;
			}
			else
				index++;
		}
	}	
	//minus the forfeits from the cost
	cost_forfeits = calculate_forfeit_cost(ins);
	cost -= cost_forfeits;	
}

/* random shuffle */
void random_shuffle(int *items, int n)
{
	int temp;
	for (int i = 0; i < n; i++)
	{
		//从i到n-1随机选一个元素
		int rand_num = rand() % (n - i) + i;
		//交换items[i]与items[rand_num]
		temp = items[i];
		items[i] = items[rand_num];
		items[rand_num] = temp;
	}
}

/*create an initial solution in a random way */
void solution_data::initial_sol_random(const instance_data &ins)
{
	int *random_items = new int[ins.num_items];
	int item, index = 0;
	int cost_forfeits = 0;
	bool no_full = true;
	initialing_solution_datas(ins.num_items);
	for (int i = 0; i < ins.num_items; i++)
		random_items[i] = i;
	random_shuffle(random_items, ins.num_items);
	while (no_full)
	{
		no_full = false;		
		while (index < ins.num_items)
		{
			item = random_items[index];
			if (weight + ins.item_weight[item] <= ins.capacity)					//satisfy the capacity constraint
			{
				weight += ins.item_weight[item];
				cost += ins.item_profit[item];
				sol[item] = true;
				no_full = true;
				index++;
				break;
			}
			else
				index++;
		}
	}
	//minus the forfeits from the cost
	cost_forfeits = calculate_forfeit_cost(ins);
	cost -= cost_forfeits;	
	delete[]random_items; random_items = NULL;
}

void solution_data::initial_forfeit_delta(const instance_data &ins)
{
	for (int i = 0; i < ins.num_items; i++)
	{	
		for (int j = 0; j < ins.num_items; j++)
		{
			if (sol[j] && j != i)
				forfeit_delta[i] += ins.cost_forfeit[i][j];
		}
	}
}

/*update forfeit_delta of each item, complexity: O(n) */
void solution_data::update_forfeit_delta(const instance_data &ins, int item1, int item2, char type)
{
	switch (type)
	{
	case 'd':
		for (int i = 0; i < ins.num_items; i++)
			if (i != item1)
				forfeit_delta[i] -= ins.cost_forfeit[i][item1];
		break;
	case 'a':
		for (int i = 0; i < ins.num_items; i++)
			if (i != item1)
				forfeit_delta[i] += ins.cost_forfeit[i][item1];
		break;
	case 's':
		for (int i = 0; i < ins.num_items; i++)
		{
			if (i == item1)
				forfeit_delta[i] += ins.cost_forfeit[i][item2];
			else if (i == item2)
				forfeit_delta[i] -= ins.cost_forfeit[i][item1];
			else
				forfeit_delta[i] += ins.cost_forfeit[i][item2] - ins.cost_forfeit[i][item1];
		}
		break;
	}
}

int solution_data::calculate_forfeit_cost(const instance_data &ins)
{
	int cost_forfeits = 0;
	initial_forfeit_delta(ins);
	for (int i = 0; i < ins.num_items; i++)
		if (sol[i])
			cost_forfeits += forfeit_delta[i];
	cost_forfeits /= 2;
	return cost_forfeits;
}

void solution_data::execute_move(const instance_data &ins, const candidate_move *moves, int moves_len, int *tabu_list, int tabu_tenure, int tt_base, int iter)
{
	assert(moves_len > 0);
	int rx = rand() % moves_len;
	char type = moves[rx].type;
	int item1 = moves[rx].item1;
	int item2 = moves[rx].item2;
	//cost += delta_max;
	switch (type)
	{
	case 'd':														//for drop move
		weight -= ins.item_weight[item1];
		sol[item1] = false;
		cost += -ins.item_profit[item1] + forfeit_delta[item1];
		update_forfeit_delta(ins, item1, item2, 'd');
		tabu_list[item1] = iter + tt_base + rand() % tabu_tenure;
		break;
	case 'a':														//for add move
		weight += ins.item_weight[item1];
		sol[item1] = true;
		cost += ins.item_profit[item1] - forfeit_delta[item1];
		update_forfeit_delta(ins, item1, item2, 'a');
		tabu_list[item1] = iter + tt_base;
		break;
	case 's':														//for swap move
		weight += ins.item_weight[item2] - ins.item_weight[item1];
		sol[item1] = false;
		sol[item2] = true;
		cost += ins.item_profit[item2] - ins.item_profit[item1] + forfeit_delta[item1] - forfeit_delta[item2] + ins.cost_forfeit[item1][item2];
		update_forfeit_delta(ins, item1, item2, 's');
		tabu_list[item1] = iter + tt_base + rand() % tabu_tenure;
		tabu_list[item2] = iter + tt_base;
		break;
	default:
		break;
	}
	//cout << "type=" << type << ", item1=" << item1 << ", item2=" << item2 << endl;
}

void solution_data::check_move(const instance_data &ins, int cost_gs)
{
	int check_weight = 0;
	int check_cost = 0;
	int check_cost_gs = weight <= ins.capacity ? 0 : weight - ins.capacity;
	for (int i = 0; i < ins.num_items; i++)
	{
		if (sol[i])
		{
			check_weight += ins.item_weight[i];
			check_cost += ins.item_profit[i];
			for (int j = i + 1; j < ins.num_items; j++)
			{
				if (sol[j])
					check_cost -= ins.cost_forfeit[i][j];
			}
		}
		
	}
	//cout << "check_weight=" << check_weight << ", weight=" << weight << ", capacity=" << ins.capacity << endl;
	//cout << "check_cost=" << check_cost << ", cost=" << cost << endl;
	assert(check_weight == weight);
	assert(check_cost == cost);
	assert(check_cost_gs == cost_gs);
	//assert(check_weight <= ins.capacity);
}

int calculate_fitness(const instance_data &ins, const bool* sol, int& weight)
{
	int cost = 0;
	weight = 0;
	for (int i = 0; i < ins.num_items; i++)
	{
		if (sol[i])
		{
			weight += ins.item_weight[i];
			cost += ins.item_profit[i];
			for (int j = i + 1; j < ins.num_items; j++)
			{
				if (sol[j])
					cost -= ins.cost_forfeit[i][j];
			}
		}
	}	
	return cost;
}

/*check the obtained solution is feasible or not*/
bool solution_data::verify_solution(const instance_data &ins)
{
	int check_weight = 0;
	int check_cost = 0;
	check_cost = calculate_fitness(ins, sol, check_weight);
	assert(check_weight == weight);
	assert(check_cost == cost);
	assert(check_weight <= ins.capacity);
	return true;
}

bool solution_data::is_same_solution(const instance_data &ins, solution_data &sd_cmp)
{
	bool is_exist = true;
	for (int i = 0; i < ins.num_items; i++)
	{
		if (sol[i] != sd_cmp.sol[i])
		{
			is_exist = false;
			break;
		}
	}
	return is_exist;
}
