#ifndef _TYPEDEF_H
#define _TYPEDEF_H
#include<assert.h>
#include<time.h>
#include<vector>
#include<algorithm>
#include<fstream>
#include<iostream>
#include<string.h>
#include<math.h>
using namespace std;

#define PRECISION 10e-6
#define MAXCAN 200
#define MAXVALUE 99999999
extern int NUM_ITEMS;

typedef struct candidate_move
{
	char type;						//move type including drop, add and swap
	int item1;
	int item2;
}candidate_move;

typedef struct map_data
{
	int item;
	float pi_wi;
}map_data;

class instance_data
{
public:
	void read_instance_file(char file[]);
	void print_org_graph();

	int num_items;					//the number of items
	int num_forfeits;				//the number of forfeit pairs
	int capacity;					//the capacity of knapsack
	int *item_profit;				//the profit of each item
	int *item_weight;				//the weight of each item
	int **cost_forfeit;				//the cost for each forfeit pair, cost_forfeit[i][j] = 0 indicates that the pair <i, j> have no forfeit
	int *item_map;					//record the items according the pi/wi values in an non-decreaing order
};

class solution_data
{
public:
	void initial_sol_greedy(const instance_data &ins);
	void initial_sol_random(const instance_data &ins);
	void initialing_solution_datas(int num_items);							//initial solution_data 
	void initial_forfeit_delta(const instance_data &ins);
	void update_forfeit_delta(const instance_data &ins, int item1, int item2, char type);
	void execute_move(const instance_data &ins, const candidate_move *moves, int moves_len, int *tabu_list, int tabu_tenure, int tt_base, int iter);
	void copy_solution(const instance_data &ins, const solution_data &sd_src);
	void check_move(const instance_data &ins, int cost_gs);
	bool is_same_solution(const instance_data &ins, solution_data &sd_cmp);
	bool verify_solution(const instance_data &ins);
	int calculate_forfeit_cost(const instance_data &ins);
	solution_data(const instance_data &ins);										//construction method 
	solution_data();																//default construciton method
	~solution_data();
	int *forfeit_delta;				//records the sum of forfeit of each item with respect to all the selected items
	bool *sol;						//solution representation <x0, x1, ..., x(n-1)>
	int weight;						//the total weights of all the selected items 
	int cost;						//the total profits minus the total forfeits of all the selected items
};



#endif
