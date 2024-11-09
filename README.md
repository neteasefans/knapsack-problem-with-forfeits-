# Adaptive feasible and infeasible evolutionary search for the knapsack problem with forfeits (KPF)
The 120 benchmark instances used in our paper titled with "Adaptive feasible and infeasible evolutionary search for the knapsack problem with forfeits" were firstly introduced by Dr. Cerulli (raffaele@unisa.it) and friendly provided by Dr. Andera Raiconi (araiconi@unisa.it).  To facilitate the further research, we upload the instances here.

The source code given in 'src' directory implements the proposed HESM method described in our paper.

We made comparisons between HESM and some state-of-the-art methods from the following related KPF works::

1. Cerulli, R., D'Ambrosio, C., Raiconi, A., \& Vitale, G. (2020). The Knapsack Problem with Forfeits. In: Ba\"{i}ou, M., Gendron, B., G\"{u}nl\"{u}k, O., \& Mahjoub, A. R. (Eds.), In International Symposium on Combinatorial Optimization, ISCO2020, Vol 12176 (pp. 263-272). Cham: Springer.

2. Capobianco, G., D'Ambrosio, C., Pavone, L., Raiconi, A., Vitale, G., \& Sebastiano, F. (2022). A hybrid metaheuristic for the Knapsack Problem with Forfeits. Soft Computing, 26(2), 749-762.

3. D’Ambrosio, C., Laureana, F., Raiconi, A., & Vitale, G. (2023). The Knapsack Problem with forfeit sets. Computers & Operations Research, 151, 106093.

** Instructions to use the source code of HESM

*** To compile:

q.zhou$ make

q.zhou$

*** To run:

q.zhou$ ./HESM_KPF ./instance_file ./output_stat_file ./output_sol_file seed 

(where "instance_file is the instance name, output_stat_file is a file used to store the running information, output_sol_file stores the solution information,
and seed is the random seed, such as 1, 2, ...")

q.zhou$

*** To clean

q.zhou$ make clean

q.zhou$



