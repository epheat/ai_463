// Evan Heaton
// 09/27/2016
// AI Satisfiability Programming Assignment
// hill climbing helper functions header file

#ifndef HILL_CLIMBING_H
#define HILL_CLIMBING_H

using namespace std;

bool hill_climbing(vector<vector<int> > formula, int nbvar);
vector<int> generate_solution(int nbvar);
vector<int> mutate_solution(vector<int> solution);
vector<vector<int> > get_neighbors(vector<int> solution);
int get_fitness(vector<int> solution, vector<vector<int> > formula);
string vec_to_string(vector<int> v);


#endif
