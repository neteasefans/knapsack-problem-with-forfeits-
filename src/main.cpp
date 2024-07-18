#include"typedef.h"
#include"func_state.h"

int main(int argc, char* argv[])
{
	char *input_file = "E:\\kpf_soco_instances\\LK\\1000\\10_id_220b_objs_1000_size_3000_sets_6000_maxNumConflicts_2_maxCost_15_seme_1402.txt";
	//char *input_file = "E:\\kpf_soco_instances\\kpf_soco_instances\\MF\\500\\08_id_1203_objs_500_size_1500_sets_4000_maxNumConflicts_2_maxCost_15_seme_2079.txt";
	char *out_file = "E:\\output_kpf\\HEA_AFITS_out_30March.txt";
	char *out_sol_file = "E:\\output_kpf\\HEA_AFITS_sol_30March.txt";
	double time_limit = 100;
	double incre = 2.0;						//parameter 
	int lambda = 5;							//parameter
	int tabu_tenure = 15;					
	int tt_base = 10;						
	int ts_depth = 3000;
	int pop_num = 10;						//the population size
	int ini_num = 1000;						
	double mut_str = 0.10;
	//srand(unsigned(time(NULL)));
	srand(1);
	instance_data ins;
	ins.read_instance_file(input_file);
	//ins.print_org_graph();

	evolutionary_search(input_file, out_file, out_sol_file, ins, tabu_tenure, tt_base, ts_depth, time_limit, incre, lambda, pop_num, ini_num, mut_str);
	getchar();
	return 0;
}