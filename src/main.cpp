#include"typedef.h"
#include"func_state.h"

int main(int argc, char* argv[])
{
	if (argc < 5)
	{
		cout << "HESM_KPF usage: instance_file output_stat_file output_sol_file seed" << endl;
		cout << "instance_file is the instance name, output_stat_file is a file used to store the running information, output_sol_file stores the solution information, and seed is the random seed, such as 1, 2, ...";
		exit(-1);
	}
	//the following are parameters
	int tabu_tenure = 15;					
	int ts_depth = 7000;
	int pop_num = 30;						
	double incre = 2.0;						
	int lambda = 5;				
	int tt_base = 10;							
	int ini_num = 1000;						
	double mut_str = 0.10;				//it is unused
	
	double time_limit = 18;				//the cutoff time for each run
	
	char *input_file = argv[1];	
	char *out_stat_file = argv[2];
	char *out_sol_file = argv[3];
	int seed = atoi(argv[4]);
	srand(seed);
	
	instance_data ins;
	ins.read_instance_file(input_file);
	//ins.print_org_graph();

	evolutionary_search(input_file, out_stat_file, out_sol_file, ins, tabu_tenure, tt_base, ts_depth, time_limit, incre, lambda, pop_num, ini_num, mut_str);
	getchar();
	return 0;
}
