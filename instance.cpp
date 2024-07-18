#include"typedef.h"

/* sort in an ascending order */
bool compare_descent(const map_data &a, const map_data &b)
{
	return a.pi_wi > b.pi_wi;
}

void instance_data::read_instance_file(char in_file[])
{
	ifstream infile(in_file);
	char tmpstr[1024];
	int count_item, count_pair;
	int number1, number2, cost;
	if (!infile)
	{
		fprintf(stderr, "Can not open file %s\n", in_file);
		exit(10);
	}
	infile >> num_items >> num_forfeits >> capacity;
	item_profit = new int[num_items];
	item_weight = new int[num_items];
	cost_forfeit = new int*[num_items];
	for (int i = 0; i < num_items; i++)
	{
		cost_forfeit[i] = new int[num_items];
		memset(cost_forfeit[i], 0, sizeof(int)*num_items);					//initializing to the value zero
	}
	item_map = new  int[num_items];
	//read the item_profit
	count_item = 0;
	while (count_item < num_items)
		infile >> item_profit[count_item++];
	//read the item_weight
	count_item = 0;
	while (count_item < num_items)
		infile >> item_weight[count_item++];
	//read the cost_forfeit
	count_pair = 0;
	while (infile.getline(tmpstr, 1024))
	{
		if (strlen(tmpstr) == 0)
			continue;
		sscanf(tmpstr, "%d %d %d", &number1, &cost, &number2);
		infile.getline(tmpstr, 1024);
		sscanf(tmpstr, "%d %d", &number1, &number2);
		//cout << number1 << " " << number2 << " " << cost << endl;
		assert(number1 >= 0 && number2 < num_items && number2 >= 0 && number2 < num_items && number1 != number2);
		cost_forfeit[number1][number2] = cost_forfeit[number2][number1] = cost;
		count_pair++;
	}
	assert(count_pair == num_forfeits);
	//calculate the item_map data
	vector<map_data> sort_items(num_items);
	for (int i = 0; i < num_items; i++)
	{
		sort_items[i].item = i;
		sort_items[i].pi_wi = float(item_profit[i]) / item_weight[i];
	}
	sort(sort_items.begin(), sort_items.end(), compare_descent);
	for (int i = 0; i < num_items; i++)
		item_map[i] = sort_items[i].item;
}

void instance_data::print_org_graph()
{
	int line_cnt = 1;
	cout << num_items << " " << num_forfeits << " " << capacity << endl;
	//print the item_profit
	for (int i = 0; i < num_items; i++)
		cout << item_profit[i] << " ";
	cout << endl;
	//print the item_weight
	for (int i = 0; i < num_items; i++)
		cout << item_weight[i] << " ";
	cout << endl;
	//print the item_map
	for (int i = 0; i < num_items; i++)
		cout << item_map[i] << " ";
	cout << endl;
	cout << "read file finisehd" << endl;
}